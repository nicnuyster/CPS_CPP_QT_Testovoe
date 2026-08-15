#include "PostgresSettings.h"

PostgresSettings::PostgresSettings()
    : m_serverName("Local_qt_serv")
    , m_host("127.0.0.1")
    , m_port(6321)
    , m_userName("postgres")
    , m_password("admin")
    , m_databaseName("local_qt_db")
{
    QString appDir = QCoreApplication::applicationDirPath();

    // Try multiple candidate paths for pgsql/bin
    QStringList candidates;
    candidates << QDir(appDir).filePath("../../pgsql/bin");   // shadow build
    candidates << QDir(appDir).filePath("../pgsql/bin");      // flat build
    candidates << QDir(appDir).filePath("pgsql/bin");         // same dir
    candidates << "C:/Program Files/PostgreSQL/9.6/bin";      // system install

    for (const QString &path : candidates) {
        QString cleanPath = QDir::cleanPath(path);
        if (QFile::exists(QDir(cleanPath).filePath("initdb.exe"))) {
            m_pgBinDir = cleanPath;
            break;
        }
    }

    // Fallback to first candidate if none found (will fail loudly later)
    if (m_pgBinDir.isEmpty()) {
        m_pgBinDir = QDir::cleanPath(candidates.first());
    }

    m_dataDir  = QDir(appDir).filePath("pgdata");
    m_logFile  = QDir(m_dataDir).filePath("server.log");
}

// db server
QString PostgresSettings::serverName() const { return m_serverName; }
void PostgresSettings::setServerName(const QString &name) { m_serverName = name; }

QString PostgresSettings::host() const { return m_host; }
void PostgresSettings::setHost(const QString &host) { m_host = host; }

int PostgresSettings::port() const { return m_port; }
void PostgresSettings::setPort(int port) { m_port = port; }

QString PostgresSettings::userName() const { return m_userName; }
void PostgresSettings::setUserName(const QString &user) { m_userName = user; }

QString PostgresSettings::password() const { return m_password; }
void PostgresSettings::setPassword(const QString &pass) { m_password = pass; }
// db db
QString PostgresSettings::databaseName() const { return m_databaseName; }
void PostgresSettings::setDatabaseName(const QString &dbName) { m_databaseName = dbName; }

// db path
QString PostgresSettings::pgBinDirectory() const { return m_pgBinDir; }
void PostgresSettings::setPgBinDirectory(const QString &path) { m_pgBinDir = path; }

QString PostgresSettings::dataDirectory() const  { return m_dataDir; }
void PostgresSettings::setDataDirectory(const QString &path)  { m_dataDir = path; }

QString PostgresSettings::logFile() const        { return m_logFile; }
void PostgresSettings::setLogFile(const QString &path)        { m_logFile = path; }

//
void PostgresSettings::load(const QString &organization, const QString &application)
{
    QSettings settings(organization, application);
    settings.beginGroup("Database");
    m_serverName   = settings.value("serverName", "Local_qt_serv").toString();
    m_host         = settings.value("host", "127.0.0.1").toString();
    m_port         = settings.value("port", 6321).toInt();
    m_userName     = settings.value("user", "postgres").toString();
    m_password     = settings.value("password", "admin").toString();
    m_databaseName = settings.value("database", "local_qt_db").toString();
    settings.endGroup();
}

void PostgresSettings::save(const QString &organization, const QString &application) const
{
    QSettings settings(organization, application);

    settings.beginGroup("Database");
    settings.setValue("serverName", m_serverName);
    settings.setValue("host", m_host);
    settings.setValue("port", m_port);
    settings.setValue("user", m_userName);
    settings.setValue("password", m_password);
    settings.setValue("database", m_databaseName);
    settings.endGroup();

    settings.beginGroup("ServerPaths");
    settings.setValue("pgBinDir", m_pgBinDir);
    settings.setValue("dataDir", m_dataDir);
    settings.setValue("logFile", m_logFile);
    settings.endGroup();
}