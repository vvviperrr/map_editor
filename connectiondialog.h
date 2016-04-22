#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

class QPushButton;
class QLabel;
class QLineEdit;

class ConnectionDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConnectionDialog(QWidget *parent = 0);

    const QString host() const;
    int port() const;
    const QString dbname() const;
    const QString user() const;
    const QString password() const;

    void setHost(const QString &str);
    void setPort(int port);
    void setDBName(const QString &str);
    void setUser(const QString &str);
    void setPassword(const QString &str);

private slots:
	void checkConnection();

private:
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
	QPushButton *buttonCheckConnection;

    QLabel *hostLabel;
    QLabel *portLabel;
    QLabel *dbnameLabel;
    QLabel *userLabel;
    QLabel *passwordLabel;

    QLineEdit *hostEdit;
    QLineEdit *portEdit;
    QLineEdit *dbnameEdit;
    QLineEdit *userEdit;
    QLineEdit *passwordEdit;
};

#endif // CONNECTIONDIALOG_H
