#include "rmapobjecttreewidgetitem.h"

//--------------------------------------------------------------------------------------
RMapObjectTreeWidgetItem::RMapObjectTreeWidgetItem(RMapObject *obj) :
    QTreeWidgetItem(),
    map_object(obj)
{}

//--------------------------------------------------------------------------------------
RMapObjectTreeWidgetItem::~RMapObjectTreeWidgetItem()
{
    takeChildren();
}

//--------------------------------------------------------------------------------------
RMapObject *RMapObjectTreeWidgetItem::mapObject() const
{ return map_object; }
