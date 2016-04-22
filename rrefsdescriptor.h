#ifndef RREFSDESCRIPTOR_H
#define RREFSDESCRIPTOR_H

#include <QString>
#include <QMetaType>

class RRefsDescriptor {
public:
    RRefsDescriptor();
    RRefsDescriptor(int refs_id, const QString &str);

    int id() const;
    const QString &text() const;

private:
    int m_id;
    QString m_text;
};

//--------------------------------------------------------------------------------------
inline int RRefsDescriptor::id() const
{ return m_id; }

//--------------------------------------------------------------------------------------
inline const QString &RRefsDescriptor::text() const
{ return m_text; }

Q_DECLARE_METATYPE(RRefsDescriptor)
Q_DECLARE_METATYPE(const RRefsDescriptor *)

#endif // RREFSDESCRIPTOR_H
