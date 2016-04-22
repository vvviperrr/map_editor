#ifndef RCOMPOSITEMAPOBJECT_H
#define RCOMPOSITEMAPOBJECT_H

#include "rmapobject.h"
#include <QList>

class QPaintEvent;

class RCompositeMapObject : public RMapObject {
    Q_OBJECT
public:
    static void createInstance(RMapObject *parent,
                                         RMapObjectMovePolicy *movePolicy,
                                         const QList<RMapObject *> &list);

    static void deleteInstance();

    static RCompositeMapObject *instance();

	void addMapObject(RMapObject *obj);
	void removeMapObject(RMapObject *obj);
    void removeAllMapObject();
    bool isEmpty() const;

    const QList<RMapObject *> mapObjectList() const;
	void setMapObjectList(const QList<RMapObject *> &list);

	virtual RCompositeMapObject *clone() const;

protected:
    explicit RCompositeMapObject(QWidget *parent = 0);
    virtual ~RCompositeMapObject();

protected:
	virtual void paintEvent(QPaintEvent *event);

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	virtual void resizeEvent(QResizeEvent *event);
	virtual void moveEvent(QMoveEvent *event);

    virtual void focusSetEvent();
    virtual void focusLostEvent();

private slots:
    void onRenumberTables();

private:
    QList<RMapObject *> map_object_list;
	bool mouse_pressed;
    mutable bool self_deleted;

    static RCompositeMapObject *m_instance;
};

#endif // RCOMPOSITEMAPOBJECT_H
