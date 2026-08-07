#include "db.h"

#include <QDebug>
#include <QtSql/QSqlError>


Database::Database()
{
    db = QSqlDatabase::addDatabase("QPSQL");

    db.setHostName("127.0.0.1");   // PostgreSQL server address
    db.setPort(5432);              // Default PostgreSQL port
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("admin");
}


bool Database::connect()
{
    if (!db.open())
    {
        qDebug() << "Database connection failed!";
        qDebug() << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully!";
    return true;
}