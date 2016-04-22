#ifndef RTABLEMODEL_H
#define RTABLEMODEL_H

#include "rmapobjectmodel.h"
#include <QColor>

class RImageDescriptor;
class RFontDescriptor;

class RTableModel : public RMapObjectModel {
    Q_OBJECT
public:
    explicit RTableModel(QObject *parent = 0);
	virtual ~RTableModel();

	virtual RTableModel *clone() const;

    virtual bool isValid() const;

	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

	virtual const QString &caption() const;

    virtual void setMapObjectGeometry(const QRect &rect);

    void setNumber(int number);
    void setText(const QString &text);
    void setNumberFont(const RFontDescriptor *font);
    void setNumberFontColor(const QColor &color);
    void setTextFont(const RFontDescriptor *font);
    void setTextFontColor(const QColor &color);
    void setPictureFree(const RImageDescriptor *image);
    void setPictureBeforder(const RImageDescriptor *image);
    void setPictureOpen(const RImageDescriptor *image);
    void setPictureReserved(const RImageDescriptor *image);
    void setPictureSeveralNboards(const RImageDescriptor *image);
    void setPictureOpenWithZeroSum(const RImageDescriptor *image);

    int number() const;
    const QString &text() const;
    const QFont &numberFont() const;
    const QFont &textFont() const;
    const QColor &numberFontColor() const;
    const QColor &textFontColor() const;
    const QImage &pictureFree() const;
    const QImage &pictureBeforder() const;
    const QImage &pictureOpen() const;
    const QImage &pictureReserved() const;
    const QImage &pictureSeveralNboards() const;
    const QImage &pictureOpenWithZeroSum() const;

    const RFontDescriptor *numberFontDescriptor() const;
    const RFontDescriptor *textFontDescriptor() const;
    const RImageDescriptor *pictureFreeDescriptor() const;
    const RImageDescriptor *pictureBeforderDescriptor() const;
    const RImageDescriptor *pictureOpenDescriptor() const;
    const RImageDescriptor *pictureReservedDescriptor() const;
    const RImageDescriptor *pictureSeveralNboardsDescriptor() const;
    const RImageDescriptor *pictureOpenWithZeroSumDescriptor() const;

    void setTablePicture(int tablePictID, int type, const RImageDescriptor *image);
    const RImageDescriptor *tablePicture(int type) const;
    int tablePicturesCount() const;

	enum SECTIONS {
        EN_NUMBER = RMapObjectModel::EN_SECTIONS_COUNT,
        EN_NUMBER_FONT,
        EN_NUMBER_COLOR,
        EN_TEXT,
        EN_TEXT_FONT,
        EN_TEXT_COLOR,

        EN_PICTURE_FREE,
        EN_PICTURE_BEFORDER,
        EN_PICTURE_OPEN,
        EN_PICTURE_RESERVED,
        EN_PICTURE_SEVERAL_NBOARDS,
        EN_PICTURE_OPEN_WITH_ZERO_SUM,

        EN_SECTIONS_COUNT
	};

    static const int tablePictCount = EN_PICTURE_OPEN_WITH_ZERO_SUM - EN_PICTURE_FREE + 1;
    int tablePicturesID[tablePictCount];
    bool tablePicturesModify[tablePictCount];

protected:
     void initModel(RTableModel *m) const;

private:
    struct {
        int number;
        const RFontDescriptor *numberFont;
        QColor numberFontColor;
        QString text;
        const RFontDescriptor *textFont;
        QColor textFontColor;
        const RImageDescriptor *pictureFree;
        const RImageDescriptor *pictureBeforder;
        const RImageDescriptor *pictureOpen;
        const RImageDescriptor *pictureReserved;
        const RImageDescriptor *pictureSeveralNboards;
        const RImageDescriptor *pictureOpenWithZeroSum;
    } Data;
};

#endif // RTABLEMODEL_H
