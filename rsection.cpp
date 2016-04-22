#include "rsection.h"
#include "rsectionmodel.h"
#include "rmapobjectmovepolicy.h"
#include "rmapobjectresizepolicy.h"
#include "rcompositemapobject.h"
#include "rmapobjecttreewidgetitem.h"
#include "rmapregistry.h"

#include <QPaintEvent>
#include <QPainter>
#include <QList>
#include <QDebug>
#include <QPixmap>

//--------------------------------------------------------------------------------------
RSection::RSection(QWidget *parent) : RMapObject(parent)
{
    setModel(new RSectionModel);
	gripTriangle << QPoint(0, 0) << QPoint(20, 0) << QPoint(0, 20);
}

//--------------------------------------------------------------------------------------
RSection::~RSection()
{
    RMapRegistry::instance().addSectionForDelete(model()->mapObjectID());
}

//--------------------------------------------------------------------------------------
RSection *RSection::clone() const
{
	RSection *section = new RSection(parentWidget());
	section->setModel(model()->clone());
	section->setMovePolicy(movePolicy()->clone());
	section->setResizePolicy(resizePolicy()->clone());
	section->setGeometry(geometry());
	section->setVisible(isVisible());

    if (QTreeWidget *treeWidget = treeWidgetItem()->treeWidget()) {
        treeWidget->insertTopLevelItem(treeWidget->topLevelItemCount(), section->treeWidgetItem());
    }

	foreach (QObject *obj, children()) {
		if (RMapObject *o = qobject_cast<RMapObject *>(obj)) {
			RMapObject *child = o->clone();
            child->setParent(section);
            child->show();

            child->treeWidgetItem()->parent()->removeChild(child->treeWidgetItem());

            section->treeWidgetItem()->insertChild(
                section->treeWidgetItem()->childCount(), child->treeWidgetItem());
		}
	}

    section->treeWidgetItem()->setExpanded(true);
	return section;
}

//--------------------------------------------------------------------------------------
RSectionModel *RSection::model() const
{ return static_cast<RSectionModel *>(RMapObject::model()); }

//--------------------------------------------------------------------------------------
void RSection::setMovePolicy(RMapObjectMovePolicy *policy)
{
	policy->setMovePolygon(gripTriangle);
	RMapObject::setMovePolicy(policy);
}

//--------------------------------------------------------------------------------------
const QPolygon &RSection::movePolygon() const
{ return gripTriangle; }

//--------------------------------------------------------------------------------------
void RSection::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPen pen;

	if (hasFocusMapObject()) {
		pen.setWidth(10);
	} else {
		pen.setWidth(5);
	}

    painter.fillRect(rect(), Qt::white);

    const QImage &image = model()->sectionPicture();
    painter.drawImage(rect().center() - image.rect().center(), image);

    pen.setColor(model()->sectionFontColor());
    painter.setPen(pen);
    painter.setFont(model()->sectionFont());
    painter.drawText(rect(), model()->sectionName(), QTextOption(Qt::AlignCenter));

    pen.setColor(model()->sectionFrameColor());
	painter.setPen(pen);
    painter.drawRect(rect());

	pen.setWidth(1);
	painter.setPen(pen);
	painter.setBrush(QBrush(QColor(Qt::red), Qt::SolidPattern));
	painter.drawPolygon(gripTriangle);

	if (!mouseMovePoint.isNull()) {
		pen.setStyle(Qt::DotLine);
		pen.setColor(Qt::black);
		painter.setPen(pen);
		painter.setBrush(QBrush());
		QPoint to = mouseMovePoint - mouseDownPoint;
		painter.drawRect(mouseDownPoint.x(), mouseDownPoint.y(), to.x(), to.y());
	}

	RMapObject::paintEvent(event);
}

//--------------------------------------------------------------------------------------
void RSection::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		if (!childAt(event->pos())) {
			mouseDownPoint = event->pos();
		}
	}

	RMapObject::mousePressEvent(event);
}

//--------------------------------------------------------------------------------------
void RSection::mouseMoveEvent(QMouseEvent *event)
{
	if (hasFocusMapObject() && (event->buttons() & Qt::LeftButton)) {
		if (cursor().shape() == Qt::ArrowCursor) {
			mouseMovePoint = event->pos();
			update();
		}
	}

	RMapObject::mouseMoveEvent(event);
}

//--------------------------------------------------------------------------------------
void RSection::mouseReleaseEvent(QMouseEvent *event)
{
	if (!mouseMovePoint.isNull()) {
		QPoint to = mouseMovePoint - mouseDownPoint;
		selectChild(QRect(mouseDownPoint.x(), mouseDownPoint.y(), to.x(), to.y()));
	}

	mouseDownPoint = QPoint();
	mouseMovePoint = QPoint();
	update();

	RMapObject::mouseReleaseEvent(event);
}

//--------------------------------------------------------------------------------------
void RSection::selectChild(const QRect &r)
{
	QList<RMapObject *> list;

	foreach (QObject *obj, children()) {
		if (RMapObject *mapObj = qobject_cast<RMapObject *>(obj)) {
			if (r.contains(mapObj->geometry(), true)) {
				list.push_back(mapObj);
			}
		}
	}

    if (list.size() == 1) {
        list.at(0)->setFocusMapObject();
    } else if (list.size() > 1) {
        RCompositeMapObject::createInstance(this, movePolicy()->clone(), list);
    }
}
