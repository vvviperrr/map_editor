#ifndef RMAPOPTIONS_H
#define RMAPOPTIONS_H

#include <QMap>
#include <QList>
#include "rfontdescriptor.h"
#include "rimagedescriptor.h"
#include "rrefsdescriptor.h"

class RMapRegistry {
public:
    static RMapRegistry &instance();

    bool addFont(int font_id, const QString &path, int weight, int height);
    const RFontDescriptor &getFontDescriptorById(int id) const;
    const QMap<int, RFontDescriptor> &fontsMap() const;

    bool addImage(int image_id, const QString &path, int width, int height);
    const RImageDescriptor &getImageDescriptorById(int id) const;
    const QMap<int, RImageDescriptor> &imageMap() const;

    bool addRefs(int refs_id, const QString &text);
    const RRefsDescriptor &getRefsDescriptorById(int id) const;
    const QMap<int, RRefsDescriptor> &refsMap() const;

    const RFontDescriptor &emptyFont() const;
    const RImageDescriptor &emptyImage() const;
    const RRefsDescriptor &emptyRefs() const;

    void addSectionForDelete(int sect_id);
    void addTableForDelete(int table_id);

    const QList<int> sectionsForDelete() const;
    const QList<int> tablesForDelete() const;

    bool isModified() const;
    void setModified(bool b);

    void clearAll();

private:
    RMapRegistry();

    QMap<int, RFontDescriptor> m_fontsMap;
    QMap<int, RImageDescriptor> m_imageMap;
    QMap<int, RRefsDescriptor> m_RefsMap;

    RFontDescriptor m_emptyFont;
    RImageDescriptor m_emptyImage;
    RRefsDescriptor m_emptyRefs;

    QList<int> m_deletedSections;
    QList<int> m_deletedTables;

    bool m_modified;
};

#endif // RMAPOPTIONS_H
