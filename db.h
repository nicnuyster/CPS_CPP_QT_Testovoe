#ifndef DB_H
#define DB_H

#include <QtSql/QSqlDatabase>

class Database
{
public:
    Database();

    bool connect();

private:
    QSqlDatabase db;
};

#endif // DB_H