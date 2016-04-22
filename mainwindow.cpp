#include "mainwindow.h"
#include <QTableView>
#include <QSplitter>
#include <QScrollArea>
#include <QCloseEvent>
#include <QDebug>
#include <QToolBar>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QSettings>
#include <QMessageBox>
#include <QTreeWidget>
#include <QtSql>
#include <QHeaderView>
#include <QLabel>

#include "rtable.h"
#include "rsection.h"
#include "rmapobjectmovepolicy.h"
#include "rmapobjectresizepolicy.h"
#include "rgridwidget.h"
#include "newmapdialog.h"
#include "connectiondialog.h"
#include "rmapobjectfactory.h"
#include "rmapobjecttreewidgetitem.h"
#include "rmapobjecttableviewdelegate.h"
#include "rmapobjectfocusholder.h"
#include "rmapregistry.h"
#include "aboutdialog.h"
#include "rcompositemapobject.h"

namespace {
inline int serialize_rcolor(const QColor& c)
{ return static_cast<int>(qRgb(c.blue(), c.green(), c.red())); }

inline QColor deserialize_rcolor(int i)
{ return QColor(qBlue(i), qGreen(i), qRed(i)); }
}

//--------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	gridWidget(0),
	treeWidget(0),
	tableView(0),
	objectFactory(0),
	splitter1(0),
	splitter2(0),
    scrollArea(0),
    userID(0),
    delReasonID(0)
{
    connectionInfo.host = tr("localhost");
    connectionInfo.port = 50000;
    connectionInfo.dbname = tr("payment");
    connectionInfo.user = tr("db2admin");
    connectionInfo.password = tr("1");

	initWidgets();
	createToolBars();
    loadConfig();
    onCloseMap();
}

//--------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    blockSignals(true);
    treeWidget->blockSignals(true);
    tableView->blockSignals(true);

    delete gridWidget;
    delete treeWidget;
    delete tableView;
}

//--------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (gridWidget) {
        if (!commitChanges()) {
            event->ignore();
            return;
        }
    }

    QSettings settings;

    settings.setValue("MainWindowGeometry", saveGeometry());
    settings.setValue("MainWindowState", saveState());

    settings.setValue("MainWindowSplitter1Geometry", splitter1->saveGeometry());
    settings.setValue("MainWindowSplitter1State", splitter1->saveState());

    settings.setValue("MainWindowSplitter2Geometry", splitter2->saveGeometry());
    settings.setValue("MainWindowSplitter2State", splitter2->saveState());

    settings.setValue("connectionInfo::host", connectionInfo.host);
    settings.setValue("connectionInfo::port", connectionInfo.port);
    settings.setValue("connectionInfo::dbname", connectionInfo.dbname);
    settings.setValue("connectionInfo::user", connectionInfo.user);
    settings.setValue("connectionInfo::password", connectionInfo.password);

	QMainWindow::closeEvent(event);
}

//--------------------------------------------------------------------------------------
void MainWindow::loadConfig()
{
    QSettings settings;

    restoreGeometry(settings.value("MainWindowGeometry").toByteArray());
    restoreState(settings.value("MainWindowState").toByteArray());

    splitter1->restoreGeometry(settings.value("MainWindowSplitter1Geometry").toByteArray());
    splitter1->restoreState(settings.value("MainWindowSplitter1State").toByteArray());

    splitter2->restoreGeometry(settings.value("MainWindowSplitter2Geometry").toByteArray());
    splitter2->restoreState(settings.value("MainWindowSplitter2State").toByteArray());

    if (!settings.value("connectionInfo::host").isNull()) {
        connectionInfo.host = settings.value("connectionInfo::host").toString();
        connectionInfo.port = settings.value("connectionInfo::port").toInt();
        connectionInfo.dbname = settings.value("connectionInfo::dbname").toString();
        connectionInfo.user = settings.value("connectionInfo::user").toString();
        connectionInfo.password = settings.value("connectionInfo::password").toString();
    }
}

