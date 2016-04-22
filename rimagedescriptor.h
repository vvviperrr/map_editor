#ifndef RIMAGEDESCRIPTOR_H
#define RIMAGEDESCRIPTOR_H

#include <QImage>
#include <QMetaType>

class RImageDescriptor {
public:
    RImageDescriptor();
    RImageDescriptor(int id, int width, int height, const QString &path, const QImage &image);

    int id() const;
    int width() const;
    int height() const;
    const QString &path() const;
    const QImage &image() const;

    const QString &text() const;

private:
    int m_id;
    QString m_path;
    int m_width;
    int m_height;
    QImage m_image;
    QString m_text;
};

//--------------------------------------------------------------------------------------
inline int RImageDescriptor::id() const
{ return m_id; }

//--------------------------------------------------------------------------------------
inline int RImageDescriptor::width() const
{ return m_width; }

//--------------------------------------------------------------------------------------
inline int RImageDescriptor::height() const
{ return m_height; }

//--------------------------------------------------------------------------------------
inline const QString &RImageDescriptor::path() const
{ return m_path; }

//--------------------------------------------------------------------------------------
inline const QImage &RImageDescriptor::image() const
{ return m_image; }

//--------------------------------------------------------------------------------------
inline const QString &RImageDescriptor::text() const
{ return m_text; }

Q_DECLARE_METATYPE(RImageDescriptor)
Q_DECLARE_METATYPE(const RImageDescriptor *)

#endif // RIMAGEDESCRIPTOR_H
