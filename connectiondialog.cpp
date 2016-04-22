#include "connectiondialog.h"

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QtSql>
#include <QDebug>
#include <QMessageBox>

//--------------------------------------------------------------------------------------
ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Настройки соединения"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    buttonOk = new QPushButton(tr("Ok"));
    buttonCancel = new QPushButton(tr("Отмена"));
    buttonCheckConnection = new QPushButton(tr("Проверить соединение"));

    hostLabel = new QLabel(tr("Хост:"));
    portLabel = new QLabel(tr("Порт:"));
    dbnameLabel = new QLabel(tr("База данных:"));
    userLabel = new QLabel(tr("Логин:"));
    passwordLabel = new QLabel(tr("Пароль:"));

    hostEdit = new QLineEdit;
    portEdit = new QLineEdit;
    dbnameEdit = new QLineEdit;
    userEdit = new QLineEdit;
    passwordEdit = new QLineEdit;

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(buttonOk, QDialogButtonBox::ActionRole);
    buttonBox->addButton(buttonCancel, QDialogButtonBox::ActionRole);

    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(hostLabel, 0, 0);
    gridLayout->addWidget(hostEdit, 0, 1);

    gridLayout->addWidget(portLabel, 1, 0);
    gridLayout->addWidget(portEdit, 1, 1);

    gridLayout->addWidget(dbnameLabel, 2, 0);
    gridLayout->addWidget(dbnameEdit, 2, 1);

    gridLayout->addWidget(userLabel, 3, 0);
    gridLayout->addWidget(userEdit, 3, 1);

    gridLayout->addWidget(passwordLabel, 4, 0);
    gridLayout->addWidget(passwordEdit, 4, 1);

	gridLayout->addWidget(buttonCheckConnection, 5, 0, 1, 5, Qt::AlignCenter);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->addLayout(gridLayout, 1, 0, Qt::AlignCenter);
	mainLayout->addWidget(buttonBox, 2, 0, Qt::AlignCenter);

    setLayout(mainLayout);

    connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(buttonCheckConnection, SIGNAL(clicked()), this, SLOT(checkConnection()));

    buttonOk->setDefault(true);
}

//--------------------------------------------------------------------------------------
void ConnectionDialog::checkConnection()
{
    setEnabled(false);
    repaint();

    QSqlDatabase db = QSqlDatabase::database();
	db.setHostName(host());
	db.setPort(port());
	db.setDatabaseName(dbname());
	db.setUserName(user());
	db.setPassword(password());

	QMessageBox msg(this);

	if (db.open()) {
		msg.setText(tr("Соединение с бд успешно установлено."));
		msg.setIcon(QMessageBox::Information);
	} else {
		msg.setText(QString(tr("Ошибка установления соединения с бд.\nПричина:\n%1")).arg(db.lastError().text()));
		msg.setIcon(QMessageBox::Critical);
	}

	msg.exec();
    db.close();

    setEnabled(true);
    repaint();
}

//--------------------------------------------------------------------------------------
const QString ConnectionDialog::host() const
{ return hostEdit->text(); }

//--------------------------------------------------------------------------------------
int ConnectionDialog::port() const
{ return portEdit->text().toInt(); }

//--------------------------------------------------------------------------------------
const QString ConnectionDialog::dbname() const
{ return dbnameEdit->text(); }

//--------------------------------------------------------------------------------------
const QString ConnectionDialog::user() const
{ return userEdit->text(); }

//--------------------------------------------------------------------------------------
const QString ConnectionDialog::password() const
{ return passwordEdit->text(); }

//--------------------------------------------------------------------------------------
void ConnectionDialog::setHost(const QString &str)
{ hostEdit->setText(str); }

//--------------------------------------------------------------------------------------
void ConnectionDialog::setPort(int port)
{ portEdit->setText(QString().number(port)); }

//--------------------------------------------------------------------------------------
void ConnectionDialog::setDBName(const QString &str)
{ dbnameEdit->setText(str); }

//--------------------------------------------------------------------------------------
void ConnectionDialog::setUser(const QString &str)
{ userEdit->setText(str); }

//--------------------------------------------------------------------------------------
void ConnectionDialog::setPassword(const QString &str)
{ passwordEdit->setText(str); }
