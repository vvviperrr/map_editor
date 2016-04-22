#include "rrefsdescriptor.h"

//--------------------------------------------------------------------------------------
RRefsDescriptor::RRefsDescriptor() :
    m_id(0),
    m_text(QObject::tr("Зал не выбран"))
{}


//--------------------------------------------------------------------------------------
RRefsDescriptor::RRefsDescriptor(int refs_id, const QString &str) :
    m_id(refs_id),
    m_text(str)
{}
