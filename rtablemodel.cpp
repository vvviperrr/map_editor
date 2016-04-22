#include "rtablemodel.h"
#include "rmapregistry.h"
#include "rfontdescriptor.h"
#include "rimagedescriptor.h"

#include <QDebug>

//--------------------------------------------------------------------------------------
RTableModel::RTableModel(QObject *parent) :
    RMapObjectModel(parent)
{
    Data.number = 1;
    Data.text = tr("");
    Data.numberFont = &RMapRegistry::instance().emptyFont();
    Data.textFont = &RMapRegistry::instance().emptyFont();

    Data.pictureFree = &RMapRegistry::instance().emptyImage();
    Data.pictureBeforder = &RMapRegistry::instance().emptyImage();
    Data.pictureOpen = &RMapRegistry::instance().emptyImage();
    Data.pictureReserved = &RMapRegistry::instance().emptyImage();
    Data.pictureSeveralNboards = &RMapRegistry::instance().emptyImage();
    Data.pictureOpenWithZeroSum = &RMapRegistry::instance().emptyImage();

    for (int i = 0; i < tablePictCount; i++) {
        tablePicturesID[i] = 0;
        tablePicturesModify[i] = true;
    }
}

//--------------------------------------------------------------------------------------
RTableModel::~RTableModel()
{
}

//--------------------------------------------------------------------------------------
void RTableModel::setNumber(int number)
{ Data.number = number; }

//--------------------------------------------------------------------------------------
void RTableModel::setText(const QString &text)
{ Data.text = text; }

//--------------------------------------------------------------------------------------
void RTableModel::setNumberFont(const RFontDescriptor *font)
{ Data.numberFont = font; }

//--------------------------------------------------------------------------------------
void RTableModel::setNumberFontColor(const QColor &color)
{ Data.numberFontColor = color; }

//--------------------------------------------------------------------------------------
void RTableModel::setTextFont(const RFontDescriptor *font)
{ Data.textFont = font; }

//--------------------------------------------------------------------------------------
void RTableModel::setTextFontColor(const QColor &color)
{ Data.textFontColor = color; }

//--------------------------------------------------------------------------------------
void RTableModel::setPictureFree(const RImageDescriptor *image)
{
    tablePicturesModify[0] = true;
    Data.pictureFree = image;
    setMapObjectGeometry(QRect(mapObjectGeometry().topLeft(),
        image->image().isNull() ? mapObjectGeometry().size() : image->image().size()));
}

//--------------------------------------------------------------------------------------
void RTableModel::setPictureBeforder(const RImageDescriptor *image)
{
    tablePicturesModify[1] = true;
    Data.pictureBeforder = image;
}

//--------------------------------------------------------------------------------------
void RTableModel::setPictureOpen(const RImageDescriptor *image)
{
    tablePicturesModify[2] = true;
    Data.pictureOpen = image;
}

//--------------------------------------------------------------------------------------
void RTableModel::setPictureReserved(const RImageDescriptor *image)
{
    tablePicturesModify[3] = true;
    Data.pictureReserved = image;
}

//--------------------------------------------------------------------------------------
void RTableModel::setPictureSeveralNboards(const RImageDescriptor *image)
{
    tablePicturesModify[4] = true;
    Data.pictureSeveralNboards = image;
}

//--------------------------------------------------------------------------------------
void RTableModel::setPictureOpenWithZeroSum(const RImageDescriptor *image)
{
    tablePicturesModify[5] = true;
    Data.pictureOpenWithZeroSum = image;
}


//--------------------------------------------------------------------------------------
void RTableModel::setTablePicture(int tablePictID, int type, const RImageDescriptor *image)
{
    switch (type) {
    case 0:
        setPictureFree(image);
        break;
    case 1:
        setPictureBeforder(image);
        break;
    case 2:
        setPictureOpen(image);
        break;
    case 3:
        setPictureReserved(image);
        break;
    case 4:
        setPictureSeveralNboards(image);
        break;
    case 5:
        setPictureOpenWithZeroSum(image);
        break;
    default:
        return;
    }

    tablePicturesID[type] = tablePictID;
    tablePicturesModify[type] = false;
}

//--------------------------------------------------------------------------------------
const RImageDescriptor *RTableModel::tablePicture(int type) const
{
    switch (type) {
    case 0:
        return Data.pictureFree;
    case 1:
        return Data.pictureBeforder;
    case 2:
        return Data.pictureOpen;
    case 3:
        return Data.pictureReserved;
    case 4:
        return Data.pictureSeveralNboards;
    case 5:
        return Data.pictureOpenWithZeroSum;
    default:
        return &RMapRegistry::instance().emptyImage();
    }
}

