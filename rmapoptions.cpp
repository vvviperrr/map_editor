#include "rmapoptions.h"
#include <QFontDatabase>
#include <QStringList>
#include <QDebug>

//--------------------------------------------------------------------------------------
RMapOptions &RMapOptions::instance()
{
    static RMapOptions opt;
    return opt;
}

//--------------------------------------------------------------------------------------
RMapOptions::RMapOptions()
{}

// методы для работы со шрифтами
//--------------------------------------------------------------------------------------
bool RMapOptions::addFont(int font_id, const QString &path, int weight, int height)
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
const RFontDescriptor &RMapOptions::getFontDescriptorById(int id) const
{
    QMap<int, RFontDescriptor>::const_iterator it = m_fontsMap.find(id);
    return it == m_fontsMap.end() ? m_emptyFont : *it;
}

//--------------------------------------------------------------------------------------
const QMap<int, RFontDescriptor> &RMapOptions::fontsMap() const
{ return m_fontsMap; }

//--------------------------------------------------------------------------------------
const RFontDescriptor &RMapOptions::emptyFont() const
{ return m_emptyFont; }



// методы для работы с картинками
//--------------------------------------------------------------------------------------
bool RMapOptions::addImage(int image_id, const QString &path, int width, int height)
{
    QImage image(0, 0, QImage::Format_ARGB32_Premultiplied);
    image.load(path);

    m_imageMap.insert(image_id, RImageDescriptor(image_id, width, height, path, image));
    return true;
}

//--------------------------------------------------------------------------------------
const RImageDescriptor &RMapOptions::getImageDescriptorById(int id) const
{
    QMap<int, RImageDescriptor>::const_iterator it = m_imageMap.find(id);
    return it == m_imageMap.end() ? m_emptyImage : *it;
}

//--------------------------------------------------------------------------------------
const QMap<int, RImageDescriptor> &RMapOptions::imageMap() const
{ return m_imageMap; }

//--------------------------------------------------------------------------------------
const RImageDescriptor &RMapOptions::emptyImage() const
{ return m_emptyImage; }

//--------------------------------------------------------------------------------------
void RMapOptions::clearAll()
{
    m_fontsMap.clear();
    m_imageMap.clear();
}
