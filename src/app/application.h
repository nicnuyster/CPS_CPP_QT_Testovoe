#ifndef APPLICATION_H
#define APPLICATION_H

/* ---  QT  --- */
#include <QApplication>
#include <QMainWindow>

/* ---  src   ---*/
#include "../ui/mainwindow.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);    /*создание*/
    ~Application();                         /*удаление*/

private:
    MainWindow *mainWindow;
};

#endif // APPLICATION_H