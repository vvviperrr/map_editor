#include "rmapobjectfocusholder.h"
#include "rmapobject.h"
#include <QDebug>

//--------------------------------------------------------------------------------------
RMapObjectFocusHolder &RMapObjectFocusHolder::instance()
{
    static RMapObjectFocusHolder holder;
    return holder;
}

//--------------------------------------------------------------------------------------
RMapObjectFocusHolder::RMapObjectFocusHolder() :
    QObject(0),
    focusMapObject(0)
{}

//--------------------------------------------------------------------------------------
RMapObject *RMapObjectFocusHolder::getFocusMapObject() const
{ return focusMapObject; }

//--------------------------------------------------------------------------------------
void RMapObjectFocusHolder::resetFocusMapObject()
{ focusMapObject = 0; }

//--------------------------------------------------------------------------------------
void RMapObjectFocusHolder::setFocusMapObject(RMapObject *obj, bool with_real_focus /* = false */)
{
    if (focusMapObject == obj)
        return;

    if (focusMapObject) {
        focusMapObject->update();
        focusMapObject->focusLostEvent();
    }

    focusMapObject = obj;

    if (focusMapObject) {
        focusMapObject->raise();
        focusMapObject->update();
        focusMapObject->focusSetEvent();

        if (with_real_focus) {
            focusMapObject->setFocus();
        }
    }

    emit(focusMapObjectChanged(focusMapObject));
}
