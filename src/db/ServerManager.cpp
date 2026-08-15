#include "ServerManager.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QThread>

ServerManager::ServerManager(const PostgresSettings &settings, QObject *parent)
    : QObject(parent)
    , pgSettings(settings)
    , pgProcess(nullptr)
{
}

QString ServerManager::pgCtlPath() const
{
    return QDir(pgSettings.pgBinDirectory()).filePath("pg_ctl.exe");
}

QString ServerManager::initDbPath() const
{
    return QDir(pgSettings.pgBinDirectory()).filePath("initdb.exe");
}

bool ServerManager::initDatabaseCluster()
{
    QDir dataDir(pgSettings.dataDirectory());

    QDir().mkpath(pgSettings.dataDirectory());

    qDebug() << "[ServerManager] pgBinDir:" << pgSettings.pgBinDirectory();
    qDebug() << "[ServerManager] initdb exists?" << QFile::exists(initDbPath());

    QString versionFile = dataDir.filePath("PG_VERSION");
    if (QFile::exists(versionFile)) {
        emit logMessage("Data directory already initialized (PG_VERSION found). Skipping initdb.");
        emit initDbFinished(true, "Already initialized.");
        return true;
    }

    //
    
    if (pgProcess) pgProcess->deleteLater();
    pgProcess = new QProcess(this);
    connect(pgProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &ServerManager::onInitDbFinished);
    connect(pgProcess, &QProcess::errorOccurred, this, &ServerManager::onProcessError);
    connect(pgProcess, &QProcess::readyReadStandardOutput, this, &ServerManager::readOutput);
    connect(pgProcess, &QProcess::readyReadStandardError, this, &ServerManager::readOutput);

    QStringList args;
    args << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory());
    args << "--auth=trust";
    args << "--encoding=UTF8";
    args << "-U" << pgSettings.userName();

    QString cmd = initDbPath();
    QString argsStr = args.join(' ');
    emit logMessage("Initializing database cluster...");
    emit logMessage(QString("Command: %1 %2").arg(cmd, argsStr));
    qDebug() << "Full command:" << cmd << argsStr;
    pgProcess->start(cmd, args);

return true;
}

void ServerManager::start()
{
    QString postgresPath = QDir(pgSettings.pgBinDirectory()).filePath("postgres.exe");

    QStringList args;
    args << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory());
    args << "-p" << QString::number(pgSettings.port());
    args << "-h" << pgSettings.host();

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", pgSettings.pgBinDirectory() + ";" + env.value("PATH"));
    // postgres.exe doesn't need PGPASSWORD, but keep it for consistency
    env.insert("PGPASSWORD", pgSettings.password());

    qDebug() << "[ServerManager] Starting postgres.exe:" << postgresPath << args;

    qint64 pid = 0;
    bool ok = QProcess::startDetached(postgresPath, args, pgSettings.pgBinDirectory(), &pid);

    if (!ok) {
        emit logMessage("Failed to start postgres.exe");
        emit startFailed("QProcess::startDetached returned false.");
        return;
    }

    qDebug() << "[ServerManager] postgres.exe detached, PID:" << pid;

    // Poll for server readiness (max 10 seconds)
    for (int i = 0; i < 20; ++i) {
        QThread::msleep(500);
        if (isRunning()) {
            emit logMessage("Server started successfully.");
            emit serverStarted();
            return;
        }
    }

    QString err = "postgres.exe started but did not become ready within 10 seconds. Check server.log.";
    emit logMessage(err);
    emit startFailed(err);
}

void ServerManager::stop()
{
    QProcess proc;
    proc.setWorkingDirectory(pgSettings.pgBinDirectory());
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", pgSettings.pgBinDirectory() + ";" + env.value("PATH"));
    proc.setProcessEnvironment(env);

    QStringList args;
    args << "stop"
         << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory())
         << "-m" << "fast";

    emit logMessage("Stopping PostgreSQL server...");
    proc.start(pgCtlPath(), args);

    if (!proc.waitForStarted(5000)) {
        emit logMessage("Failed to start pg_ctl stop: " + proc.errorString());
        emit stopFailed(proc.errorString());
        return;
    }

    if (!proc.waitForFinished(15000)) {
        proc.kill();
        proc.waitForFinished(3000);
    }

    QString output = QString::fromLocal8Bit(proc.readAllStandardOutput() + proc.readAllStandardError());
    if (proc.exitCode() == 0) {
        emit logMessage("Server stopped.");
        emit serverStopped();
    } else {
        emit logMessage("Failed to stop server:\n" + output);
        emit stopFailed(output);
    }
}

void ServerManager::stopImmediate()
{
    QProcess proc;
    proc.setWorkingDirectory(pgSettings.pgBinDirectory());
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", pgSettings.pgBinDirectory() + ";" + env.value("PATH"));
    proc.setProcessEnvironment(env);

    QStringList args;
    args << "stop"
         << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory())
         << "-m" << "immediate";

    proc.start(pgCtlPath(), args);
    proc.waitForFinished(10000);
}

