#include "rpictureselectordialog.h"
#include "rmapregistry.h"

#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QScrollArea>
#include <QSplitter>
#include <QDebug>
#include <QSettings>

//--------------------------------------------------------------------------------------
RPictureSelectorDialog::RPictureSelectorDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Выбор картинки"));

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
    connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(listItemSelectionChanged()));

    // специальное значение - без картинки
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(tr("Без картинки"));
    item->setData(Qt::UserRole, 0);
    listWidget->addItem(item);

    // заполним листбокс доступными картинками
    foreach (const RImageDescriptor &id, RMapRegistry::instance().imageMap()) {
        item = new QListWidgetItem;
        item->setText(id.path());
        item->setData(Qt::UserRole, id.id());
        listWidget->addItem(item);
    }

    QSettings settings;
    if (!settings.value("PictureSelectorGeometry").isNull()) {
        restoreGeometry(settings.value("PictureSelectorGeometry").toByteArray());
        splitter->restoreGeometry(settings.value("PictureSelectorSplitterGeometry").toByteArray());
        splitter->restoreState(settings.value("PictureSelectorSplitterState").toByteArray());
    }

    setLayout(mainLayout);
}

//--------------------------------------------------------------------------------------
void RPictureSelectorDialog::listItemSelectionChanged()
{
    if (QListWidgetItem *item = listWidget->currentItem()) {
        int image_id = item->data(Qt::UserRole).toInt();
        const QImage &image = RMapRegistry::instance().getImageDescriptorById(image_id).image();
        QRect geom = label->geometry();
        geom.setSize(image.size());
        label->setGeometry(geom);
        label->setPixmap(QPixmap::fromImage(image));
    } else {
        label->clear();
    }
}

//--------------------------------------------------------------------------------------
void RPictureSelectorDialog::setCurrentImageDescriptor(const RImageDescriptor *id)
{
    for (int i = 0; i < listWidget->count(); i++) {
        if (QListWidgetItem *item = listWidget->item(i)) {
            if (item->data(Qt::UserRole).toInt() == id->id()) {
                listWidget->setCurrentRow(i);
                return;
            }
        }
    }
}

//--------------------------------------------------------------------------------------
const RImageDescriptor *RPictureSelectorDialog::currentImageDescriptor() const
{
    if (QListWidgetItem *item = listWidget->currentItem()) {
        return &RMapRegistry::instance().getImageDescriptorById(item->data(Qt::UserRole).toInt());
    }
    return &RMapRegistry::instance().emptyImage();
}

//--------------------------------------------------------------------------------------
void RPictureSelectorDialog::hideEvent(QHideEvent *event)
{
    if (result()) {
        QSettings settings;
        settings.setValue("PictureSelectorGeometry", saveGeometry());
        settings.setValue("PictureSelectorSplitterGeometry", splitter->saveGeometry());
        settings.setValue("PictureSelectorSplitterState", splitter->saveState());
    }

    QDialog::hideEvent(event);
}

//--------------------------------------------------------------------------------------
QSize RPictureSelectorDialog::sizeHint() const
{
    return QSize(400, 500);
}
