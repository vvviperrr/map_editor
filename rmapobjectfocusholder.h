#ifndef RMAPOBJECTFOCUSHOLDER_H
#define RMAPOBJECTFOCUSHOLDER_H

#include <QObject>

class RMapObject;

class RMapObjectFocusHolder : public QObject {
    Q_OBJECT
public:
    static RMapObjectFocusHolder &instance();

    RMapObject *getFocusMapObject() const;
    void setFocusMapObject(RMapObject *obj, bool with_real_focus = false);

    void resetFocusMapObject();

signals:
    void focusMapObjectChanged(RMapObject *obj);

private:
    RMapObjectFocusHolder();
    RMapObject *focusMapObject;
};

#endif // RMAPOBJECTFOCUSHOLDER_H