//--------------------------------------------------------------------------------------
void MainWindow::initWidgets()
{
    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabel(tr("Список объектов"));
    treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableView = new QTableView(this);
    tableView->verticalHeader()->hide();
    tableView->setItemDelegateForColumn(1, new RMapObjectTableViewDelegate(tableView));

	scrollArea = new QScrollArea(this);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	splitter1 = new QSplitter(Qt::Vertical);
	splitter1->addWidget(treeWidget);
	splitter1->addWidget(tableView);

	splitter2 = new QSplitter(Qt::Horizontal);
	splitter2->addWidget(scrollArea);
	splitter2->addWidget(splitter1);

    connectionInfoLabel = new QLabel;
    statusBar()->addWidget(connectionInfoLabel);

	setCentralWidget(splitter2);

    connect(treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(treeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

    connect(&RMapObjectFocusHolder::instance(), SIGNAL(focusMapObjectChanged(RMapObject*)),
            this, SLOT(focusChanged(RMapObject*)));
}

//--------------------------------------------------------------------------------------
void MainWindow::createToolBars()
{    
    openMapAction = new QAction(tr("Открыть карту"), this);
    closeMapAction = new QAction(tr("Закрыть карту"), this);
    createSectionAction = new QAction(tr("Новый зал"), this);
    createTableAction = new QAction(tr("Новый стол"), this);
    copyAction = new QAction(tr("Копировать"), this);
    deleteAction = new QAction(tr("Удалить"), this);
    connectionSettingsAction = new QAction(tr("Настройки соединения"), this);
    exitAction = new QAction(tr("Выход"), this);
    aboutAction = new QAction(tr("О программе"), this);
    aboutQtAction = new QAction(tr("О Qt"), this);

    copyAction->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_C));
    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));

    toolbarAction = new QAction(tr("Строка меню"), this);
    toolbarAction->setCheckable(true);
    toolbarAction->setChecked(true);

    statusbarAction = new QAction(tr("Строка состояния"), this);
    statusbarAction->setCheckable(true);
    statusbarAction->setChecked(true);

    treeviewAction = new QAction(tr("Дерево объектов"), this);
    treeviewAction->setCheckable(true);
    treeviewAction->setChecked(true);

    tableviewAction = new QAction(tr("Окно свойств"), this);
    tableviewAction->setCheckable(true);
    tableviewAction->setChecked(true);

    QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(openMapAction);
    fileMenu->addAction(closeMapAction);
    fileMenu->addSeparator();
    fileMenu->addAction(createSectionAction);
    fileMenu->addAction(createTableAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu *editMenu = menuBar()->addMenu(tr("&Правка"));
    editMenu->addAction(copyAction);
    editMenu->addAction(deleteAction);

    QMenu *viewMenu = menuBar()->addMenu(tr("&Вид"));
    viewMenu->addAction(toolbarAction);
    viewMenu->addAction(statusbarAction);
    viewMenu->addAction(treeviewAction);
    viewMenu->addAction(tableviewAction);

    QMenu *optionsMenu = menuBar()->addMenu(tr("&Настройки"));
    optionsMenu->addAction(connectionSettingsAction);

    QMenu *aboutMenu = menuBar()->addMenu(tr("&Справка"));
    aboutMenu->addAction(aboutAction);
    aboutMenu->addAction(aboutQtAction);

    toolBar = addToolBar(tr("MainToolBar"));
    toolBar->setObjectName(tr("MainToolBar"));
    toolBar->addAction(openMapAction);
    toolBar->addAction(closeMapAction);
    toolBar->addSeparator();
    toolBar->addAction(createSectionAction);
    toolBar->addAction(createTableAction);
    toolBar->addSeparator();
    toolBar->addAction(copyAction);
    toolBar->addAction(deleteAction);
    toolBar->addSeparator();
    toolBar->addAction(connectionSettingsAction);
    toolBar->addSeparator();
    toolBar->addAction(exitAction);

    toolBar->setFloatable(false);
    toolBar->setMovable(false);

    connect(openMapAction, SIGNAL(triggered()), SLOT(onOpenMap()));
    connect(closeMapAction, SIGNAL(triggered()), SLOT(onCloseMap()));
    connect(createSectionAction, SIGNAL(triggered()), SLOT(onCreateSection()));
	connect(createTableAction, SIGNAL(triggered()), SLOT(onCreateTable()));
	connect(copyAction, SIGNAL(triggered()), SLOT(onCopyMapObject()));
	connect(deleteAction, SIGNAL(triggered()), SLOT(onDeleteMapObject()));
    connect(connectionSettingsAction, SIGNAL(triggered()), this, SLOT(onConnectionSettings()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(toolbarAction, SIGNAL(triggered(bool)), toolBar, SLOT(setShown(bool)));
    connect(statusbarAction, SIGNAL(triggered(bool)), statusBar(), SLOT(setShown(bool)));
    connect(treeviewAction, SIGNAL(triggered(bool)), treeWidget, SLOT(setShown(bool)));
    connect(tableviewAction, SIGNAL(triggered(bool)), tableView, SLOT(setShown(bool)));

    closeMapAction->setEnabled(false);
}

//--------------------------------------------------------------------------------------
bool MainWindow::connectToDataBase()
{
#if 0
    QSqlDatabase db = QSqlDatabase::database();

    // подключаемся к бд
    db.setHostName(connectionInfo.host);
    db.setPort(connectionInfo.port);
    db.setDatabaseName(connectionInfo.dbname);
    db.setUserName(connectionInfo.user);
    db.setPassword(connectionInfo.password);

    if (!db.open()) {
        QMessageBox box(this);
        box.setText(tr("Ошибка подключения к бд. Проверьте настройки!"));
        box.setIcon(QMessageBox::Critical);
        box.exec();
        return false;
    }

    QSqlQuery query;
    query.exec("set current schema twobyte");
    query.exec("set current path twobyte");

#endif
    return true;
}

// проверка разрешения у пользователя на редактирование карты
//--------------------------------------------------------------------------------------
bool MainWindow::checkUser(const QString &user, const QString &password)
{
#if 0
    QSqlQuery query;

    query.prepare("select id, access "
                  "from hero "
                  "where login = ? and password = ? "
                  "and delinfo_id is null");

    query.addBindValue(user);
    query.addBindValue(password);
    query.exec();
    query.next();

    if (query.isNull(0)) {
        QMessageBox msg;
        msg.setText(tr("Пользователь не найден! Проверьте правильность ввода логина/пароля и повторите попытку."));
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
        return false;
    }

    if (!((query.value(1).toLongLong() >> 44) & 1)) {
        QMessageBox msg;
        msg.setText(tr("У пользователя недостаточно прав для редактирования карты!"));
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
        return false;
        return false;
    }

    userID = query.value(0).toInt();
#endif
    return true;
}

//--------------------------------------------------------------------------------------
bool MainWindow::loadInfoFromDataBase()
{
#if 0
    RMapRegistry::instance().clearAll();

    QSqlQuery query;

    // вытаскиваем id причины удаления
    query.exec("select value_i from options where option_type = 3 and field = 3");
    delReasonID = query.next() ? query.value(0).toInt() : 3;

    // вытаскиваем названия залов
    query.exec("select id, text "
               "from refs "
               "where id_refs = 1 and delinfo_id is null");

    while (query.next()) {
        int refs_id = query.value(0).toInt();
        QString text = query.value(1).toString();
        RMapRegistry::instance().addRefs(refs_id, text);
    }

    // вытаскиваем шрифты
    query.exec("select id, path, height, weight "
               "from font "
               "where delinfo_id is null");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString path = query.value(1).toString();
        int height = query.value(2).toInt();
        int weight = query.value(3).toInt();
        RMapRegistry::instance().addFont(id, path, weight, height);
    }


    // вытаскиваем картинки
    query.exec("select id, path, w, h "
               "from picture "
               "where delinfo_id is null");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString path = query.value(1).toString();
        int width = query.value(2).toInt();
        int height = query.value(3).toInt();
        RMapRegistry::instance().addImage(id, path, width, height);
    }

    QMessageBox box(this);
    box.setIcon(QMessageBox::Critical);

    if (RMapRegistry::instance().refsMap().isEmpty()) {
        box.setText(tr("Список залов пуст! Невозможно открыть карту."));
        box.exec();
        return false;
    }

    if (RMapRegistry::instance().fontsMap().isEmpty()) {
        box.setText(tr("Список шрифтов пуст! Невозможно открыть карту."));
        box.exec();
        return false;
    }

    if (RMapRegistry::instance().imageMap().isEmpty()) {
        box.setText(tr("Список картинок пуст! Невозможно открыть карту."));
        box.exec();
        return false;
    }

#endif
    return true;
}

