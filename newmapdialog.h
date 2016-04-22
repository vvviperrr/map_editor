#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include <QDialog>

class QPushButton;
class QComboBox;
class QSpinBox;
class QLabel;
class QLineEdit;
class QDialogButtonBox;
class QCheckBox;

class NewMapDialog : public QDialog {
    Q_OBJECT
public:
    explicit NewMapDialog(QWidget *parent = 0);

    int mapWidth() const;
    int mapHeight() const;
    int gridSize() const;
    bool useGrid() const;

    QString user() const;
    QString password() const;

private slots:
	void useGridCheckChanged(int state);
    void actionOk();

private:
	QPushButton *okButton;
	QPushButton *cancelButton;

	QLabel *widthLabel;
	QSpinBox *widthEdit;

	QLabel *heightLabel;
	QSpinBox *heightEdit;

	QCheckBox *useGridCheckBox;

	QLabel *gridSizeLabel;
	QSpinBox *gridSizeSpinBox;

	QDialogButtonBox *buttonBox;

    QLabel *userLabel;
    QLineEdit *userEdit;

    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
};

#endif // NEWMAPDIALOG_H
