#include "ui/mainwindow.h"
#include <QApplication>

#include "db/db.h"
#include <QSqlDataBase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database database;
    if(database.connect())
    {
        qDebug() << "Everything is OK.";
    }
    else
    {
        qDebug() << "Check your PostgreSQL settings.";
    }

    MainWindow w;
    w.show();
    return QApplication::exec();
}
