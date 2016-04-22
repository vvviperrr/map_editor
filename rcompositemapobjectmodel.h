#ifndef RCOMPOSITEMAPOBJECTMODEL_H
#define RCOMPOSITEMAPOBJECTMODEL_H

#include "rmapobjectmodel.h"

class RCompositeMapObjectModel : public RMapObjectModel {
    Q_OBJECT
public:
    explicit RCompositeMapObjectModel(QObject *parent = 0);
    ~RCompositeMapObjectModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    virtual RCompositeMapObjectModel *clone() const;
    virtual const QString &caption() const;

protected:
    void initModel(RCompositeMapObjectModel *m) const;

private:
    QString text;
};

#endif // RCOMPOSITEMAPOBJECTMODEL_H
