#include "rmapobjectmodel.h"
#include <QRect>
#include <QDebug>

//--------------------------------------------------------------------------------------
RMapObjectModel::RMapObjectModel(QObject *parent) :
    QAbstractTableModel(parent),
    is_modify(true),
    map_object_id(0)
{
}

//--------------------------------------------------------------------------------------
RMapObjectModel::~RMapObjectModel()
{
}

// копируем нужные нам опции из текущей модели
// функция вызывается каскадно, таким образом настройки модели копируются
// сверху вниз по дереву наследования
//--------------------------------------------------------------------------------------
void RMapObjectModel::initModel(RMapObjectModel *m) const
{
    m->map_object_geometry = map_object_geometry;
    m->blockSignals(signalsBlocked());
}

//--------------------------------------------------------------------------------------
const QRect &RMapObjectModel::mapObjectGeometry() const
{ return map_object_geometry; }

//--------------------------------------------------------------------------------------
void RMapObjectModel::setMapObjectGeometry(const QRect &rect)
{
    if (map_object_geometry != rect) {
        map_object_geometry = rect;
        emit(dataChanged(createIndex(EN_XPOS, EN_VALUE), createIndex(EN_HEIGHT, EN_VALUE)));
    }
}

//--------------------------------------------------------------------------------------
bool RMapObjectModel::isModified() const
{ return is_modify; }

//--------------------------------------------------------------------------------------
void RMapObjectModel::setModified(bool modify)
{ is_modify = modify; }

//--------------------------------------------------------------------------------------
int RMapObjectModel::mapObjectID() const
{ return map_object_id; }

//--------------------------------------------------------------------------------------
void RMapObjectModel::setMapObjectID(int id)
{ map_object_id = id; }

//--------------------------------------------------------------------------------------
int RMapObjectModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return EN_COLUMNS_COUNT;
}

//--------------------------------------------------------------------------------------
bool RMapObjectModel::isValid() const
{ return true; }

//--------------------------------------------------------------------------------------
QVariant RMapObjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch (section) {
		case EN_PROPERTY:
			return QString(tr("Свойство"));
		case EN_VALUE:
			return QString(tr("Значение"));
		}
	}

	return QVariant();
}

//--------------------------------------------------------------------------------------
Qt::ItemFlags RMapObjectModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractTableModel::flags(index) | (index.column() == EN_PROPERTY ? Qt::ItemIsEnabled : Qt::ItemIsEditable);
}


//--------------------------------------------------------------------------------------
QVariant RMapObjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == EN_PROPERTY) {
            switch (index.row()) {
            case EN_XPOS:
                return QString(tr("Координата Х"));
            case EN_YPOS:
                return QString(tr("Координата У"));
            case EN_WIDTH:
                return QString(tr("Ширина"));
            case EN_HEIGHT:
                return QString(tr("Высота"));
            }
        } else if (index.column() == EN_VALUE) {
            switch (index.row()) {
            case EN_XPOS:
                return map_object_geometry.left();
            case EN_YPOS:
                return map_object_geometry.top();
            case EN_WIDTH:
                return map_object_geometry.width();
            case EN_HEIGHT:
                return map_object_geometry.height();
            }
        }
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------
bool RMapObjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        switch (index.row()) {
        case EN_XPOS:
			map_object_geometry.moveLeft(value.toInt());
            break;
        case EN_YPOS:
			map_object_geometry.moveTop(value.toInt());
            break;
        case EN_WIDTH:
            map_object_geometry.setWidth(value.toInt());
            break;
        case EN_HEIGHT:
            map_object_geometry.setHeight(value.toInt());
           break;
        default:
            return false;
        }

        emit(dataChanged(index, index));
        return true;
    }

    return false;
}