//--------------------------------------------------------------------------------------
bool MainWindow::loadMapFromDataBase()
{
#if 0
    QMap<int, RSection *> sectionMap;
    QMap<int, RTable *> tableMap;

    QSqlQuery query;

    // вытаскиваем залы
    query.exec("select room.id, refs_id, pict_id, title_font_id, title_rgb, "
               "table_font_id, table_rgb, sel_table_pict_id, x, y, w, h, rect_rgb "
               "from room "
               "where delinfo_id is null");

    while (query.next()) {
        RSection *section = objectFactory->createSection(gridWidget);
        section->model()->setMapObjectID(query.value(0).toInt());
        section->model()->setSectionName(&RMapRegistry::instance().getRefsDescriptorById(query.value(1).toInt()));
        section->model()->setSectionPicture(&RMapRegistry::instance().getImageDescriptorById(query.value(2).toInt()));
        section->model()->setSectionFont(&RMapRegistry::instance().getFontDescriptorById(query.value(3).toInt()));
        section->model()->setSectionFontColor(deserialize_rcolor(query.value(4).toInt()));
        section->model()->setChildFont(&RMapRegistry::instance().getFontDescriptorById(query.value(5).toInt()));
        section->model()->setChildFontColor(deserialize_rcolor(query.value(6).toInt()));
        section->model()->setActiveChildPicture(&RMapRegistry::instance().getImageDescriptorById(query.value(7).toInt()));
        section->model()->setMapObjectGeometry(QRect(query.value(8).toInt(), query.value(9).toInt(), query.value(10).toInt(), query.value(11).toInt()));
        section->model()->setSectionFrameColor(deserialize_rcolor(query.value(12).toInt()));

        treeWidget->insertTopLevelItem(treeWidget->topLevelItemCount(), section->treeWidgetItem());
        section->show();
        sectionMap.insert(query.value(0).toInt(), section);
    }

    // вытаскиваем столики
    query.exec("select id, num, room_id, font_id, rgb_title, x, y "
               "from table "
               "where delinfo_id is null");

    while (query.next()) {
        if (RSection *section = sectionMap.value(query.value(2).toInt())) {
            RTable *table = objectFactory->createTable(section);
            table->model()->setMapObjectID(query.value(0).toInt());
            table->model()->setNumber(query.value(1).toInt());

            table->model()->setNumberFont(&RMapRegistry::instance().getFontDescriptorById(query.value(3).toInt()));
            table->model()->setNumberFontColor(deserialize_rcolor(query.value(4).toInt()));

            // этих полей в бд еще нет, устанавливаем такие же параметры, как и у номера
            table->model()->setTextFont(&RMapRegistry::instance().getFontDescriptorById(query.value(3).toInt()));
            table->model()->setTextFontColor(deserialize_rcolor(query.value(4).toInt()));

            table->model()->setMapObjectGeometry(QRect(query.value(5).toInt(), query.value(6).toInt(), 50, 50));

            table->show();
            tableMap.insert(query.value(0).toInt(), table);
        }
    }

    // вытаскиваем картинки к столикам
    query.exec("select id, table_id, pict_id, type "
               "from tablepict "
               "where delinfo_id is null");

    while (query.next()) {
        if (RTable *table = tableMap.value(query.value(1).toInt())) {
            int tablePictID = query.value(0).toInt();
            int type = query.value(3).toInt();
            const RImageDescriptor *image = &RMapRegistry::instance().getImageDescriptorById(query.value(2).toInt());
            table->model()->setTablePicture(tablePictID, type, image);
        }
    }

    foreach (RSection *section, sectionMap)
        section->model()->setModified(false);

    foreach (RTable *table, tableMap)
        table->model()->setModified(false);

    RMapRegistry::instance().setModified(false);

#endif
    return true;
}

