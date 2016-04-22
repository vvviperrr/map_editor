#ifndef RSECTION_H
#define RSECTION_H

#include "rmapobject.h"
#include "rsectionmodel.h"

class QPaintEvent;
class RTable;

class RSection : public RMapObject {
	Q_OBJECT
public:
	explicit RSection(QWidget *parent = 0);
	virtual ~RSection();

	virtual RSection *clone() const;
    virtual RSectionModel *model() const;

	void setMovePolicy(RMapObjectMovePolicy *policy);

	const QPolygon &movePolygon() const;

protected:
	virtual void paintEvent(QPaintEvent *event);

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	void selectChild(const QRect &r);

private:
	QPolygon gripTriangle;
	QPoint mouseDownPoint;
	QPoint mouseMovePoint;
};

#endif // RSECTION_H