//--------------------------------------------------------------------------------------
int RTableModel::tablePicturesCount() const
{ return tablePictCount; }


//--------------------------------------------------------------------------------------
int RTableModel::number() const
{ return Data.number; }

//--------------------------------------------------------------------------------------
const QString &RTableModel::text() const
{ return Data.text; }

//--------------------------------------------------------------------------------------
const QFont &RTableModel::numberFont() const
{ return Data.numberFont->font(); }

//--------------------------------------------------------------------------------------
const QFont &RTableModel::textFont() const
{ return Data.textFont->font(); }

//--------------------------------------------------------------------------------------
const QColor &RTableModel::numberFontColor() const
{ return Data.numberFontColor; }

//--------------------------------------------------------------------------------------
const QColor &RTableModel::textFontColor() const
{ return Data.textFontColor; }

//--------------------------------------------------------------------------------------
const QImage &RTableModel::pictureFree() const
{ return Data.pictureFree->image(); }

//--------------------------------------------------------------------------------------
const QImage &RTableModel::pictureBeforder() const
{ return Data.pictureBeforder->image(); }

//--------------------------------------------------------------------------------------
const QImage &RTableModel::pictureOpen() const
{ return Data.pictureOpen->image(); }

//--------------------------------------------------------------------------------------
const QImage &RTableModel::pictureReserved() const
{ return Data.pictureReserved->image(); }

//--------------------------------------------------------------------------------------
const QImage &RTableModel::pictureSeveralNboards() const
{ return Data.pictureSeveralNboards->image(); }

//--------------------------------------------------------------------------------------
const QImage &RTableModel::pictureOpenWithZeroSum() const
{ return Data.pictureOpenWithZeroSum->image(); }



//--------------------------------------------------------------------------------------
const RFontDescriptor *RTableModel::numberFontDescriptor() const
{ return Data.numberFont; }

//--------------------------------------------------------------------------------------
const RFontDescriptor *RTableModel::textFontDescriptor() const
{ return Data.textFont; }

//--------------------------------------------------------------------------------------
const RImageDescriptor *RTableModel::pictureFreeDescriptor() const
{ return Data.pictureFree; }

//--------------------------------------------------------------------------------------
const RImageDescriptor *RTableModel::pictureBeforderDescriptor() const
{ return Data.pictureBeforder; }

//--------------------------------------------------------------------------------------
const RImageDescriptor *RTableModel::pictureOpenDescriptor() const
{ return Data.pictureOpen; }

//--------------------------------------------------------------------------------------
const RImageDescriptor *RTableModel::pictureReservedDescriptor() const
{ return Data.pictureReserved; }

//--------------------------------------------------------------------------------------
const RImageDescriptor *RTableModel::pictureSeveralNboardsDescriptor() const
{ return Data.pictureSeveralNboards; }

//--------------------------------------------------------------------------------------
const RImageDescriptor *RTableModel::pictureOpenWithZeroSumDescriptor() const
{ return Data.pictureOpenWithZeroSum; }



//--------------------------------------------------------------------------------------
RTableModel *RTableModel::clone() const
{
	RTableModel *m = new RTableModel();
    initModel(m);
	return m;
}

//--------------------------------------------------------------------------------------
void RTableModel::initModel(RTableModel *m) const
{
    m->Data = Data;
    RMapObjectModel::initModel(m);
}

//--------------------------------------------------------------------------------------
const QString &RTableModel::caption() const
{
    static QString str;
    str = QString("%1%2%3").arg(Data.text).arg(Data.text.isEmpty() ? tr("") : tr(" ")).arg(QString().number(Data.number));
    return str;
}

//--------------------------------------------------------------------------------------
bool RTableModel::isValid() const
{ return Data.number != 0 && Data.numberFont->id() != 0 && Data.pictureFree->id() != 0; }

//--------------------------------------------------------------------------------------
void RTableModel::setMapObjectGeometry(const QRect &rect)
{
    RMapObjectModel::setMapObjectGeometry(
        Data.pictureFree->image().isNull() ? rect : QRect(rect.topLeft(), Data.pictureFree->image().size()));
}

//--------------------------------------------------------------------------------------
int RTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return EN_SECTIONS_COUNT;
}

