#ifndef RMAPOPTIONS_H
#define RMAPOPTIONS_H

#include <QMap>
#include "rfontdescriptor.h"
#include "rimagedescriptor.h"

class RMapOptions {
public:
    static RMapOptions &instance();

    bool addFont(int font_id, const QString &path, int weight, int height);
    const RFontDescriptor &getFontDescriptorById(int id) const;
    const QMap<int, RFontDescriptor> &fontsMap() const;

    bool addImage(int image_id, const QString &path, int width, int height);
    const RImageDescriptor &getImageDescriptorById(int id) const;
    const QMap<int, RImageDescriptor> &imageMap() const;

    const RFontDescriptor &emptyFont() const;
    const RImageDescriptor &emptyImage() const;

    void clearAll();

private:
    RMapOptions();

    QMap<int, RFontDescriptor> m_fontsMap;
    QMap<int, RImageDescriptor> m_imageMap;

    RFontDescriptor m_emptyFont;
    RImageDescriptor m_emptyImage;
};

#endif // RMAPOPTIONS_H
