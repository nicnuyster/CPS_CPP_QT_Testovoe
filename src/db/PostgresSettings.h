// DBSettings.h
#ifndef POSTGRESSETTINGS_H
#define POSTGRESSETTINGS_H

#include <QString>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>

class PostgresSettings
{
public:
    PostgresSettings();

    // db server
    QString serverName() const;
    void setServerName(const QString &name);
    
    QString host() const;
    void setHost(const QString &host);
    
    int port() const;
    void setPort(int port);
    
    QString userName() const;
    void setUserName(const QString &user);
    
    QString password() const;
    void setPassword(const QString &pass);
    // db db
    QString databaseName() const;
    void setDatabaseName(const QString &dbName);

    // db path
    QString pgBinDirectory() const;       //pg_ctl.exe, initdb.exe
    void setPgBinDirectory(const QString &path);

    QString dataDirectory() const;        // PostgreSQL data cluster
    void setDataDirectory(const QString &path);

    QString logFile() const;              // server log
    void setLogFile(const QString &path);

    // qtSettings
    void load(const QString &organization, const QString &application);
    void save(const QString &organization, const QString &application) const;

private:
    QString m_serverName;
    QString m_host;         
    int m_port;
    QString m_userName;
    QString m_password;
    QString m_databaseName;

    // Paths
    QString m_pgBinDir;
    QString m_dataDir;
    QString m_logFile;
};

#endif // POSTGRESSETTINGS_H