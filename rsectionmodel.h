#ifndef RSECTIONMODEL_H
#define RSECTIONMODEL_H

#include "rmapobjectmodel.h"
#include <QColor>

class RImageDescriptor;
class RFontDescriptor;
class RRefsDescriptor;

class RSectionModel : public RMapObjectModel {
	Q_OBJECT
public:
	explicit RSectionModel(QObject *parent = 0);
	virtual ~RSectionModel();

	virtual RSectionModel *clone() const;

    virtual bool isValid() const;

	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

	virtual const QString &caption() const;

    enum SECTIONS {
        EN_NAME = RMapObjectModel::EN_SECTIONS_COUNT,
        EN_PICTURE,
        EN_FONT,
        EN_FONT_COLOR,
        EN_CHILD_FONT,
        EN_CHILD_FONT_COLOR,
        EN_FRAME_COLOR,
        EN_ACTIVE_CHILD_PICTURE,
        EN_SECTIONS_COUNT
    };

    void setSectionName(const RRefsDescriptor *refs);
    void setSectionPicture(const RImageDescriptor *image);
    void setSectionFont(const RFontDescriptor *font);
    void setSectionFontColor(const QColor &color);
    void setChildFont(const RFontDescriptor *font);
    void setChildFontColor(const QColor &color);
    void setSectionFrameColor(const QColor &color);
    void setActiveChildPicture(const RImageDescriptor *image);

    const QString &sectionName() const;
    const QImage &sectionPicture() const;
    const QFont &sectionFont() const;
    const QColor &sectionFontColor() const;
    const QFont &childFont() const;
    const QColor &childFontColor() const;
    const QColor &sectionFrameColor() const;
    const QImage &activeChildPicture() const;

    const RRefsDescriptor *sectionNameDescriptor() const;
    const RFontDescriptor *sectionFontDescriptor() const;
    const RFontDescriptor *childFontDescriptor() const;
    const RImageDescriptor *sectionPictureDescriptor() const;
    const RImageDescriptor *activeChildPictureDescriptor() const;

protected:
    void initModel(RSectionModel *m) const;

private:
    struct SectionInfo {
        const RRefsDescriptor   *sectionName;
        const RImageDescriptor  *sectionPicture;
        const RFontDescriptor   *sectionFont;
        QColor                  sectionFontColor;
        const RFontDescriptor   *childFont;
        QColor                  childFontColor;
        QColor                  sectionFrameColor;
        const RImageDescriptor  *activeChildPicture;
    } Data;
};

#endif // RSECTIONMODEL_H
