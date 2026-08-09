#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include "PostgresSettings.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(const PostgresSettings &settings, QObject *parent = nullptr);
    ~DatabaseManager();

    bool open();                     // open the connection (lazy initialisation)
    void close();                    // close and remove connection
    bool isOpen() const;
    QString lastError() const;

    QSqlDatabase database() const;   // access the underlying QSqlDatabase for queries

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);

private:
    void ensureConnectionConfigured(); // set up connection parameters once

    const PostgresSettings &pgSettings;
    QString pgConnectionName;
    bool pgConfigured = false;
};

#endif // DATABASEMANAGER_H