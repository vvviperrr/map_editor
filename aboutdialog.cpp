#include "aboutdialog.h"

//--------------------------------------------------------------------------------------
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("О программе"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}
