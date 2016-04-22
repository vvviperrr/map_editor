#include "rmapobjecttableviewdelegate.h"
#include "rmapobjectmodel.h"
#include "rfontselectordialog.h"
#include "rpictureselectordialog.h"
#include "rfontdescriptor.h"
#include "rimagedescriptor.h"

#include "rrefsdescriptor.h"
#include "rmapregistry.h"

#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QColorDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>

//--------------------------------------------------------------------------------------
namespace {

// все диалоги должны создаваться через этот вспомогательный класс
//--------------------------------------------------------------------------------------
template <typename T>
class RDelegateDialog : public T {
public:
    RDelegateDialog(QWidget *parent = 0) :
        T(parent)
    {}

    virtual void done(int result)
    {
        T::setResult(result);
        T::hide();
    }
};

// установить диалог по центру экрана
//--------------------------------------------------------------------------------------
void setDialogCenterScreen(QWidget *w)
{
    QPoint p = QApplication::desktop()->geometry().center();
    QSize s = w->sizeHint();
    p.setX(p.x() - s.width() / 2);
    p.setY(p.y() - s.height() / 2);
    w->move(p);
}

} // namespace


//--------------------------------------------------------------------------------------
RMapObjectTableViewDelegate::RMapObjectTableViewDelegate(QObject *parent /*= 0*/) :
    QItemDelegate(parent)
{}

