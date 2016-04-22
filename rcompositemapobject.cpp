#include "rcompositemapobject.h"
#include "rmapobjectmovepolicy.h"
#include "rmapobjectresizepolicy.h"
#include "rsection.h"
#include "rtable.h"
#include "rmapobjecttreewidgetitem.h"
#include "rmapobjectresizepolicy.h"
#include "rcompositemapobjectmodel.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QAction>
#include <QInputDialog>

namespace {
bool cmpTablesByRows(const RTable *tbl1, const RTable *tbl2)
{
    return tbl1->geometry().top() == tbl2->geometry().top()
            ? tbl1->geometry().left() < tbl2->geometry().left()
            : tbl1->geometry().top() < tbl2->geometry().top();
}
}

RCompositeMapObject *RCompositeMapObject::m_instance = 0;

//--------------------------------------------------------------------------------------
void RCompositeMapObject::createInstance(
        RMapObject *parent,
        RMapObjectMovePolicy *movePolicy,
        const QList<RMapObject *> &list)
{
    RCompositeMapObject *compositeMapObject = new RCompositeMapObject(parent);
    compositeMapObject->setModel(new RCompositeMapObjectModel);
    compositeMapObject->setMovePolicy(movePolicy);
    compositeMapObject->setResizePolicy(new RMapObjectNoResizePolicy);
    compositeMapObject->setMapObjectList(list);
    compositeMapObject->show();
    compositeMapObject->setFocusMapObject();
    compositeMapObject->model()->setModified(false);
    m_instance = compositeMapObject;
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::deleteInstance()
{
    if (m_instance) {
        m_instance->self_deleted = true;
        delete m_instance;
    }
}

//--------------------------------------------------------------------------------------
RCompositeMapObject *RCompositeMapObject::instance()
{ return m_instance; }

//--------------------------------------------------------------------------------------
RCompositeMapObject::RCompositeMapObject(QWidget *parent) :
    RMapObject(parent),
    mouse_pressed(false),
    self_deleted(false)
{
    treeWidgetItem()->setHidden(true);

    QAction *renumberTablesAction = new QAction(tr("Пронумеровать столики"), this);
    parentWidget()->parentWidget()->addAction(renumberTablesAction);
    connect(renumberTablesAction, SIGNAL(triggered()), this, SLOT(onRenumberTables()));
}

//--------------------------------------------------------------------------------------
RCompositeMapObject::~RCompositeMapObject()
{
    if (self_deleted) {
        // сами удалились, восстанавливаем сигналы
        foreach (RMapObject *obj, map_object_list) {
            obj->model()->blockSignals(false);

            // если наша модель была обновлена - обновляем и дочерние модели
            if (model()->isModified()) {
                obj->model()->setModified(true);
            }
        }
    } else {
        // нас принудительно удалили, удаляем и объекты
        foreach (RMapObject *obj, map_object_list) {
            delete obj;
        }
    }

    m_instance = 0;
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::onRenumberTables()
{
    QList<RTable *> tablesList;

    foreach (RMapObject *obj, map_object_list) {
        if (RTable *table = qobject_cast<RTable *>(obj)) {
            tablesList.push_back(table);
        }
    }

    // отсортируем список в нужном нам направлении
    qSort(tablesList.begin(), tablesList.end(), cmpTablesByRows);

    bool isOk = false;
    int number = QInputDialog::getInt(this, tr("Нумерация столиков"), tr("Начальный номер"), 1, 1, 999999, 1, &isOk);

    if (isOk) {
        foreach (RTable *table, tablesList) {
            table->model()->setNumber(number++);
            table->model()->setModified(true);
        }
    }
}

//--------------------------------------------------------------------------------------
RCompositeMapObject *RCompositeMapObject::clone() const
{   
	RCompositeMapObject *compositeObject = new RCompositeMapObject(parentWidget());
    compositeObject->setModel(model()->clone());
    compositeObject->setMovePolicy(movePolicy()->clone());
	compositeObject->setResizePolicy(resizePolicy()->clone());

    foreach (RMapObject *mapObj, map_object_list) {
		compositeObject->addMapObject(mapObj->clone());
	}

	compositeObject->setGeometry(geometry());
	compositeObject->setVisible(isVisible());
    compositeObject->setFocusMapObject();
    compositeObject->model()->setModified(false);

    // старый объект удалим
    self_deleted = true;
    delete m_instance;
    m_instance = compositeObject;

    // тут уже нельзя производить никаких операций с объектом this!

	return compositeObject;
}

// добавить объект в набор
//--------------------------------------------------------------------------------------
void RCompositeMapObject::addMapObject(RMapObject *obj)
{
    // заблокируем сигналы модели выбранного объекта
    obj->model()->blockSignals(true);

    map_object_list.push_back(obj);

    QRect newGeom = obj->geometry();

    foreach (RMapObject *mapObj, map_object_list) {
		newGeom.setLeft(qMin(newGeom.left(), mapObj->geometry().left()));
		newGeom.setTop(qMin(newGeom.top(), mapObj->geometry().top()));
		newGeom.setRight(qMax(newGeom.right(), mapObj->geometry().right()));
		newGeom.setBottom(qMax(newGeom.bottom(), mapObj->geometry().bottom()));
    }

    setGeometry(newGeom);
}

// извлечь объект из набора
//--------------------------------------------------------------------------------------
void RCompositeMapObject::removeMapObject(RMapObject *obj)
{
    if (map_object_list.removeOne(obj)) {
        obj->model()->blockSignals(false);
    }
}

//--------------------------------------------------------------------------------------
const QList<RMapObject *> RCompositeMapObject::mapObjectList() const
{ return map_object_list; }

//--------------------------------------------------------------------------------------
void RCompositeMapObject::setMapObjectList(const QList<RMapObject *> &list)
{
	foreach (RMapObject *mapObj, list) {
		addMapObject(mapObj);
	}
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::removeAllMapObject()
{
    foreach (RMapObject *obj, map_object_list) {
        obj->model()->blockSignals(false);
    }

    map_object_list.clear();
}

//--------------------------------------------------------------------------------------
bool RCompositeMapObject::isEmpty() const
{ return map_object_list.empty(); }

//--------------------------------------------------------------------------------------
void RCompositeMapObject::paintEvent(QPaintEvent *event)
{
    if (!hasFocusMapObject()) {
        self_deleted = true;
        deleteLater();
        return;
    }

	QPainter painter(this);
	QPen pen;

	pen.setWidth(4);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    painter.drawRect(rect());

	RMapObject::paintEvent(event);
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::resizeEvent(QResizeEvent *event)
{
	movePolicy()->setMovePolygon(QPolygon(rect().adjusted(5, 5, -5, -5), true));
	RMapObject::resizeEvent(event);
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::mousePressEvent(QMouseEvent *event)
{
    foreach (RMapObject *obj, map_object_list) {
		if (obj->geometry().contains(mapToParent(event->pos()), true)) {
			mouse_pressed = true;
			return RMapObject::mousePressEvent(event);
		}
    }

    static_cast<RMapObject *>(parentWidget())->setFocusMapObject();
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::mouseMoveEvent(QMouseEvent *event)
{
	if (mouse_pressed) {
		return RMapObject::mouseMoveEvent(event);
	}

    foreach (RMapObject *obj, map_object_list) {
		if (obj->geometry().contains(mapToParent(event->pos()), true)) {
			return RMapObject::mouseMoveEvent(event);
		}
	}

	if (cursor().shape() != Qt::ArrowCursor) {
		setCursor(Qt::ArrowCursor);
	}
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::mouseReleaseEvent(QMouseEvent *event)
{
	mouse_pressed = false;
	RMapObject::mouseReleaseEvent(event);
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::moveEvent(QMoveEvent *event)
{
	RMapObject::moveEvent(event);

    foreach (RMapObject *obj, map_object_list) {
		QRect geom = obj->geometry();
		geom.moveTopLeft(geom.topLeft() + event->pos() - event->oldPos());
		obj->setGeometry(geom);
	}
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::focusSetEvent()
{
    if (QTreeWidget *treeWidget = treeWidgetItem()->treeWidget()) {
        treeWidget->selectionModel()->clearSelection();
		treeWidget->setCurrentItem(treeWidgetItem());
    }

    foreach (RMapObject *obj, map_object_list) {
        obj->treeWidgetItem()->setSelected(true);
    }
}

//--------------------------------------------------------------------------------------
void RCompositeMapObject::focusLostEvent()
{
    if (QTreeWidget *treeWidget = treeWidgetItem()->treeWidget()) {
        treeWidget->selectionModel()->clearSelection();
    }
}
