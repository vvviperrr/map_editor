#ifndef RMAPOBJECTRESIZEPOLICY_H
#define RMAPOBJECTRESIZEPOLICY_H

#include "rmapobjectbehaviourpolicy.h"

#include <QPolygon>
#include <QPoint>

class RMapObject;
class QMouseEvent;

//--------------------------------------------------------------------------------------
class RMapObjectResizePolicy : public RMapObjectBehaviourPolicy {
public:
	RMapObjectResizePolicy(RMapObject *obj = 0);

	virtual bool mouseMoveEvent(QMouseEvent *event);

	virtual RMapObjectResizePolicy *clone() const = 0;

protected:
    bool invertResize;
};

//--------------------------------------------------------------------------------------
class RMapObjectNoResizePolicy : public RMapObjectResizePolicy {
public:
	RMapObjectNoResizePolicy();
	virtual RMapObjectNoResizePolicy *clone() const;
};

//--------------------------------------------------------------------------------------
class RMapObjectSimpleResizePolicy : public RMapObjectResizePolicy {
public:
	RMapObjectSimpleResizePolicy(RMapObject *obj = 0);

	virtual bool mouseMoveEvent(QMouseEvent *event);

	virtual RMapObjectSimpleResizePolicy *clone() const;
};

//--------------------------------------------------------------------------------------
class RMapObjectGridResizePolicy : public RMapObjectResizePolicy {
public:
	RMapObjectGridResizePolicy(size_t size, RMapObject *obj = 0);

	virtual bool mouseMoveEvent(QMouseEvent *event);

	virtual RMapObjectGridResizePolicy *clone() const;

protected:
	void doResize(int w, int h);

private:
	size_t gridSize;
};

#endif // RMAPOBJECTRESIZEPOLICY_H