// проверка карты на корректность
// - столики не могут стоять друг на друге
// - залы не могут стоять друг на друге
// - все настройки должны быть заполнены
// - названия залов должны быть уникальны
// - номера столиков должны быть уникальны в пределах одного зала
//--------------------------------------------------------------------------------------
bool MainWindow::checkMapIsValid()
{
    QMessageBox box(this);
    box.setIcon(QMessageBox::Critical);

    QMap<RSection *, QList<RTable *> > m_SectionMap;

    // проверяем, все ли необходимые настройки заполнены
    foreach (QObject *obj, gridWidget->children()) {
        if (RSection *section = qobject_cast<RSection *>(obj)) {
            if (!section->model()->isValid()) {
                section->setFocusMapObject();
                box.setText(tr("Для выделенного объекта заполнены не все обязательные поля!"));
                box.exec();
                return false;
            }
            m_SectionMap.insert(section, QList<RTable *>());

            foreach (QObject *obj2, section->children()) {
                if (RTable *table = qobject_cast<RTable *>(obj2)) {
                    if (!table->model()->isValid()) {
                        table->setFocusMapObject();
                        box.setText(tr("Для выделенного объекта заполнены не все обязательные поля!"));
                        box.exec();
                        return false;
                    }
                    m_SectionMap[section].push_back(table);
                }
            }
        }
    }

    // проверяем, стоят ли залы друг на друге
    // проверяем, уникальны ли названия залов
    QList<RSection *> oldSections;
    foreach (RSection *section, m_SectionMap.keys()) {
        foreach (RSection *obj, oldSections) {
            if (section->geometry().intersects(obj->geometry())) {
                section->setFocusMapObject();
                box.setText(tr("Выделенный зал пересекается с другим залом!"));
                box.exec();
                return false;
            }
            if (section->model()->sectionNameDescriptor()->id() == obj->model()->sectionNameDescriptor()->id()) {
                section->setFocusMapObject();
                box.setText(tr("Зал с таким названием уже существует!"));
                box.exec();
                return false;
            }
        }
        oldSections.push_back(section);
    }

    // проверяем, стоят ли столики друг на друге
    // проверяем, уникальны ли номера столов
    QList<RTable *> oldTables;
    foreach (QList<RTable *> tableList, m_SectionMap.values()) {
        oldTables.clear();
        foreach (RTable *table, tableList) {
            foreach (RTable *obj, oldTables) {
                if (table->geometry().intersects(obj->geometry())) {
                    table->setFocusMapObject();
                    box.setText(tr("Выделенный столик пересекается с другим столиком!"));
                    box.exec();
                    return false;
                }
                if (table->model()->number() == obj->model()->number()) {
                    table->setFocusMapObject();
                    box.setText(tr("Столик с таким номером уже существует!"));
                    box.exec();
                    return false;
                }
            }
            oldTables.push_back(table);
        }
    }

    return true;
}

