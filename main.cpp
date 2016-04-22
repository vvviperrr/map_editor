#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <QtSql>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    a.addLibraryPath("./");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QSqlDatabase::addDatabase("QDB2");

    QCoreApplication::setOrganizationName("TwoByte");
    QCoreApplication::setApplicationName("BitRestMapEditor");

    MainWindow wnd;
    wnd.show();
    return a.exec();
}
