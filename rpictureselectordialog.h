#ifndef RPICTURESELECTORDIALOG_H
#define RPICTURESELECTORDIALOG_H

#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QPushButton;
class QLabel;
class QScrollArea;
class QSplitter;

class RImageDescriptor;

class RPictureSelectorDialog : public QDialog {
    Q_OBJECT
public:
    explicit RPictureSelectorDialog(QWidget *parent = 0);

    virtual QSize sizeHint() const;

    void setCurrentImageDescriptor(const RImageDescriptor *id);
    const RImageDescriptor *currentImageDescriptor() const;

protected:
    virtual void hideEvent(QHideEvent *event);

private slots:
    void listItemSelectionChanged();

private:
    QLabel *label;
    QListWidget *listWidget;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QScrollArea *scrollArea;
    QSplitter *splitter;
};

#endif // RPICTURESELECTORDIALOG_H