// сохранение карты в бд
// удаляем объекты, которые были в бд, измененные перезаписываем
//--------------------------------------------------------------------------------------
bool MainWindow::saveMapToDataBase()
{
    if (!gridWidget)
        return false;

    // удаляем из бд удаленные залы
    foreach (int id, RMapRegistry::instance().sectionsForDelete()) {
        if (!deleteSection(id))
            return false;
    }

    // удаляем из бд удаленные столы
    foreach (int id, RMapRegistry::instance().tablesForDelete()) {
        if (!deleteTable(id))
            return false;
    }

    // сохраняем внесенные изменения
    foreach (QObject *obj, gridWidget->children()) {
        if (RSection *section = qobject_cast<RSection *>(obj)) {
            if (section->model()->isModified()) {
                if (!saveSection(section))
                    return false;
            }
            foreach (QObject *obj2, section->children()) {
                if (RTable *table = qobject_cast<RTable *>(obj2)) {
                    if (table->model()->isModified()) {
                        if (!saveTable(section, table))
                            return false;
                    }
                    if (!saveTablePict(table))
                        return false;
                }
            }
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------
bool MainWindow::saveSection(RSection *section)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("CALL SETROOM(?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(section->model()->mapObjectID(), QSql::InOut);
    query.addBindValue(userID);
    query.addBindValue(section->model()->sectionNameDescriptor()->id());
    query.addBindValue(section->model()->sectionPictureDescriptor()->id());
    query.addBindValue(section->model()->sectionFontDescriptor()->id());
    query.addBindValue(serialize_rcolor(section->model()->sectionFontColor()));
    query.addBindValue(section->model()->childFontDescriptor()->id());
    query.addBindValue(serialize_rcolor(section->model()->childFontColor()));
    query.addBindValue(section->model()->activeChildPictureDescriptor()->id());
    query.addBindValue(section->geometry().left());
    query.addBindValue(section->geometry().top());
    query.addBindValue(section->geometry().width());
    query.addBindValue(section->geometry().height());
    query.addBindValue(serialize_rcolor(section->model()->sectionFrameColor()));
    ret = query.exec();
    section->model()->setMapObjectID(query.boundValue(0).toInt());
    return ret;
}

//--------------------------------------------------------------------------------------
bool MainWindow::saveTable(RSection *section, RTable *table)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("CALL SETTABLE(?,?,?,?,?,?,?,?)");
    query.addBindValue(table->model()->mapObjectID(), QSql::InOut);
    query.addBindValue(userID);
    query.addBindValue(table->model()->number());
    query.addBindValue(section->model()->mapObjectID());
    query.addBindValue(table->model()->numberFontDescriptor()->id());
    query.addBindValue(serialize_rcolor(table->model()->numberFontColor()));
    query.addBindValue(table->geometry().left());
    query.addBindValue(table->geometry().top());
    ret = query.exec();
    table->model()->setMapObjectID(query.boundValue(0).toInt());
    return ret;
}

//--------------------------------------------------------------------------------------
bool MainWindow::saveTablePict(RTable *table)
{
    QSqlQuery query;
    for (int i = 0; i < table->model()->tablePicturesCount(); i++) {
        int pict_id = table->model()->tablePicture(i)->id();
        if (table->model()->tablePicturesModify[i] && pict_id) {
            query.prepare("CALL SETTABLEPICT(?,?,?,?,?)");
            query.addBindValue(table->model()->tablePicturesID[i], QSql::InOut);
            query.addBindValue(userID);
            query.addBindValue(table->model()->mapObjectID());
            query.addBindValue(pict_id);
            query.addBindValue(i);
            if (!query.exec())
                return false;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------------
bool MainWindow::deleteSection(int sect_id)
{
    QSqlQuery query;
    query.prepare("CALL DELRECORD(?,?,?,?,?)");
    query.addBindValue(tr("room"));
    query.addBindValue(userID);
    query.addBindValue(sect_id);
    query.addBindValue(delReasonID);
    query.addBindValue(QVariant(0), QSql::Out);
    return query.exec();
}

//--------------------------------------------------------------------------------------
bool MainWindow::deleteTable(int table_id)
{
    QSqlQuery query;
    query.prepare("CALL DELRECORD(?,?,?,?,?)");
    query.addBindValue(tr("table"));
    query.addBindValue(userID);
    query.addBindValue(table_id);
    query.addBindValue(delReasonID);
    query.addBindValue(QVariant(0), QSql::Out);
    return query.exec();
}

//--------------------------------------------------------------------------------------
void MainWindow::focusChanged(RMapObject *obj)
{
	tableView->setModel(obj ? static_cast<QAbstractTableModel *>(obj->model()) : 0);
    tableView->resizeColumnsToContents();
    tableView->resizeRowsToContents();
    createSectionAction->setEnabled(gridWidget);
	createTableAction->setEnabled(obj && qobject_cast<RSection *>(obj));
	deleteAction->setEnabled(obj);
    copyAction->setEnabled(obj);
}

//--------------------------------------------------------------------------------------
void MainWindow::treeWidgetCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    if (current) {
        RMapObjectFocusHolder::instance().setFocusMapObject(static_cast<RMapObjectTreeWidgetItem *>(current)->mapObject());
	} else {
        RMapObjectFocusHolder::instance().setFocusMapObject(0);
		focusChanged(0);
    }
}

//--------------------------------------------------------------------------------------
void MainWindow::onConnectionSettings()
{
    ConnectionDialog dlg(this);
    dlg.setHost(connectionInfo.host);
    dlg.setPort(connectionInfo.port);
    dlg.setDBName(connectionInfo.dbname);
    dlg.setUser(connectionInfo.user);
    dlg.setPassword(connectionInfo.password);

    if (dlg.exec() == QDialog::Accepted) {
        connectionInfo.host = dlg.host();
        connectionInfo.port = dlg.port();
        connectionInfo.dbname = dlg.dbname();
        connectionInfo.user = dlg.user();
        connectionInfo.password = dlg.password();
    }
}

//--------------------------------------------------------------------------------------
void MainWindow::onAbout()
{
    AboutDialog dlg(this);
    dlg.exec();
}

//--------------------------------------------------------------------------------------
void MainWindow::onOpenMap()
{
	NewMapDialog dlg(this);
    dlg.show();

	if (dlg.exec() != QDialog::Accepted)
        return;

    if (!connectToDataBase())
        return;

    if (!checkUser(dlg.user(), dlg.password()))
        return;

    if (!loadInfoFromDataBase())
        return;

    blockSignals(true);
    treeWidget->blockSignals(true);
    tableView->blockSignals(true);

    if (gridWidget) {
		delete gridWidget;
		gridWidget = 0;
		focusChanged(0);
	}

	if (objectFactory)
		delete objectFactory;

    if (dlg.useGrid()) {
        objectFactory = new RMapObjectGridFactory(dlg.gridSize());
	} else {
		objectFactory = new RMapObjectSimpleFactory();
	}

    gridWidget = new RGridWidget(this, dlg.gridSize());
	gridWidget->show();
    gridWidget->setGeometry(0, 0, dlg.mapWidth(), dlg.mapHeight());

    gridWidget->addAction(copyAction);
    gridWidget->addAction(deleteAction);
    gridWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

	scrollArea->setWidget(gridWidget);
	scrollArea->setAlignment(Qt::AlignCenter);
	focusChanged(0);

    blockSignals(false);
    treeWidget->blockSignals(false);
    tableView->blockSignals(false);

    openMapAction->setEnabled(false);
    closeMapAction->setEnabled(true);
    connectionSettingsAction->setEnabled(false);

    connectionInfoLabel->setText(QString(tr("Соединение установлено с %1")).arg(connectionInfo.dbname));

    loadMapFromDataBase();
}

//--------------------------------------------------------------------------------------
bool MainWindow::commitChanges()
{
    if (!RMapRegistry::instance().isModified())
        return true;

    QMessageBox box;
    box.setIcon(QMessageBox::Question);
    box.setText(tr("Сохранить изменения?"));
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Yes, tr("Да"));
    box.setButtonText(QMessageBox::No, tr("Нет"));
    box.setButtonText(QMessageBox::Cancel, tr("Отмена"));
    box.setDefaultButton(QMessageBox::Yes);
    int ret = box.exec();

    if (ret == QMessageBox::Yes) {

        // проверка карты на валидность
        if (!checkMapIsValid())
            return false;

        RCompositeMapObject::instance()->deleteInstance();

        // сохранение карты, используя транзакцию
        QSqlDatabase::database().transaction();
        if (!saveMapToDataBase()) {
            box.setIcon(QMessageBox::Critical);
            box.setText(tr("Ошибка сохранения!"));
            box.setStandardButtons(QMessageBox::Ok);
            box.exec();
            QSqlDatabase::database().rollback();
            return false;
        } else {
            QSqlDatabase::database().commit();
            return true;
        }
    }

    return ret == QMessageBox::No;
}

//--------------------------------------------------------------------------------------
void MainWindow::onCloseMap()
{
    if (!commitChanges())
        return;

    blockSignals(true);
    treeWidget->blockSignals(true);
    tableView->blockSignals(true);

    if (gridWidget) {
        delete gridWidget;
        gridWidget = 0;
    }

    if (objectFactory) {
        delete objectFactory;
        objectFactory = 0;
    }

    blockSignals(false);
    treeWidget->blockSignals(false);
    tableView->blockSignals(false);

    openMapAction->setEnabled(true);
    closeMapAction->setEnabled(false);
    connectionSettingsAction->setEnabled(true);

    QSqlDatabase::database().close();
    connectionInfoLabel->setText(tr("Нет соединения"));

    focusChanged(0);
    userID = 0;
}

//--------------------------------------------------------------------------------------
void MainWindow::onCreateSection()
{
	RSection *section = objectFactory->createSection(gridWidget);
    section->show();
    treeWidget->insertTopLevelItem(treeWidget->topLevelItemCount(), section->treeWidgetItem());
}

//--------------------------------------------------------------------------------------
void MainWindow::onCreateTable()
{	
    if (RSection *section = qobject_cast<RSection *>(RMapObjectFocusHolder::instance().getFocusMapObject())) {
		RTable *table = objectFactory->createTable(section);
        table->model()->setNumberFont(section->model()->childFontDescriptor());
        table->model()->setNumberFontColor(section->model()->childFontColor());
        table->model()->setTextFont(section->model()->childFontDescriptor());
        table->model()->setTextFontColor(section->model()->childFontColor());
		table->show();
	}
}

//--------------------------------------------------------------------------------------
void MainWindow::onCopyMapObject()
{
    if (RMapObject *obj = RMapObjectFocusHolder::instance().getFocusMapObject()) {
		RMapObject *newObj = obj->clone();
        newObj->move(QPoint(0, 0));
	}
}

//--------------------------------------------------------------------------------------
void MainWindow::onDeleteMapObject()
{
    if (RMapObject *obj = RMapObjectFocusHolder::instance().getFocusMapObject()) {
        QMessageBox box;
        box.setIcon(QMessageBox::Question);
        box.setText(tr("Вы действительно ходите удалить выбранный элемент?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setButtonText(QMessageBox::Yes, tr("Да"));
        box.setButtonText(QMessageBox::No, tr("Нет"));
        if (box.exec() == QMessageBox::Yes) {
            delete obj;
        }
	}
}
