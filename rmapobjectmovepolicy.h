#ifndef RMAPOBJECTMOVEPOLICY_H
#define RMAPOBJECTMOVEPOLICY_H

#include "rmapobjectbehaviourpolicy.h"

#include <QPolygon>
#include <QPoint>

class QMouseEvent;
class QKeyEvent;

//--------------------------------------------------------------------------------------
class RMapObjectMovePolicy : public RMapObjectBehaviourPolicy {
public:
	RMapObjectMovePolicy(RMapObject *obj = 0);

	virtual bool mousePressEvent(QMouseEvent *event);
	virtual bool mouseReleaseEvent(QMouseEvent *event);
	virtual bool mouseMoveEvent(QMouseEvent *event);
	virtual bool keyPressEvent(QKeyEvent *event);

	virtual RMapObjectMovePolicy *clone() const = 0;

	const QPolygon &movePolygon() const;
	void setMovePolygon(const QPolygon &polygon);

	bool isMoveClip() const;
	void setMoveClip(bool clip = true);

protected:
	const QPoint &mouseDownPoint() const;
	void adjustClippingPos(int &row, int &col);

private:
	QPoint mouse_down_point;
	QPolygon move_polygon;
	bool is_clipped;
};

//--------------------------------------------------------------------------------------
class RMapObjectNoMovePolicy : public RMapObjectMovePolicy {
public:
	RMapObjectNoMovePolicy();
	virtual RMapObjectNoMovePolicy *clone() const;
};

//--------------------------------------------------------------------------------------
class RMapObjectSimpleMovePolicy : public RMapObjectMovePolicy {
public:
	RMapObjectSimpleMovePolicy(RMapObject *obj = 0);

	virtual bool mouseMoveEvent(QMouseEvent *event);
	virtual bool keyPressEvent(QKeyEvent *event);

	virtual RMapObjectSimpleMovePolicy *clone() const;
};

//--------------------------------------------------------------------------------------
class RMapObjectGridMovePolicy : public RMapObjectMovePolicy {
public:
	RMapObjectGridMovePolicy(size_t size, RMapObject *obj = 0);

	virtual bool mouseMoveEvent(QMouseEvent *event);
	virtual bool keyPressEvent(QKeyEvent *event);

	virtual RMapObjectGridMovePolicy *clone() const;

protected:
	void doMove(const QPoint &p);

private:
	size_t gridSize;
};

#endif // RMAPOBJECTMOVEPOLICY_H
