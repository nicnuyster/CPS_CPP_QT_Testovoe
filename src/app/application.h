#ifndef APPLICATION_H
#define APPLICATION_H

/* ---  QT  --- */
#include <QApplication>
#include <QMainWindow>

/* ---  src   ---*/
#include "../ui/mainwindow.h"
#include "../db/DatabaseManager.h"
#include "../db/ServerManager.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);

private:
    MainWindow mainWindow;
    ServerManager *pgServerManager;
    DatabaseManager *pgDatabaseManager;
};

#endif // APPLICATION_H