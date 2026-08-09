#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QProcess>
#include "PostgresSettings.h"

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(const PostgresSettings &settings, QObject *parent = nullptr);

    bool initDatabaseCluster();
    void start();
    void stop();
    void stopImmediate();
    bool isRunning();

signals:
    void serverStarted();
    void serverStopped();
    void startFailed(const QString &error);
    void stopFailed(const QString &error);
    void initDbFinished(bool success, const QString &output);
    void logMessage(const QString &message);

private slots:
    void onStartFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onStopFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onInitDbFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void readOutput();

private:
    QString pgCtlPath() const;
    QString initDbPath() const;

    const PostgresSettings &pgSettings;
    QProcess *pgProcess;
};

#endif // SERVERMANAGER_H