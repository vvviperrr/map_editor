#ifndef RMAPOBJECTTREEWIDGETITEM_H
#define RMAPOBJECTTREEWIDGETITEM_H

#include <QTreeWidgetItem>

class RMapObject;

class RMapObjectTreeWidgetItem : public QTreeWidgetItem {
public:
    RMapObjectTreeWidgetItem(RMapObject *obj);
    virtual ~RMapObjectTreeWidgetItem();

    RMapObject *mapObject() const;

private:
    RMapObject *map_object;
};

#endif // RMAPOBJECTTREEWIDGETITEM_H