//--------------------------------------------------------------------------------------
QWidget *RMapObjectTableViewDelegate::createEditor(QWidget *parent,
                                                   const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const
{
    switch (index.data(Qt::UserRole).toInt()) {
    case RMapObjectModel::ET_COLOR_SELECTOR: {
        QColorDialog *editor = new RDelegateDialog<QColorDialog>(parent);
        return editor;
    }

    case RMapObjectModel::ET_FONT_SELECTOR: {
        RFontSelectorDialog *editor = new RDelegateDialog<RFontSelectorDialog>(parent);
        return editor;
    }

    case RMapObjectModel::ET_PICTURE_SELECTOR: {
        RPictureSelectorDialog *editor = new RDelegateDialog<RPictureSelectorDialog>(parent);
        return editor;
    }

    case RMapObjectModel::ET_REFS_SELECTOR: {
        QComboBox *box = new QComboBox(parent);
        foreach (const RRefsDescriptor &refs, RMapRegistry::instance().refsMap())
            box->addItem(refs.text(), refs.id());
        connect(box, SIGNAL(activated(int)), this, SLOT(commitAndCloseEditor(int)));
        return box;
    }

    default:
        return QItemDelegate::createEditor(parent, option, index);
    }
}

//--------------------------------------------------------------------------------------
void RMapObjectTableViewDelegate::setEditorData(QWidget *editor,
                                                const QModelIndex &index) const
{

    switch (index.data(Qt::UserRole).toInt()) {
    case RMapObjectModel::ET_COLOR_SELECTOR: {
        QColorDialog *colorbox = static_cast<QColorDialog *>(editor);
        QColor color = index.model()->data(index, Qt::EditRole).value<QColor>();
        colorbox->setCurrentColor(color);
        break;
    }

    case RMapObjectModel::ET_FONT_SELECTOR: {
        RFontSelectorDialog *fontSelector = static_cast<RFontSelectorDialog *>(editor);
        const RFontDescriptor *fd = index.model()->data(index, Qt::EditRole).value<const RFontDescriptor *>();
        fontSelector->setCurrentFontDescriptor(fd);
        break;
    }

    case RMapObjectModel::ET_PICTURE_SELECTOR: {
        RPictureSelectorDialog *pictureSelector = static_cast<RPictureSelectorDialog *>(editor);
        const RImageDescriptor *id = index.model()->data(index, Qt::EditRole).value<const RImageDescriptor *>();
        pictureSelector->setCurrentImageDescriptor(id);
        break;
    }

    case RMapObjectModel::ET_REFS_SELECTOR: {
        QComboBox *refsSelector = static_cast<QComboBox *>(editor);
        const RRefsDescriptor *rd = index.model()->data(index, Qt::EditRole).value<const RRefsDescriptor *>();
        int pos = refsSelector->findData(rd->id());
        refsSelector->setCurrentIndex(pos);
        break;
    }

    default:
        QItemDelegate::setEditorData(editor, index);
        break;
    }
}

//--------------------------------------------------------------------------------------
void RMapObjectTableViewDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{   
    switch (index.data(Qt::UserRole).toInt()) {
    case RMapObjectModel::ET_COLOR_SELECTOR: {
        QColorDialog *colorbox = static_cast<QColorDialog *>(editor);
        if (colorbox->result() == QDialog::Accepted) {
            QColor color = colorbox->currentColor();
            model->setData(index, color, Qt::EditRole);
        }
        break;
    }

    case RMapObjectModel::ET_FONT_SELECTOR: {
        RFontSelectorDialog *fontSelector = static_cast<RFontSelectorDialog *>(editor);
        if (fontSelector->result() == QDialog::Accepted) {
            const RFontDescriptor *fd = fontSelector->currentFontDescriptor();
            model->setData(index, qVariantFromValue(fd), Qt::EditRole);
        }
        break;
    }

    case RMapObjectModel::ET_PICTURE_SELECTOR: {
        RPictureSelectorDialog *pictureSelector = static_cast<RPictureSelectorDialog *>(editor);
        if (pictureSelector->result() == QDialog::Accepted) {
            const RImageDescriptor *id = pictureSelector->currentImageDescriptor();
            model->setData(index, qVariantFromValue(id), Qt::EditRole);
        }
        break;
    }

    case RMapObjectModel::ET_REFS_SELECTOR: {
        QComboBox *refsSelector = static_cast<QComboBox *>(editor);
        int row = refsSelector->currentIndex();
        int data = refsSelector->itemData(row).toInt();
        const RRefsDescriptor *rd = &RMapRegistry::instance().getRefsDescriptorById(data);
        model->setData(index, qVariantFromValue(rd), Qt::EditRole);
        break;
    }

    default:
        QItemDelegate::setModelData(editor, model, index);
        break;
    }
}

//--------------------------------------------------------------------------------------
void RMapObjectTableViewDelegate::updateEditorGeometry(QWidget *editor,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &index) const
{
    switch (index.data(Qt::UserRole).toInt()) {
    case RMapObjectModel::ET_COLOR_SELECTOR:
    case RMapObjectModel::ET_FONT_SELECTOR:
    case RMapObjectModel::ET_PICTURE_SELECTOR:
        setDialogCenterScreen(editor);
        break;

    default:
        QItemDelegate::updateEditorGeometry(editor, option, index);
        break;
    }
}

//--------------------------------------------------------------------------------------
void RMapObjectTableViewDelegate::paint(QPainter *painter,
										const QStyleOptionViewItem &option,
										const QModelIndex &index) const
{
    switch (index.data(Qt::UserRole).toInt()) {
    case RMapObjectModel::ET_COLOR_SELECTOR: {
        QColor color = index.model()->data(index, Qt::DisplayRole).value<QColor>();
        painter->fillRect(option.rect, color);
        drawFocus(painter, option, option.rect);
        break;
    }

    case RMapObjectModel::ET_FONT_SELECTOR: {
        const RFontDescriptor *fd = index.model()->data(index, Qt::DisplayRole).value<const RFontDescriptor *>();
        QStyleOptionViewItem opt = option;
        opt.displayAlignment = Qt::AlignCenter;
        drawDisplay(painter, opt, opt.rect, fd->text());
        drawFocus(painter, opt, opt.rect);
        break;
    }

    case RMapObjectModel::ET_PICTURE_SELECTOR: {
        const RImageDescriptor *id = index.model()->data(index, Qt::DisplayRole).value<const RImageDescriptor *>();
        QStyleOptionViewItem opt = option;
        opt.displayAlignment = Qt::AlignCenter;
        drawDisplay(painter, opt, opt.rect, id->text());
        drawFocus(painter, opt, opt.rect);
        break;
    }

    case RMapObjectModel::ET_REFS_SELECTOR: {
        const RRefsDescriptor *rd = index.model()->data(index, Qt::DisplayRole).value<const RRefsDescriptor *>();
        QStyleOptionViewItem opt = option;
        opt.displayAlignment = Qt::AlignCenter;
        drawDisplay(painter, opt, opt.rect, rd->text());
        drawFocus(painter, opt, opt.rect);
        break;
    }

    default:
        QItemDelegate::paint(painter, option, index);
        break;
    }
}

//--------------------------------------------------------------------------------------
QSize RMapObjectTableViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.data(Qt::UserRole).toInt()) {
    case RMapObjectModel::ET_COLOR_SELECTOR: {
        return QSize(100, option.font.pixelSize());
    }

    case RMapObjectModel::ET_FONT_SELECTOR: {
        const RFontDescriptor *fd = index.model()->data(index, Qt::DisplayRole).value<const RFontDescriptor *>();
        return option.fontMetrics.boundingRect(fd->text()).size();
    }

    case RMapObjectModel::ET_PICTURE_SELECTOR: {
        const RImageDescriptor *id = index.model()->data(index, Qt::DisplayRole).value<const RImageDescriptor *>();
        return option.fontMetrics.boundingRect(id->text()).size();
    }

    case RMapObjectModel::ET_REFS_SELECTOR: {
        const RRefsDescriptor *rd = index.model()->data(index, Qt::DisplayRole).value<const RRefsDescriptor *>();
        return option.fontMetrics.boundingRect(rd->text()).size();
    }

    default:
        return QItemDelegate::sizeHint(option, index);
    }
}

//--------------------------------------------------------------------------------------
void RMapObjectTableViewDelegate::commitAndCloseEditor(int id)
{
    QWidget *editor = qobject_cast<QWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
