#include "rimagedescriptor.h"

//--------------------------------------------------------------------------------------
RImageDescriptor::RImageDescriptor() :
    m_id(0),
    m_path(""),
    m_width(0),
    m_height(0),
    m_text(QObject::tr("Картинка не выбрана"))
{}

//--------------------------------------------------------------------------------------
RImageDescriptor::RImageDescriptor(int id, int width, int height, const QString &path, const QImage &image) :
    m_id(id),
    m_path(path),
    m_width(width),
    m_height(height),
    m_image(image),
    m_text(path.mid(path.lastIndexOf(QChar('\\')) + 1))
{}
