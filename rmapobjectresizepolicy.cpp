#include "rmapobjectresizepolicy.h"
#include "rmapobject.h"

#include <QMouseEvent>
#include <QDebug>

//--------------------------------------------------------------------------------------
RMapObjectResizePolicy::RMapObjectResizePolicy(RMapObject *obj /* = 0 */) :
    RMapObjectBehaviourPolicy(obj),
    invertResize(false)
{}

//--------------------------------------------------------------------------------------
bool RMapObjectResizePolicy::mouseMoveEvent(QMouseEvent *event)
{
	Q_UNUSED(event);
	return false;
}


//--------------------------------------------------------------------------------------
RMapObjectNoResizePolicy::RMapObjectNoResizePolicy() :
	RMapObjectResizePolicy(0)
{}

//--------------------------------------------------------------------------------------
RMapObjectNoResizePolicy *RMapObjectNoResizePolicy::clone() const
{ return new RMapObjectNoResizePolicy; }


//--------------------------------------------------------------------------------------
RMapObjectSimpleResizePolicy::RMapObjectSimpleResizePolicy(RMapObject *obj /* = 0 */) :
		RMapObjectResizePolicy(obj)
{}

//--------------------------------------------------------------------------------------
RMapObjectSimpleResizePolicy *RMapObjectSimpleResizePolicy::clone() const
{ return new RMapObjectSimpleResizePolicy(mapObject()); }

//--------------------------------------------------------------------------------------
bool RMapObjectSimpleResizePolicy::mouseMoveEvent(QMouseEvent *event)
{
	bool ret = false;

	if (RMapObject *obj = mapObject()) {
		if (obj->hasFocusMapObject() && (event->buttons() & Qt::LeftButton)) {
			QPoint p = obj->mapToParent(event->pos() -obj->geometry().topLeft());

			switch (obj->cursor().shape()) {
			case Qt::SizeFDiagCursor:
				obj->resize(p.x(), p.y());
				ret = true;
				break;

			case Qt::SizeVerCursor:
                if (invertResize) {
                    QRect r = obj->geometry();
                    r.setTop(r.top() + p.y());
                    obj->setGeometry(r);
                } else {
                    obj->resize(obj->geometry().width(), p.y());
                }
				ret = true;
				break;

			case Qt::SizeHorCursor:
                if (invertResize) {
                    QRect r = obj->geometry();
                    r.setLeft(r.left() + p.x());
                    obj->setGeometry(r);
                } else {
                    obj->resize(p.x(), obj->geometry().height());
                }
				ret = true;
				break;

			default:
				ret = false;
				break;
			}
		} else {
			QPoint curPos = obj->mapToParent(event->pos());
			QPoint diff = obj->geometry().bottomRight() - curPos;

			if (diff.x() <= 6 && diff.y() <= 6) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeFDiagCursor) {
					obj->setCursor(Qt::SizeFDiagCursor);
                    invertResize = false;
				}
			} else if (abs(curPos.y() - obj->geometry().bottom()) <= 6) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeVerCursor) {
					obj->setCursor(Qt::SizeVerCursor);
                    invertResize = false;
				}
			} else if (abs(curPos.x() - obj->geometry().right()) <= 6) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeHorCursor) {
					obj->setCursor(Qt::SizeHorCursor);
                    invertResize = false;
				}
            } else if (abs(curPos.x() - obj->geometry().left()) <= 6) {
                ret = true;
                if (obj->cursor().shape() != Qt::SizeHorCursor) {
                    obj->setCursor(Qt::SizeHorCursor);
                    invertResize = true;
                }
            } else if (abs(curPos.y() - obj->geometry().top()) <= 6) {
                ret = true;
                if (obj->cursor().shape() != Qt::SizeVerCursor) {
                    obj->setCursor(Qt::SizeVerCursor);
                    invertResize = true;
                }
            }
            else {
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
RMapObjectGridResizePolicy::RMapObjectGridResizePolicy(size_t size, RMapObject *obj /* = 0 */) :
    RMapObjectResizePolicy(obj),
    gridSize(size)
{

}

//--------------------------------------------------------------------------------------
RMapObjectGridResizePolicy *RMapObjectGridResizePolicy::clone() const
{ return new RMapObjectGridResizePolicy(gridSize, mapObject()); }

//--------------------------------------------------------------------------------------
bool RMapObjectGridResizePolicy::mouseMoveEvent(QMouseEvent *event)
{
	bool ret = false;

	if (RMapObject *obj = mapObject()) {
		if (obj->hasFocusMapObject() && (event->buttons() & Qt::LeftButton)) {
			QPoint p = obj->mapToParent(event->pos() - obj->geometry().topLeft());

			switch (obj->cursor().shape()) {
			case Qt::SizeFDiagCursor:
				p += QPoint(p.x() % gridSize, p.y() % gridSize);
				doResize(p.x(), p.y());
				ret = true;
				break;

			case Qt::SizeVerCursor:
				if (invertResize) {
					doResize(obj->geometry().width(), p.y() + p.y() % gridSize - gridSize / 2);
				} else {
					doResize(obj->geometry().width(), p.y() + p.y() % gridSize);
				}
				ret = true;
				break;

			case Qt::SizeHorCursor:
				if (invertResize) {
					doResize(p.x() + p.x() % gridSize - gridSize / 2, obj->geometry().height());
				} else {
					doResize(p.x() + p.x() % gridSize, obj->geometry().height());
				}
				ret = true;
				break;

			default:
				ret = false;
				break;
			}
		} else {
			QPoint curPos = obj->mapToParent(event->pos());
			QPoint diff = obj->geometry().bottomRight() - curPos;

			if (diff.x() <= 6 && diff.y() <= 6) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeFDiagCursor) {
					obj->setCursor(Qt::SizeFDiagCursor);
					invertResize = false;
				}
			} else if (abs(curPos.y() - obj->geometry().bottom()) <= 6) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeVerCursor) {
					obj->setCursor(Qt::SizeVerCursor);
					invertResize = false;
				}
			} else if (abs(curPos.x() - obj->geometry().right()) <= 6) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeHorCursor) {
					obj->setCursor(Qt::SizeHorCursor);
					invertResize = false;
				}
			} else if (abs(curPos.y() - obj->geometry().top()) <= 6) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeVerCursor) {
					obj->setCursor(Qt::SizeVerCursor);
					invertResize = true;
				}
			} else if (abs(curPos.x() - obj->geometry().left()) <= 6) {
				ret = true;
				if (obj->cursor().shape() != Qt::SizeHorCursor) {
					obj->setCursor(Qt::SizeHorCursor);
					invertResize = true;
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
void RMapObjectGridResizePolicy::doResize(int w, int h)
{
	int row = (abs(w) / gridSize) * gridSize;
	int col = (abs(h) / gridSize) * gridSize;

	if (w < 0)
		row = -row;

	if (h < 0)
		col = -col;

	RMapObject *obj = mapObject();

	if (!obj)
		return;

	if (!invertResize) {
		if (obj->geometry().width() != row || obj->geometry().height() != col) {
			obj->resize(row, col);
		}
	} else {
		if (row && obj->cursor().shape() == Qt::SizeHorCursor) {
			obj->setGeometry(obj->geometry().left() + row, obj->geometry().y(), obj->geometry().width() - row, obj->geometry().height());
		} else if (col && obj->cursor().shape() == Qt::SizeVerCursor) {
			obj->setGeometry(obj->geometry().x(), obj->geometry().top() + col, obj->geometry().width(), obj->geometry().height() - col);
		}
	}
}
