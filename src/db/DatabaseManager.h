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

    bool open();                     
    void close();                    
    bool isOpen() const;
    QString lastError() const;

    QSqlDatabase database() const;   

    // acid
    bool beginTransaction();
    bool commit();
    bool rollback();

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);

private:
    void ensureConnectionConfigured(); 

    const PostgresSettings &pgSettings;
    QString pgConnectionName;
    bool pgConfigured = false;
};

#endif // DATABASEMANAGER_H