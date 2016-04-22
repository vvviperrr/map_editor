#ifndef RMAPOBJECT_H
#define RMAPOBJECT_H

#include <QWidget>

class QModelIndex;
class QMouseEvent;

class RMapObjectModel;
class RMapObjectMovePolicy;
class RMapObjectResizePolicy;
class RMapObjectTreeWidgetItem;
class RMapObjectFocusHolder;

//--------------------------------------------------------------------------------------
class RMapObject : public QWidget {
    Q_OBJECT
public:
    explicit RMapObject(QWidget *parent);
	virtual ~RMapObject();

	virtual RMapObject *clone() const = 0;

	virtual RMapObjectModel *model() const;
	virtual void setModel(RMapObjectModel *m);

	RMapObjectMovePolicy *movePolicy() const;
	void setMovePolicy(RMapObjectMovePolicy *policy);

	RMapObjectResizePolicy *resizePolicy() const;
	void setResizePolicy(RMapObjectResizePolicy *policy);

    RMapObjectTreeWidgetItem *treeWidgetItem() const;

	bool hasFocusMapObject() const;
    void setFocusMapObject();

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

    virtual void resizeEvent(QResizeEvent *event);
    virtual void moveEvent(QMoveEvent *event);

	virtual void focusSetEvent();
	virtual void focusLostEvent();

	virtual void updateTreeWidgetItem();

protected slots:
	void modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
	RMapObjectModel *mapObjectModel;
	RMapObjectMovePolicy *mapObjectMovePolicy;
	RMapObjectResizePolicy *mapObjectResizePolicy;
    RMapObjectTreeWidgetItem *mapObjectTreeWidgetItem;

    friend class RMapObjectFocusHolder;
};

#endif // RMAPOBJECT_H
