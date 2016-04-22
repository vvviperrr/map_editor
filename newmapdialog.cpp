#include "newmapdialog.h"

#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QSettings>

//--------------------------------------------------------------------------------------
NewMapDialog::NewMapDialog(QWidget *parent) :
    QDialog(parent)
{
	setModal(true);
    setWindowTitle(tr("Новая карта"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	okButton = new QPushButton(tr("Ok"), this);
	cancelButton = new QPushButton(tr("Отмена"), this);

	widthEdit = new QSpinBox(this);
	widthEdit->setMinimum(1);
	widthEdit->setMaximum(10000);
    widthEdit->setValue(1000);

	heightEdit = new QSpinBox(this);
	heightEdit->setMinimum(1);
	heightEdit->setMaximum(10000);
    heightEdit->setValue(1000);

	useGridCheckBox = new QCheckBox(this);
	useGridCheckBox->setText(tr("Использовать сетку:"));

	gridSizeSpinBox = new QSpinBox(this);
	gridSizeSpinBox->setMinimum(5);
	gridSizeSpinBox->setMaximum(100);
    gridSizeSpinBox->setValue(10);

    userLabel = new QLabel(tr("Пользователь:"), this);
    passwordLabel = new QLabel(tr("Пароль:"), this);

    userEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);

    userEdit->setText(tr("Admin"));

    passwordEdit->setText(tr("1"));
    passwordEdit->setEchoMode(QLineEdit::Password);

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	widthLabel = new QLabel(tr("Ширина:"));
	widthLabel->setBuddy(widthEdit);

	heightLabel = new QLabel(tr("Высота:"));
	heightLabel->setBuddy(heightEdit);

	gridSizeLabel = new QLabel(tr("Размер сетки:"));
	gridSizeLabel->setBuddy(gridSizeSpinBox);

	buttonBox = new QDialogButtonBox(Qt::Horizontal);
	buttonBox->addButton(okButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);

	QGridLayout *gridLayout = new QGridLayout;

	gridLayout->addWidget(widthLabel, 0, 0);
	gridLayout->addWidget(widthEdit, 0, 1);

	gridLayout->addWidget(heightLabel, 1, 0);
	gridLayout->addWidget(heightEdit, 1, 1);

	gridLayout->addWidget(useGridCheckBox, 2, 0, 1, 2, Qt::AlignLeft);

	gridLayout->addWidget(gridSizeLabel, 3, 0);
	gridLayout->addWidget(gridSizeSpinBox, 3, 1);

    gridLayout->addWidget(userLabel, 4, 0);
    gridLayout->addWidget(userEdit, 4, 1);

    gridLayout->addWidget(passwordLabel, 5, 0);
    gridLayout->addWidget(passwordEdit, 5, 1);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->addLayout(gridLayout, 1, 0);
	mainLayout->addWidget(buttonBox, 2, 0);

	setLayout(mainLayout);

    QSettings settings;
    if (!settings.value("NewMapDialog::mapWidth").isNull()) {
        widthEdit->setValue(settings.value("NewMapDialog::mapWidth").toInt());
        heightEdit->setValue(settings.value("NewMapDialog::mapHeight").toInt());
        gridSizeSpinBox->setValue(settings.value("NewMapDialog::gridSize").toInt());
        useGridCheckBox->setChecked(settings.value("NewMapDialog::useGrid").toBool());
        userEdit->setText(settings.value("NewMapDialog::user").toString());
        passwordEdit->setText(settings.value("NewMapDialog::password").toString());
    }

    connect(useGridCheckBox, SIGNAL(stateChanged(int)), this, SLOT(useGridCheckChanged(int)));
    connect(this, SIGNAL(accepted()), this, SLOT(actionOk()));
    useGridCheckChanged(useGridCheckBox->isChecked());
}

//--------------------------------------------------------------------------------------
void NewMapDialog::actionOk()
{
    QSettings settings;

    settings.setValue("NewMapDialog::mapWidth", mapWidth());
    settings.setValue("NewMapDialog::mapHeight", mapHeight());
    settings.setValue("NewMapDialog::gridSize", gridSize());
    settings.setValue("NewMapDialog::useGrid", useGrid());
    settings.setValue("NewMapDialog::user", user());
    settings.setValue("NewMapDialog::password", password());
}

//--------------------------------------------------------------------------------------
void NewMapDialog::useGridCheckChanged(int state)
{ gridSizeSpinBox->setEnabled(state); }

//--------------------------------------------------------------------------------------
int NewMapDialog::mapWidth() const
{ return widthEdit->value(); }

//--------------------------------------------------------------------------------------
int NewMapDialog::mapHeight() const
{ return heightEdit->value(); }

//--------------------------------------------------------------------------------------
bool NewMapDialog::useGrid() const
{ return useGridCheckBox->isChecked(); }

//--------------------------------------------------------------------------------------
int NewMapDialog::gridSize() const
{ return useGridCheckBox->isChecked() ? gridSizeSpinBox->value() : 0; }

//--------------------------------------------------------------------------------------
QString NewMapDialog::user() const
{ return userEdit->text(); }

//--------------------------------------------------------------------------------------
QString NewMapDialog::password() const
{ return passwordEdit->text(); }