//--------------------------------------------------------------------------------------
QVariant RTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
		switch (index.column()) {
		case EN_PROPERTY:
			switch (index.row()) {
			case EN_NUMBER:
				return QString(tr("Номер"));
            case EN_NUMBER_FONT:
                return QString(tr("Шрифт номера"));
            case EN_NUMBER_COLOR:
                return QString(tr("Цвет шрифта номера"));
            case EN_TEXT:
                return QString(tr("Текст"));
            case EN_TEXT_FONT:
                return QString(tr("Шрифт текста"));
            case EN_TEXT_COLOR:
                return QString(tr("Цвет шрифта текста"));
            case EN_PICTURE_FREE:
                return QString(tr("Свободный столик"));
            case EN_PICTURE_BEFORDER:
                return QString(tr("Столик на предчеке"));
            case EN_PICTURE_OPEN:
                return QString(tr("Столик открытый"));
            case EN_PICTURE_RESERVED:
                return QString(tr("Столик зарезервирован"));
            case EN_PICTURE_SEVERAL_NBOARDS:
                return QString(tr("На столике несколько посадок"));
            case EN_PICTURE_OPEN_WITH_ZERO_SUM:
                return QString(tr("Открытый столик с нулевой суммой заказа"));
			}
			break;

		case EN_VALUE:
            switch (index.row()) {
            case EN_NUMBER:
                return Data.number;
            case EN_NUMBER_FONT:
                return qVariantFromValue(Data.numberFont);
            case EN_NUMBER_COLOR:
                return Data.numberFontColor;
            case EN_TEXT:
                return Data.text;
            case EN_TEXT_FONT:
                return qVariantFromValue(Data.textFont);
            case EN_TEXT_COLOR:
                return Data.textFontColor;
            case EN_PICTURE_FREE:
                return qVariantFromValue(Data.pictureFree);
            case EN_PICTURE_BEFORDER:
                return qVariantFromValue(Data.pictureBeforder);
            case EN_PICTURE_OPEN:
                return qVariantFromValue(Data.pictureOpen);
            case EN_PICTURE_RESERVED:
                return qVariantFromValue(Data.pictureReserved);
            case EN_PICTURE_SEVERAL_NBOARDS:
                return qVariantFromValue(Data.pictureSeveralNboards);
            case EN_PICTURE_OPEN_WITH_ZERO_SUM:
                return qVariantFromValue(Data.pictureOpenWithZeroSum);
            }
		}

    } else if (role == Qt::UserRole) {
        switch (index.row()) {
        case EN_NUMBER_COLOR:
        case EN_TEXT_COLOR:
             return ET_COLOR_SELECTOR;
        case EN_TEXT_FONT:
        case EN_NUMBER_FONT:
            return ET_FONT_SELECTOR;
        case EN_PICTURE_FREE:
        case EN_PICTURE_BEFORDER:
        case EN_PICTURE_OPEN:
        case EN_PICTURE_RESERVED:
        case EN_PICTURE_SEVERAL_NBOARDS:
        case EN_PICTURE_OPEN_WITH_ZERO_SUM:
            return ET_PICTURE_SELECTOR;
        }
    } else if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	}

    return RMapObjectModel::data(index, role);
}

//--------------------------------------------------------------------------------------
bool RTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::EditRole) {
        switch (index.row()) {
        case EN_NUMBER:
            setNumber(value.toInt());
            break;
        case EN_NUMBER_FONT:
            setNumberFont(value.value<const RFontDescriptor *>());
            break;
        case EN_NUMBER_COLOR:
            setNumberFontColor(value.value<QColor>());
            break;
        case EN_TEXT:
            setText(value.toString());
            break;
        case EN_TEXT_FONT:
            setTextFont(value.value<const RFontDescriptor *>());
            break;
        case EN_TEXT_COLOR:
            setTextFontColor(value.value<QColor>());
            break;
        case EN_PICTURE_FREE:
            setPictureFree(value.value<const RImageDescriptor *>());
            break;
        case EN_PICTURE_BEFORDER:
            setPictureBeforder(value.value<const RImageDescriptor *>());
            break;
        case EN_PICTURE_OPEN:
            setPictureOpen(value.value<const RImageDescriptor *>());
            break;
        case EN_PICTURE_RESERVED:
            setPictureReserved(value.value<const RImageDescriptor *>());
            break;
        case EN_PICTURE_SEVERAL_NBOARDS:
            setPictureSeveralNboards(value.value<const RImageDescriptor *>());
            break;
        case EN_PICTURE_OPEN_WITH_ZERO_SUM:
            setPictureOpenWithZeroSum(value.value<const RImageDescriptor *>());
            break;
        default:
            return RMapObjectModel::setData(index, value, role);
        }
		emit(dataChanged(index, index));
		return true;
	}

    return false;
}
