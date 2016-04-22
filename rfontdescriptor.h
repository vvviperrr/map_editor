#ifndef RFONTDESCRIPTOR_H
#define RFONTDESCRIPTOR_H

#include <QFont>
#include <QMetaType>

class RFontDescriptor {
public:
    RFontDescriptor();
    RFontDescriptor(int id, const QString &path, int weight, int height, const QFont &font);

    int id() const;
    int height() const;
    int weight() const;
    const QString &path() const;
    const QFont &font() const;

    const QString &text() const;

private:
    int m_id;
    QString m_path;
    int m_weight;
    int m_height;
    QFont m_font;
    QString m_text;
};

//--------------------------------------------------------------------------------------
inline int RFontDescriptor::id() const
{ return m_id; }

//--------------------------------------------------------------------------------------
inline int RFontDescriptor::height() const
{ return m_height; }

//--------------------------------------------------------------------------------------
inline int RFontDescriptor::weight() const
{ return m_weight; }

//--------------------------------------------------------------------------------------
inline const QString &RFontDescriptor::path() const
{ return m_path; }

//--------------------------------------------------------------------------------------
inline const QFont &RFontDescriptor::font() const
{ return m_font; }

//--------------------------------------------------------------------------------------
inline const QString &RFontDescriptor::text() const
{ return m_text; }

Q_DECLARE_METATYPE(RFontDescriptor)
Q_DECLARE_METATYPE(const RFontDescriptor *)

#endif // RFONTDESCRIPTOR_H
