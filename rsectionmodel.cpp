#include "rsectionmodel.h"
#include "rmapregistry.h"
#include <QDebug>

#include "rfontdescriptor.h"
#include "rimagedescriptor.h"

//--------------------------------------------------------------------------------------
RSectionModel::RSectionModel(QObject *parent) :
	RMapObjectModel(parent)
{
    Data.sectionName = &RMapRegistry::instance().emptyRefs();
    Data.sectionPicture = &RMapRegistry::instance().emptyImage();
    Data.activeChildPicture = &RMapRegistry::instance().emptyImage();
    Data.sectionFont = &RMapRegistry::instance().emptyFont();
    Data.childFont = &RMapRegistry::instance().emptyFont();
}

//--------------------------------------------------------------------------------------
RSectionModel::~RSectionModel()
{}

//--------------------------------------------------------------------------------------
void RSectionModel::setSectionName(const RRefsDescriptor *refs)
{ Data.sectionName = refs; }

//--------------------------------------------------------------------------------------
void RSectionModel::setSectionPicture(const RImageDescriptor *image)
{ Data.sectionPicture = image; }

//--------------------------------------------------------------------------------------
void RSectionModel::setSectionFont(const RFontDescriptor *font)
{ Data.sectionFont = font; }

//--------------------------------------------------------------------------------------
void RSectionModel::setSectionFontColor(const QColor &color)
{ Data.sectionFontColor = color; }

//--------------------------------------------------------------------------------------
void RSectionModel::setChildFont(const RFontDescriptor *font)
{ Data.childFont = font; }

//--------------------------------------------------------------------------------------
void RSectionModel::setChildFontColor(const QColor &color)
{ Data.childFontColor = color; }

//--------------------------------------------------------------------------------------
void RSectionModel::setSectionFrameColor(const QColor &color)
{ Data.sectionFrameColor = color; }

//--------------------------------------------------------------------------------------
void RSectionModel::setActiveChildPicture(const RImageDescriptor *image)
{ Data.activeChildPicture = image; }



//--------------------------------------------------------------------------------------
const QString &RSectionModel::sectionName() const
{ return Data.sectionName->text(); }

//--------------------------------------------------------------------------------------
const QImage &RSectionModel::sectionPicture() const
{ return Data.sectionPicture->image(); }

//--------------------------------------------------------------------------------------
const QFont &RSectionModel::sectionFont() const
{ return Data.sectionFont->font(); }

//--------------------------------------------------------------------------------------
const QColor &RSectionModel::sectionFontColor() const
{ return Data.sectionFontColor; }

//--------------------------------------------------------------------------------------
const QFont &RSectionModel::childFont() const
{ return Data.childFont->font(); }

//--------------------------------------------------------------------------------------
const QColor &RSectionModel::childFontColor() const
{ return Data.childFontColor; }

//--------------------------------------------------------------------------------------
const QColor &RSectionModel::sectionFrameColor() const
{ return Data.sectionFrameColor; }

//--------------------------------------------------------------------------------------
const QImage &RSectionModel::activeChildPicture() const
{ return Data.activeChildPicture->image(); }



//--------------------------------------------------------------------------------------
const RRefsDescriptor *RSectionModel::sectionNameDescriptor() const
{ return Data.sectionName; }

//--------------------------------------------------------------------------------------
const RFontDescriptor *RSectionModel::sectionFontDescriptor() const
{ return Data.sectionFont; }

//--------------------------------------------------------------------------------------
const RFontDescriptor *RSectionModel::childFontDescriptor() const
{ return Data.childFont; }

//--------------------------------------------------------------------------------------
const RImageDescriptor *RSectionModel::sectionPictureDescriptor() const
{ return Data.sectionPicture; }

//--------------------------------------------------------------------------------------
const RImageDescriptor *RSectionModel::activeChildPictureDescriptor() const
{ return Data.activeChildPicture; }

//--------------------------------------------------------------------------------------
RSectionModel *RSectionModel::clone() const
{
	RSectionModel *m = new RSectionModel();
    initModel(m);
	return m;
}

