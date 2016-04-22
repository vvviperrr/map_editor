#include "rmapobject.h"
#include "rmapobjectmodel.h"
#include "rmapobjectmovepolicy.h"
#include "rmapobjectresizepolicy.h"
#include "rmapobjecttreewidgetitem.h"
#include "rmapobjectfocusholder.h"
#include "rmapregistry.h"

#include <QMouseEvent>
#include <QDebug>

//--------------------------------------------------------------------------------------
RMapObject::RMapObject(QWidget *parent) :
	QWidget(parent),
	mapObjectModel(0),
	mapObjectMovePolicy(0),
    mapObjectResizePolicy(0),
    mapObjectTreeWidgetItem(new RMapObjectTreeWidgetItem(this))
{
	setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    if (RMapObject *parentMapObject = qobject_cast<RMapObject *>(parent)) {
        int count = parentMapObject->treeWidgetItem()->childCount();
        parentMapObject->treeWidgetItem()->insertChild(count, mapObjectTreeWidgetItem);
        parentMapObject->treeWidgetItem()->setExpanded(true);
    }
}

//--------------------------------------------------------------------------------------
RMapObject::~RMapObject()
{
    if (hasFocusMapObject())
        RMapObjectFocusHolder::instance().resetFocusMapObject();

    delete mapObjectTreeWidgetItem;
    delete mapObjectModel;
	delete mapObjectMovePolicy;
    delete mapObjectResizePolicy;
}

//--------------------------------------------------------------------------------------
bool RMapObject::hasFocusMapObject() const
{ return RMapObjectFocusHolder::instance().getFocusMapObject() == this; }

//--------------------------------------------------------------------------------------
void RMapObject::setFocusMapObject()
{ RMapObjectFocusHolder::instance().setFocusMapObject(this, true); }

//--------------------------------------------------------------------------------------
void RMapObject::focusSetEvent()
{
	if (QTreeWidget *treeWidget = treeWidgetItem()->treeWidget()) {
		treeWidget->setCurrentItem(treeWidgetItem());
		treeWidgetItem()->setSelected(true);
    }
}

//--------------------------------------------------------------------------------------
void RMapObject::focusLostEvent()
{}

//--------------------------------------------------------------------------------------
RMapObjectModel *RMapObject::model() const
{ return mapObjectModel; }

//--------------------------------------------------------------------------------------
void RMapObject::setModel(RMapObjectModel *m)
{
	if (!m)
		return;

	if (mapObjectModel) {
		disconnect(mapObjectModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)));
		delete mapObjectModel;
	}

	mapObjectModel = m;
	connect(mapObjectModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(modelDataChanged(QModelIndex,QModelIndex)));

    // обновим параметры в соответствии с новой моделью
    update();
    setGeometry(model()->mapObjectGeometry());
    updateTreeWidgetItem();
}

//--------------------------------------------------------------------------------------
RMapObjectMovePolicy *RMapObject::movePolicy() const
{ return mapObjectMovePolicy; }

//--------------------------------------------------------------------------------------
void RMapObject::setMovePolicy(RMapObjectMovePolicy *policy)
{
	if (!policy)
		return;

	if (mapObjectMovePolicy)
		delete mapObjectMovePolicy;

	mapObjectMovePolicy = policy;
	mapObjectMovePolicy->setMapObject(this);
}

//--------------------------------------------------------------------------------------
RMapObjectResizePolicy *RMapObject::resizePolicy() const
{ return mapObjectResizePolicy; }

//--------------------------------------------------------------------------------------
void RMapObject::setResizePolicy(RMapObjectResizePolicy *policy)
{
	if (!policy)
		return;

	if (mapObjectResizePolicy)
		delete mapObjectResizePolicy;

	mapObjectResizePolicy = policy;
	mapObjectResizePolicy->setMapObject(this);
}

//--------------------------------------------------------------------------------------
RMapObjectTreeWidgetItem *RMapObject::treeWidgetItem() const
{ return mapObjectTreeWidgetItem; }

//--------------------------------------------------------------------------------------
void RMapObject::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	Q_UNUSED(topLeft);
	Q_UNUSED(bottomRight);

    update();
    setGeometry(model()->mapObjectGeometry());
	updateTreeWidgetItem();
    model()->setModified(true);
    RMapRegistry::instance().setModified(true);
}

//--------------------------------------------------------------------------------------
void RMapObject::updateTreeWidgetItem()
{ mapObjectTreeWidgetItem->setText(0, model()->caption()); }

//--------------------------------------------------------------------------------------
void RMapObject::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & (Qt::LeftButton | Qt::RightButton)) {
		if (!childAt(event->pos())) {
            RMapObjectFocusHolder::instance().setFocusMapObject(this);
			movePolicy()->mousePressEvent(event);
		}
    }
}

//--------------------------------------------------------------------------------------
void RMapObject::mouseMoveEvent(QMouseEvent *event)
{
	if (movePolicy()->mouseMoveEvent(event))
		return;

	if (resizePolicy()->mouseMoveEvent(event))
		return;
}

//--------------------------------------------------------------------------------------
void RMapObject::mouseReleaseEvent(QMouseEvent *event)
{ movePolicy()->mouseReleaseEvent(event); }

//--------------------------------------------------------------------------------------
void RMapObject::keyPressEvent(QKeyEvent *event)
{ movePolicy()->keyPressEvent(event); }

//--------------------------------------------------------------------------------------
void RMapObject::resizeEvent(QResizeEvent *event)
{
    if (RMapObjectModel *m = model()) {
        m->setMapObjectGeometry(geometry());
    }
}

//--------------------------------------------------------------------------------------
void RMapObject::moveEvent(QMoveEvent *event)
{
    if (RMapObjectModel *m = model()) {
        m->setMapObjectGeometry(geometry());
    }
}
