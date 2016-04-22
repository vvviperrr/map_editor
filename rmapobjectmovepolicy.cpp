#include "rmapobjectmovepolicy.h"
#include "rmapobject.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

//--------------------------------------------------------------------------------------
RMapObjectMovePolicy::RMapObjectMovePolicy(RMapObject *obj /* = 0 */) :
	RMapObjectBehaviourPolicy(obj),
	is_clipped(true)
{}

//--------------------------------------------------------------------------------------
bool RMapObjectMovePolicy::mousePressEvent(QMouseEvent *event)
{
	mouse_down_point = event->pos();
	return false;
}

//--------------------------------------------------------------------------------------
bool RMapObjectMovePolicy::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	mouse_down_point = QPoint();
	return false;
}

//--------------------------------------------------------------------------------------
bool RMapObjectMovePolicy::mouseMoveEvent(QMouseEvent *event)
{
	Q_UNUSED(event);
	return false;
}

//--------------------------------------------------------------------------------------
bool RMapObjectMovePolicy::keyPressEvent(QKeyEvent *event)
{
	Q_UNUSED(event);
	return false;
}

//--------------------------------------------------------------------------------------
const QPolygon &RMapObjectMovePolicy::movePolygon() const
{ return move_polygon; }

//--------------------------------------------------------------------------------------
void RMapObjectMovePolicy::setMovePolygon(const QPolygon &polygon)
{
	move_polygon = polygon;
}

//--------------------------------------------------------------------------------------
const QPoint &RMapObjectMovePolicy::mouseDownPoint() const
{ return mouse_down_point; }

//--------------------------------------------------------------------------------------
bool RMapObjectMovePolicy::isMoveClip() const
{ return is_clipped; }

//--------------------------------------------------------------------------------------
void RMapObjectMovePolicy::setMoveClip(bool clip)
{ is_clipped = clip; }

//--------------------------------------------------------------------------------------
void RMapObjectMovePolicy::adjustClippingPos(int &row, int &col)
{
	if (!is_clipped)
		return;

	const QRect &r = mapObject()->parentWidget()->rect();

    if (row < r.x())
		row = r.x();

    if (col < r.y())
		col = r.y();

    if (row + mapObject()->width() > r.width())
        row = r.width() - mapObject()->width();

    if (col + mapObject()->height() > r.height())
        col = r.height() - mapObject()->height();
}


//--------------------------------------------------------------------------------------
RMapObjectNoMovePolicy::RMapObjectNoMovePolicy() :
	RMapObjectMovePolicy(0)
{}

//--------------------------------------------------------------------------------------
RMapObjectNoMovePolicy *RMapObjectNoMovePolicy::clone() const
{ return new RMapObjectNoMovePolicy; }


//--------------------------------------------------------------------------------------
RMapObjectSimpleMovePolicy::RMapObjectSimpleMovePolicy(RMapObject *obj /* = 0 */) :
	RMapObjectMovePolicy(obj)
{}

//--------------------------------------------------------------------------------------
RMapObjectSimpleMovePolicy *RMapObjectSimpleMovePolicy::clone() const
{ return new RMapObjectSimpleMovePolicy(mapObject()); }

//--------------------------------------------------------------------------------------
bool RMapObjectSimpleMovePolicy::mouseMoveEvent(QMouseEvent *event)
{
	bool ret = false;

	if (RMapObject *obj = mapObject()) {
		if (obj->hasFocusMapObject() && (event->buttons() & Qt::LeftButton)) {
			if (obj->cursor().shape() == Qt::SizeAllCursor) {
				QPoint p = obj->mapToParent(event->pos() - mouseDownPoint());
				int row = p.x();
				int col = p.y();
				adjustClippingPos(row, col);
				obj->move(row, col);
				ret = true;
			}
		} else {
			if (movePolygon().containsPoint(event->pos(), Qt::OddEvenFill)) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeAllCursor) {
					obj->setCursor(Qt::SizeAllCursor);
				}
			} else {
				ret = false;
				if (obj->cursor().shape() != Qt::ArrowCursor) {
					obj->setCursor(Qt::ArrowCursor);
				}
			}
		}
	}

	return ret;
}

