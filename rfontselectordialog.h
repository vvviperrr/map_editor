#ifndef RFONTSELECTORDIALOG_H
#define RFONTSELECTORDIALOG_H

#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QPushButton;
class QLabel;
class QScrollArea;
class QSplitter;

class RFontDescriptor;

class RFontSelectorDialog : public QDialog {
    Q_OBJECT
public:
    explicit RFontSelectorDialog(QWidget *parent = 0);

    virtual QSize sizeHint() const;

    void setCurrentFontDescriptor(const RFontDescriptor *fd);
    const RFontDescriptor *currentFontDescriptor() const;

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

#endif // RFONTSELECTORDIALOG_H
