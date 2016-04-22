#include "rcompositemapobjectmodel.h"

//--------------------------------------------------------------------------------------
RCompositeMapObjectModel::RCompositeMapObjectModel(QObject *parent) :
    RMapObjectModel(parent),
    text(tr(""))
{}

//--------------------------------------------------------------------------------------
RCompositeMapObjectModel::~RCompositeMapObjectModel()
{}

//--------------------------------------------------------------------------------------
RCompositeMapObjectModel *RCompositeMapObjectModel::clone() const
{
    RCompositeMapObjectModel *m = new RCompositeMapObjectModel;
    initModel(m);
    return m;
}

//--------------------------------------------------------------------------------------
void RCompositeMapObjectModel::initModel(RCompositeMapObjectModel *m) const
{ RMapObjectModel::initModel(m); }

//--------------------------------------------------------------------------------------
int RCompositeMapObjectModel::rowCount(const QModelIndex &parent) const
{ return RMapObjectModel::EN_SECTIONS_COUNT; }

//--------------------------------------------------------------------------------------
const QString &RCompositeMapObjectModel::caption() const
{ return text; }

//--------------------------------------------------------------------------------------
bool RCompositeMapObjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        switch (index.row()) {
        case EN_WIDTH:
        case EN_HEIGHT:
            return false;
        default:
            return RMapObjectModel::setData(index, value, role);
        }
    }
    return false;
}