//--------------------------------------------------------------------------------------
bool RMapObjectSimpleMovePolicy::keyPressEvent(QKeyEvent *event)
{
    const QRect &r = mapObject()->parentWidget()->rect();

	switch (event->key()) {
	case Qt::Key_Left:
        if (mapObject()->geometry().left() > 0)
            mapObject()->move(mapObject()->geometry().topLeft() - QPoint(1, 0));
		return true;

	case Qt::Key_Right:
        if (mapObject()->geometry().right() < r.width())
            mapObject()->move(mapObject()->geometry().topLeft() + QPoint(1, 0));
		return true;

	case Qt::Key_Up:
        if (mapObject()->geometry().top() > 0)
            mapObject()->move(mapObject()->geometry().topLeft() - QPoint(0, 1));
		return true;

	case Qt::Key_Down:
        if (mapObject()->geometry().bottom() < r.height())
            mapObject()->move(mapObject()->geometry().topLeft() + QPoint(0, 1));
		return true;

	default:
		return false;
	}
}


//--------------------------------------------------------------------------------------
RMapObjectGridMovePolicy::RMapObjectGridMovePolicy(size_t size, RMapObject *obj /* = 0 */) :
	RMapObjectMovePolicy(obj),
	gridSize(size)
{
}

//--------------------------------------------------------------------------------------
RMapObjectGridMovePolicy *RMapObjectGridMovePolicy::clone() const
{
	RMapObjectGridMovePolicy *policy = new RMapObjectGridMovePolicy(gridSize, mapObject());
	policy->setMovePolygon(movePolygon());
	return policy;
}

//--------------------------------------------------------------------------------------
bool RMapObjectGridMovePolicy::mouseMoveEvent(QMouseEvent *event)
{
	bool ret = false;

	if (RMapObject *obj = mapObject()) {
		if (obj->hasFocusMapObject() && (event->buttons() & Qt::LeftButton)) {
			if (obj->cursor().shape() == Qt::SizeAllCursor) {
				doMove(obj->mapToParent(event->pos() - obj->rect().topLeft() - mouseDownPoint()));
				ret = true;
			}
		} else {
			if (movePolygon().containsPoint(event->pos(), Qt::OddEvenFill)) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeAllCursor) {
					obj->setCursor(Qt::SizeAllCursor);
				}
			} else {
				ret = false;
				if (obj->cursor().shape() != Qt::ArrowCursor) {
					obj->setCursor(Qt::ArrowCursor);
				}
			}
		}
	}

	return ret;
}

//--------------------------------------------------------------------------------------
void RMapObjectGridMovePolicy::doMove(const QPoint &p)
{
    int row = (p.x() / gridSize) * gridSize;
    int col = (p.y() / gridSize) * gridSize;

    adjustClippingPos(row, col);

	if (RMapObject *obj = mapObject()) {
		if (obj->geometry().x() != row || obj->geometry().y() != col) {
			obj->move(row, col);
		}
	}
}

//--------------------------------------------------------------------------------------
bool RMapObjectGridMovePolicy::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Left:
		doMove(QPoint(mapObject()->geometry().x() - gridSize, mapObject()->geometry().y()));
		return true;

	case Qt::Key_Right:
		doMove(QPoint(mapObject()->geometry().x() + gridSize, mapObject()->geometry().y()));
		return true;

	case Qt::Key_Up:
		doMove(QPoint(mapObject()->geometry().x(), mapObject()->geometry().y() - gridSize));
		return true;

	case Qt::Key_Down:
		doMove(QPoint(mapObject()->geometry().x(), mapObject()->geometry().y() + gridSize));
		return true;

	default:
		return false;
	}
}
