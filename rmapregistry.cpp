#include "rmapregistry.h"
#include <QFontDatabase>
#include <QStringList>
#include <QDebug>

//--------------------------------------------------------------------------------------
RMapRegistry &RMapRegistry::instance()
{
    static RMapRegistry opt;
    return opt;
}

//--------------------------------------------------------------------------------------
RMapRegistry::RMapRegistry() :
    m_modified(false)
{}

// методы для работы со шрифтами
//--------------------------------------------------------------------------------------
bool RMapRegistry::addFont(int font_id, const QString &path, int weight, int height)
{
    int id = QFontDatabase::addApplicationFont(path);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);

    font.setPixelSize(height);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setStyleStrategy(QFont::PreferAntialias);

    m_fontsMap.insert(font_id, RFontDescriptor(font_id, path, weight, height, font));
    return true;
}

//--------------------------------------------------------------------------------------
const RFontDescriptor &RMapRegistry::getFontDescriptorById(int id) const
{
    QMap<int, RFontDescriptor>::const_iterator it = m_fontsMap.find(id);
    return it == m_fontsMap.end() ? m_emptyFont : *it;
}

//--------------------------------------------------------------------------------------
const QMap<int, RFontDescriptor> &RMapRegistry::fontsMap() const
{ return m_fontsMap; }

//--------------------------------------------------------------------------------------
const RFontDescriptor &RMapRegistry::emptyFont() const
{ return m_emptyFont; }



// методы для работы с картинками
//--------------------------------------------------------------------------------------
bool RMapRegistry::addImage(int image_id, const QString &path, int width, int height)
{
    QImage image(0, 0, QImage::Format_ARGB32_Premultiplied);
    image.load(path);

    m_imageMap.insert(image_id, RImageDescriptor(image_id, width, height, path, image));
    return true;
}

//--------------------------------------------------------------------------------------
const RImageDescriptor &RMapRegistry::getImageDescriptorById(int id) const
{
    QMap<int, RImageDescriptor>::const_iterator it = m_imageMap.find(id);
    return it == m_imageMap.end() ? m_emptyImage : *it;
}

//--------------------------------------------------------------------------------------
const QMap<int, RImageDescriptor> &RMapRegistry::imageMap() const
{ return m_imageMap; }

//--------------------------------------------------------------------------------------
const RImageDescriptor &RMapRegistry::emptyImage() const
{ return m_emptyImage; }




// методы для работы со справочниками
//--------------------------------------------------------------------------------------
bool RMapRegistry::addRefs(int refs_id, const QString &text)
{
    m_RefsMap.insert(refs_id, RRefsDescriptor(refs_id, text));
    return true;
}

//--------------------------------------------------------------------------------------
const RRefsDescriptor &RMapRegistry::getRefsDescriptorById(int id) const
{
    QMap<int, RRefsDescriptor>::const_iterator it = m_RefsMap.find(id);
    return it == m_RefsMap.end() ? m_emptyRefs : *it;
}

//--------------------------------------------------------------------------------------
const QMap<int, RRefsDescriptor> &RMapRegistry::refsMap() const
{ return m_RefsMap; }

//--------------------------------------------------------------------------------------
const RRefsDescriptor &RMapRegistry::emptyRefs() const
{ return m_emptyRefs; }



//--------------------------------------------------------------------------------------
bool RMapRegistry::isModified() const
{ return m_modified; }

//--------------------------------------------------------------------------------------
void RMapRegistry::setModified(bool b)
{ m_modified = b; }





//--------------------------------------------------------------------------------------
void RMapRegistry::addSectionForDelete(int sect_id)
{
    if (sect_id) {
        m_modified = true;
        m_deletedSections.push_back(sect_id);
    }
}

//--------------------------------------------------------------------------------------
void RMapRegistry::addTableForDelete(int table_id)
{
    if (table_id) {
        m_modified = true;
        m_deletedTables.push_back(table_id);
    }
}

//--------------------------------------------------------------------------------------
const QList<int> RMapRegistry::sectionsForDelete() const
{ return m_deletedSections; }

//--------------------------------------------------------------------------------------
const QList<int> RMapRegistry::tablesForDelete() const
{ return m_deletedTables; }

//--------------------------------------------------------------------------------------
void RMapRegistry::clearAll()
{
    m_fontsMap.clear();
    m_imageMap.clear();
    m_RefsMap.clear();

    m_deletedSections.clear();
    m_deletedTables.clear();

    m_modified = false;
}
