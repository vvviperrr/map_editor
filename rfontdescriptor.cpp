#include "rfontdescriptor.h"

//--------------------------------------------------------------------------------------
RFontDescriptor::RFontDescriptor() :
    m_id(0),
    m_path(""),
    m_weight(0),
    m_height(0),
    m_text(QObject::tr("Шрифт не выбран"))
{}

//--------------------------------------------------------------------------------------
RFontDescriptor::RFontDescriptor(int id, const QString &path, int weight, int height, const QFont &font) :
    m_id(id),
    m_path(path),
    m_weight(weight),
    m_height(height),
    m_font(font),
    m_text(QString(QObject::tr("%1 %2")).arg(font.family()).arg(font.pixelSize()))
{}
