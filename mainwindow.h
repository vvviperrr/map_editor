#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSplitter;
class QScrollArea;
class QAction;
class QToolBar;
class QTableView;
class QTreeWidget;
class QTreeWidgetItem;
class QLabel;

class RMapObject;
class RGridWidget;
class RMapObjectFactory;
class RSection;
class RTable;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	void focusChanged(RMapObject *obj);

    void onOpenMap();
    void onCloseMap();
	void onCreateSection();
	void onCreateTable();
	void onCopyMapObject();
	void onDeleteMapObject();
    void onConnectionSettings();
    void onAbout();

    void treeWidgetCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

protected:
	void closeEvent(QCloseEvent *event);

private:
	void initWidgets();
	void loadConfig();
	void createToolBars();

    bool connectToDataBase();
    bool loadInfoFromDataBase();
    bool loadMapFromDataBase();
    bool checkUser(const QString &user, const QString &password);

    bool checkMapIsValid();
    bool saveMapToDataBase();
    bool commitChanges();

    bool saveSection(RSection *section);
    bool saveTable(RSection *section, RTable *table);
    bool saveTablePict(RTable *table);

    bool deleteSection(int sect_id);
    bool deleteTable(int table_id);

	RGridWidget *gridWidget;

    QTreeWidget *treeWidget;
	QTableView	*tableView;

	RMapObjectFactory *objectFactory;

	QSplitter *splitter1;
	QSplitter *splitter2;
	QScrollArea *scrollArea;

    QAction *openMapAction;
    QAction *closeMapAction;
    QAction *createSectionAction;
    QAction *createTableAction;
    QAction *copyAction;
    QAction *deleteAction;
    QAction *connectionSettingsAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *aboutQtAction;

    QAction *toolbarAction;
    QAction *statusbarAction;
    QAction *treeviewAction;
    QAction *tableviewAction;

    QToolBar *toolBar;

    QLabel *connectionInfoLabel;

    struct {
        QString host;
        int port;
        QString dbname;
        QString user;
        QString password;
    } connectionInfo;

    int userID;
    int delReasonID;
};

#endif // MAINWINDOW_H