//--------------------------------------------------------------------------------------
void RSectionModel::initModel(RSectionModel *m) const
{
    m->Data = Data;
    RMapObjectModel::initModel(m);
}

//--------------------------------------------------------------------------------------
const QString &RSectionModel::caption() const
{ return Data.sectionName->text(); }

//--------------------------------------------------------------------------------------
bool RSectionModel::isValid() const
{
    return      Data.sectionName->id() != 0
            &&  Data.sectionPicture->id() != 0
            &&  Data.sectionFont->id() != 0
            &&  Data.childFont->id() != 0
            &&  Data.activeChildPicture->id() != 0;
}

//--------------------------------------------------------------------------------------
int RSectionModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return EN_SECTIONS_COUNT;
}

//--------------------------------------------------------------------------------------
QVariant RSectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		switch (index.column()) {
		case EN_PROPERTY:
			switch (index.row()) {
			case EN_NAME:
				return QString(tr("Название"));
            case EN_PICTURE:
                return QString(tr("Фоновая картинка"));
            case EN_FONT:
                return QString(tr("Шрифт"));
            case EN_FONT_COLOR:
                return QString(tr("Цвет шрифта"));
            case EN_CHILD_FONT:
                return QString(tr("Шрифт столов"));
            case EN_CHILD_FONT_COLOR:
                return QString(tr("Цвет шрифта столов"));
            case EN_FRAME_COLOR:
                return QString(tr("Цвет рамки"));
            case EN_ACTIVE_CHILD_PICTURE:
                return QString(tr("Картинка активного стола"));
			}
			break;

		case EN_VALUE:
            switch (index.row()) {
            case EN_NAME:
                return qVariantFromValue(Data.sectionName);
            case EN_PICTURE:
                return qVariantFromValue(Data.sectionPicture);
            case EN_FONT:
                return qVariantFromValue(Data.sectionFont);
            case EN_FONT_COLOR:
                return Data.sectionFontColor;
            case EN_CHILD_FONT:
                return qVariantFromValue(Data.childFont);
            case EN_CHILD_FONT_COLOR:
                return Data.childFontColor;
            case EN_FRAME_COLOR:
                return Data.sectionFrameColor;
            case EN_ACTIVE_CHILD_PICTURE:
                return qVariantFromValue(Data.activeChildPicture);
            }
            break;
		}

    } else if (role == Qt::UserRole) {
        switch (index.row()) {
        case EN_NAME:
            return ET_REFS_SELECTOR;
        case EN_FONT_COLOR:
        case EN_FRAME_COLOR:
        case EN_CHILD_FONT_COLOR:
            return ET_COLOR_SELECTOR;
        case EN_PICTURE:
        case EN_ACTIVE_CHILD_PICTURE:
            return ET_PICTURE_SELECTOR;
        case EN_FONT:
        case EN_CHILD_FONT:
            return ET_FONT_SELECTOR;
        }
    } else if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
    }

    return RMapObjectModel::data(index, role);
}

//--------------------------------------------------------------------------------------
bool RSectionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::EditRole) {
        switch (index.row()) {
        case EN_NAME:
            Data.sectionName = value.value<const RRefsDescriptor *>();
            break;
        case EN_PICTURE:
            Data.sectionPicture = value.value<const RImageDescriptor *>();
            break;
        case EN_FONT:
            Data.sectionFont = value.value<const RFontDescriptor *>();
            break;
        case EN_FONT_COLOR:
            Data.sectionFontColor = value.value<QColor>();
            break;
        case EN_CHILD_FONT:
            Data.childFont = value.value<const RFontDescriptor *>();
            break;
        case EN_CHILD_FONT_COLOR:
            Data.childFontColor = value.value<QColor>();
            break;
        case EN_FRAME_COLOR:
            Data.sectionFrameColor = value.value<QColor>();
            break;
        case EN_ACTIVE_CHILD_PICTURE:
            Data.activeChildPicture = value.value<const RImageDescriptor *>();
            break;
        default:
            return RMapObjectModel::setData(index, value, role);
        }

        emit(dataChanged(index, index));
        return true;
	}

    return false;
}
