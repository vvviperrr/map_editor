#ifndef RTABLE_H
#define RTABLE_H

#include "rmapobject.h"
#include "rtablemodel.h"

class QPaintEvent;

class RTable : public RMapObject {
    Q_OBJECT
public:
    explicit RTable(QWidget *parent = 0);
	virtual ~RTable();

	virtual RTable *clone() const;
    virtual RTableModel *model() const;

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *event);
};

#endif // RTABLE_H
