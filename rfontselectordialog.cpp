#include "rfontselectordialog.h"
#include "rmapregistry.h"

#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSplitter>
#include <QDebug>
#include <QSettings>

//--------------------------------------------------------------------------------------
RFontSelectorDialog::RFontSelectorDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Выбор шрифта"));

    listWidget = new QListWidget;
    listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    label = new QLabel;
    scrollArea = new QScrollArea;
    scrollArea->setWidget(label);

    buttonOk = new QPushButton(tr("Ok"));
    buttonCancel = new QPushButton(tr("Отмена"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(buttonOk, QDialogButtonBox::ActionRole);
    buttonBox->addButton(buttonCancel, QDialogButtonBox::ActionRole);

    splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(listWidget);
    splitter->addWidget(scrollArea);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(splitter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(buttonBox, 0, Qt::AlignCenter);

    connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(listWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(listItemSelectionChanged()));

    // заполним листбокс доступными шрифтами
    foreach (const RFontDescriptor &fd, RMapRegistry::instance().fontsMap()) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(fd.path());
        item->setData(Qt::UserRole, fd.id());
        listWidget->addItem(item);
    }

    QSettings settings;
    if (!settings.value("FontSelectorGeometry").isNull()) {
        restoreGeometry(settings.value("FontSelectorGeometry").toByteArray());
        splitter->restoreGeometry(settings.value("FontSelectorSplitterGeometry").toByteArray());
        splitter->restoreState(settings.value("FontSelectorSplitterState").toByteArray());
    }

    setLayout(mainLayout);
}

//--------------------------------------------------------------------------------------
void RFontSelectorDialog::listItemSelectionChanged()
{
    if (QListWidgetItem *item = listWidget->currentItem()) {
        int font_id = item->data(Qt::UserRole).toInt();
        const QFont &font = RMapRegistry::instance().getFontDescriptorById(font_id).font();
        label->setFont(font);
        label->setText(font.family());
        QRect rect = label->geometry();
        rect.setSize(label->sizeHint());
        label->setGeometry(rect);
    } else {
        label->clear();
    }
}

//--------------------------------------------------------------------------------------
void RFontSelectorDialog::setCurrentFontDescriptor(const RFontDescriptor *fd)
{
    for (int i = 0; i < listWidget->count(); i++) {
        if (QListWidgetItem *item = listWidget->item(i)) {
            if (item->data(Qt::UserRole).toInt() == fd->id()) {
                listWidget->setCurrentRow(i);
                return;
            }
        }
    }
}

//--------------------------------------------------------------------------------------
const RFontDescriptor *RFontSelectorDialog::currentFontDescriptor() const
{
    if (QListWidgetItem *item = listWidget->currentItem()) {
        return &RMapRegistry::instance().getFontDescriptorById(item->data(Qt::UserRole).toInt());
    }
    return &RMapRegistry::instance().emptyFont();
}

//--------------------------------------------------------------------------------------
void RFontSelectorDialog::hideEvent(QHideEvent *event)
{
    if (result()) {
        QSettings settings;
        settings.setValue("FontSelectorGeometry", saveGeometry());
        settings.setValue("FonteSelectorSplitterGeometry", splitter->saveGeometry());
        settings.setValue("FontSelectorSplitterState", splitter->saveState());
    }

    QDialog::hideEvent(event);
}

//--------------------------------------------------------------------------------------
QSize RFontSelectorDialog::sizeHint() const
{ return QSize(400, 250); }
