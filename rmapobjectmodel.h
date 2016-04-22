#ifndef RMAPOBJECTMODEL_H
#define RMAPOBJECTMODEL_H

#include <QAbstractTableModel>
#include <QRect>

class RMapObjectModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit RMapObjectModel(QObject *parent = 0);
	virtual ~RMapObjectModel();

    virtual RMapObjectModel *clone() const = 0;

	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

	virtual const QString &caption() const = 0;

    virtual bool isValid() const;

    const QRect &mapObjectGeometry() const;
    virtual void setMapObjectGeometry(const QRect &rect);

    bool isModified() const;
    void setModified(bool modify);

    int mapObjectID() const;
    void setMapObjectID(int id);

    enum COLUMNS {
        EN_PROPERTY = 0,
        EN_VALUE,
        EN_COLUMNS_COUNT
    };

    enum SECTIONS {
        EN_XPOS = 0,
        EN_YPOS,
        EN_WIDTH,
        EN_HEIGHT,
        EN_SECTIONS_COUNT
    };

    enum EDIT_TYPE {
        ET_FONT_SELECTOR = 1,
        ET_COLOR_SELECTOR,
        ET_PICTURE_SELECTOR,
        ET_REFS_SELECTOR
    };

protected:
    void initModel(RMapObjectModel *m) const;

private:
    QRect map_object_geometry;
    bool is_modify;
    int map_object_id;
};

#endif // RMAPOBJECTMODEL_H
