#include "ServerManager.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

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
    if (dataDir.exists() && !dataDir.entryList(QDir::NoDotAndDotDot).isEmpty()) {
        emit logMessage("Data directory already initialized. Skipping initdb.");
        emit initDbFinished(true, "Already exists.");
        return true;
    }

    QDir().mkpath(pgSettings.dataDirectory());   // ensure parent exists

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

//    emit logMessage("Initializing database cluster...");
//    pgProcess->start(initDbPath(), args);

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
    if (pgProcess) pgProcess->deleteLater();
    pgProcess = new QProcess(this);
    connect(pgProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &ServerManager::onStartFinished);
    connect(pgProcess, &QProcess::errorOccurred, this, &ServerManager::onProcessError);
    connect(pgProcess, &QProcess::readyReadStandardOutput, this, &ServerManager::readOutput);
    connect(pgProcess, &QProcess::readyReadStandardError, this, &ServerManager::readOutput);

    QStringList args;
    args << "start" << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory());
    args << "-l" << QDir::toNativeSeparators(pgSettings.logFile());
    args << "-o" << QString("-p %1").arg(pgSettings.port());

    emit logMessage("Starting PostgreSQL server...");
    pgProcess->start(pgCtlPath(), args);
}

void ServerManager::stop()
{
    if (pgProcess) pgProcess->deleteLater();
    pgProcess = new QProcess(this);
    connect(pgProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &ServerManager::onStopFinished);
    connect(pgProcess, &QProcess::errorOccurred, this, &ServerManager::onProcessError);

    QStringList args;
    args << "stop" << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory());
    args << "-m" << "fast";

    emit logMessage("Stopping PostgreSQL server...");
    pgProcess->start(pgCtlPath(), args);
}

void ServerManager::stopImmediate()
{
    if (pgProcess) pgProcess->deleteLater();
    pgProcess = new QProcess(this);
    connect(pgProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &ServerManager::onStopFinished);
    connect(pgProcess, &QProcess::errorOccurred, this, &ServerManager::onProcessError);

    QStringList args;
    args << "stop" << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory());
    args << "-m" << "immediate";
    pgProcess->start(pgCtlPath(), args);
}

bool ServerManager::isRunning()
{
    QProcess proc;
    QStringList args;
    args << "status" << "-D" << QDir::toNativeSeparators(pgSettings.dataDirectory());
    proc.start(pgCtlPath(), args);
    proc.waitForFinished(5000);
    return (proc.exitCode() == 0);
}

// -------- private slots ----------
void ServerManager::onStartFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString output = pgProcess->readAllStandardOutput() + pgProcess->readAllStandardError();
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        emit logMessage("Server started successfully.");
        emit serverStarted();
    } else {
        emit logMessage("Failed to start server:\n" + output);
        emit startFailed(output);
    }
}

void ServerManager::onStopFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString output = pgProcess->readAllStandardOutput() + pgProcess->readAllStandardError();
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        emit logMessage("Server stopped.");
        emit serverStopped();
    } else {
        emit logMessage("Failed to stop server:\n" + output);
        emit stopFailed(output);
    }
}

void ServerManager::onInitDbFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString output = pgProcess->readAllStandardOutput() + pgProcess->readAllStandardError();
    bool success = (exitStatus == QProcess::NormalExit && exitCode == 0);
    emit logMessage(success ? "Database cluster created." : "initdb failed:\n" + output);
    emit initDbFinished(success, output);
}

void ServerManager::onProcessError(QProcess::ProcessError error)
{
    Q_UNUSED(error)
    emit logMessage("Process error: " + pgProcess->errorString());
}

void ServerManager::readOutput()
{
    QProcess *proc = qobject_cast<QProcess*>(sender());
    if (proc)
        emit logMessage(proc->readAll());
}