bool ServerManager::isRunning()
{
    QProcess proc;
    proc.setWorkingDirectory(pgSettings.pgBinDirectory());
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", pgSettings.pgBinDirectory() + ";" + env.value("PATH"));
    proc.setProcessEnvironment(env);

    QStringList args;
    args << "status" << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory());

    proc.start(pgCtlPath(), args);
    proc.waitForFinished(5000);
    return (proc.exitCode() == 0);
}

// -------- private ----------
void ServerManager::onStartFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString output = pgProcess->readAllStandardOutput() + pgProcess->readAllStandardError();
    
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        qDebug() << "[ServerManager] pg_ctl start returned (no-wait mode).";
        
        // Poll for server readiness (max 10 seconds)
        for (int i = 0; i < 20; ++i) {
            QThread::msleep(500);
            if (isRunning()) {
                emit logMessage("Server started successfully.");
                emit serverStarted();
                return;
            }
        }
        
        QString err = "Server process did not become ready within 10 seconds.";
        emit logMessage(err + "\n" + output);
        emit startFailed(err);
    } else {
        emit logMessage("Failed to start server:\n" + output);
        emit startFailed(output);
    }
}

void ServerManager::onStopFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString output = pgProcess->readAllStandardOutput() + pgProcess->readAllStandardError();
    bool success = (exitStatus == QProcess::NormalExit && exitCode == 0);
    
    if (success) {
        // Force trust auth for all local TCP connections (fixes first-run pg_dumpall)
        QString hbaPath = QDir(pgSettings.dataDirectory()).filePath("pg_hba.conf");
        QFile hbaFile(hbaPath);
        if (hbaFile.open(QIODevice::Append | QIODevice::Text)) {
            hbaFile.write("\n# Added by app for pg_dumpall compatibility\n");
            hbaFile.write("host  all  all  127.0.0.1/32  trust\n");
            hbaFile.write("host  all  all  ::1/128       trust\n");
            hbaFile.close();
            qDebug() << "[ServerManager] Appended trust rules to pg_hba.conf";
        }
        emit logMessage("Database cluster created.");
    } else {
        emit logMessage("initdb failed:\n" + output);
    }
    emit initDbFinished(success, output);
}

void ServerManager::onInitDbFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString output = pgProcess->readAllStandardOutput() + pgProcess->readAllStandardError();
    bool success = (exitStatus == QProcess::NormalExit && exitCode == 0);
    
    if (success) {
        // Replace pg_hba.conf with trust-first rules (prepend, don't append)
        QString hbaPath = QDir(pgSettings.dataDirectory()).filePath("pg_hba.conf");
        
        // Read existing content
        QFile hbaFile(hbaPath);
        QByteArray originalContent;
        if (hbaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            originalContent = hbaFile.readAll();
            hbaFile.close();
        }
        
        // Write new content: trust rules FIRST, then original content
        if (hbaFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            hbaFile.write("# Trust rules for local TCP connections (app-managed)\n");
            hbaFile.write("host  all  all  127.0.0.1/32  trust\n");
            hbaFile.write("host  all  all  ::1/128       trust\n");
            hbaFile.write("\n");
            hbaFile.write("# --- original pg_hba.conf below ---\n");
            hbaFile.write(originalContent);
            hbaFile.close();
            qDebug() << "[ServerManager] Prepended trust rules to pg_hba.conf";
        }
        emit logMessage("Database cluster created.");
    } else {
        emit logMessage("initdb failed:\n" + output);
    }
    emit initDbFinished(success, output);
}

void ServerManager::onProcessError(QProcess::ProcessError error)
{
    Q_UNUSED(error)
    QString msg = "Process error: " + pgProcess->errorString();
    emit logMessage(msg);
    emit initDbFinished(false, msg);
}

void ServerManager::readOutput()
{
    QProcess *proc = qobject_cast<QProcess*>(sender());
    if (proc)
        emit logMessage(proc->readAll());
}

void ServerManager::stopBlocking()
{
    QProcess proc;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PGPASSWORD", pgSettings.password());
    proc.setProcessEnvironment(env);
    proc.setWorkingDirectory(pgSettings.pgBinDirectory());

    QStringList args;
    args << "stop"
         << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory())
         << "-m" << "fast";

    qDebug() << "[ServerManager] Stopping server:" << pgCtlPath() << args;
    proc.start(pgCtlPath(), args);
    
    if (!proc.waitForStarted(5000)) {
        qWarning() << "[ServerManager] Failed to start pg_ctl stop:" << proc.errorString();
        return;
    }
    
    if (!proc.waitForFinished(15000)) {
        qWarning() << "[ServerManager] pg_ctl stop timed out, killing...";
        proc.kill();
        proc.waitForFinished(3000);
    }
    
    QString output = QString::fromLocal8Bit(proc.readAllStandardOutput() + proc.readAllStandardError());
    qDebug() << "[ServerManager] pg_ctl stop output:" << output;
}