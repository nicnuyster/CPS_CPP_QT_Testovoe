#include "AppExmpl.h"

#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTime>

AppExmpl::AppExmpl(DatabaseManager *dbManager, const PostgresSettings &settings)
    : m_dbManager(dbManager)
    , m_pgSettings(settings)
{
}

bool AppExmpl::applyMigrations(QString &errorMessage) const
{
    if (!m_dbManager || !m_dbManager->database().isOpen()) {
        errorMessage = QStringLiteral("Database not open.");
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    QString migrationsDir = QDir(QCoreApplication::applicationDirPath() + "/../migrations").absolutePath();
    QDir dir(migrationsDir);
    QStringList files = dir.entryList(QStringList() << "*.up.sql", QDir::Files, QDir::Name);

    for (const QString &fileName : files) {
        QFile file(dir.absoluteFilePath(fileName));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            errorMessage = QStringLiteral("Cannot open %1").arg(fileName);
            return false;
        }
        QString sql = QString::fromUtf8(file.readAll());
        file.close();

        QSqlQuery query(db);
        if (!query.exec(sql)) {
            errorMessage = QStringLiteral("Migration %1 failed: %2").arg(fileName, query.lastError().text());
            return false;
        }
        qDebug() << "Applied migration:" << fileName;
    }

    errorMessage.clear();
    return true;
}

bool AppExmpl::seedAppointments(QString &errorMessage) const
{
    SpecialityRepo       specialityRepo(m_dbManager);
    DoctorRepo           doctorRepo(m_dbManager);
    PatientRepo          patientRepo(m_dbManager);
    DoctorSpecialityRepo doctorSpecialityRepo(m_dbManager);
    AppointmentsRepo     appointmentsRepo(m_dbManager);

    if (!m_dbManager) {
        errorMessage = QStringLiteral("AppExmpl: DatabaseManager is null.");
        qWarning() << errorMessage;
        return false;
    }

    QSqlDatabase db = m_dbManager->database();
    if (!db.isOpen()) {
        errorMessage = QStringLiteral("AppExmpl: database connection is not open.");
        qWarning() << errorMessage;
        return false;
    }

    QVector<Patient> patients;
    if (!patientRepo.listAll(patients, errorMessage)) {
        return false;
    }

    QVector<Doctor> doctor;
    if (!doctorRepo.listAll(doctor, errorMessage)) {
        return false;
    }

    QVector<DoctorSpeciality> doctorSpeciality;
    if (!doctorSpecialityRepo.listAll(doctorSpeciality, errorMessage)) {
        return false;
    }

    AppointmentsService service(m_dbManager);

    QDate  baseDate = QDate::currentDate().addDays(1);
    QTime  startTime(9, 0);

    if (patients.isEmpty()) {
        errorMessage = "AppExmpl: no patients to seed appointments.";
        qWarning() << errorMessage;
        return false;
    }
    if (doctor.isEmpty()) {
        errorMessage = "AppExmpl: no doctors to seed appointments.";
        qWarning() << errorMessage;
        return false;
    }
    if (doctorSpeciality.isEmpty()) {
        errorMessage = "AppExmpl: no doctor_specialties to seed appointments.";
        qWarning() << errorMessage;
        return false;
    }

    int createdCount = 0;
    for (int i = 0; i < 5; ++i) {
        Appointments appt;
        appt.patient_id          = patients.at(i % patients.size()).id;
        appt.doctor_id           = doctor.at(i % doctor.size()).id;
        appt.doctor_speciality_id = doctorSpeciality.at(i % doctorSpeciality.size()).id;
        appt.start_datetime      = QDateTime(baseDate.addDays(i), startTime);
        appt.status              = TableEnums::AppointmentStatus::Confirmed;

        int outId = 0;
        QString apptError;
        if (!service.createAppointment(appt, outId, apptError)) {
            qWarning() << "AppExmpl: appointment" << (i + 1) << "failed:" << apptError;
        } else {
            ++createdCount;
            qDebug() << "AppExmpl: created appointment id=" << outId;
        }
    }

    if (createdCount == 0) {
        errorMessage = QStringLiteral("AppExmpl: failed to create any appointments.");
        qWarning() << errorMessage;
        return false;
    }

    if (createdCount > 0) {
        Appointments appt;
        appt.patient_id          = patients.at(4 % patients.size()).id;
        appt.doctor_id           = doctor.at(4 % doctor.size()).id;
        appt.doctor_speciality_id = doctorSpeciality.at(4 % doctorSpeciality.size()).id;
        appt.start_datetime      = QDateTime(baseDate.addDays(4), startTime);
        appt.status              = TableEnums::AppointmentStatus::Confirmed;

        int outId = 0;
        QString apptError;
        if (!service.createAppointment(appt, outId, apptError)) {
            qWarning() << "AppExmpl: appointment" << (4 + 1) << "failed:" << apptError;
        } else {
            ++createdCount;
            qDebug() << "AppExmpl: created appointment id=" << outId;
        }        
    }

    qDebug() << "AppExmpl: created" << createdCount << "out of 6 appointments.";
    errorMessage.clear();
    return true;
}

bool AppExmpl::pgDumpAll(QString *errorMessage)
{
    // Use pg_dump instead of pg_dumpall (no template1 needed)
    QString pgDumpPath = QDir(m_pgSettings.pgBinDirectory()).filePath("pg_dump.exe");

    if (!QFile::exists(pgDumpPath)) {
        QString err = QString("pg_dump.exe not found at: %1").arg(pgDumpPath);
        qWarning() << err;
        if (errorMessage) *errorMessage = err;
        return false;
    }

    QString appDir = QCoreApplication::applicationDirPath();
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString dumpFileName = QString("backup_%1.sql").arg(timestamp);
    QString dumpFilePath = QDir(appDir).filePath(dumpFileName);

    QProcess proc;
    proc.setWorkingDirectory(m_pgSettings.pgBinDirectory());
    
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString path = env.value("PATH");
    env.insert("PATH", m_pgSettings.pgBinDirectory() + ";" + path);
    env.insert("PGPASSWORD", m_pgSettings.password());
    proc.setProcessEnvironment(env);

    // pg_dump args: specific database, no template1 needed
    QStringList args;
    args << "-h" << m_pgSettings.host()
         << "-p" << QString::number(m_pgSettings.port())
         << "-U" << m_pgSettings.userName()
         << "-d" << m_pgSettings.databaseName()
         << "-F" << "p";  // plain SQL format

    qDebug() << "[pgDump] exe :" << pgDumpPath;
    qDebug() << "[pgDump] args:" << args;
    qDebug() << "[pgDump] out :" << dumpFilePath;

    proc.start(pgDumpPath, args);

    if (!proc.waitForStarted(10000)) {
        QString err = QString("Failed to start pg_dump: %1").arg(proc.errorString());
        qWarning() << err;
        if (errorMessage) *errorMessage = err;
        return false;
    }

    QByteArray stdoutData;
    QByteArray stderrData;
    
    while (proc.state() == QProcess::Running) {
        proc.waitForReadyRead(1000);
        stdoutData += proc.readAllStandardOutput();
        stderrData += proc.readAllStandardError();
    }
    proc.waitForFinished();
    stdoutData += proc.readAllStandardOutput();
    stderrData += proc.readAllStandardError();

    // Write output
    bool fileOk = false;
    if (!stdoutData.isEmpty()) {
        QFile outFile(dumpFilePath);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            outFile.write(stdoutData);
            outFile.close();
            fileOk = true;
        }
    }

    QString errFilePath = dumpFilePath + ".err";
    if (!stderrData.isEmpty()) {
        QFile errFile(errFilePath);
        if (errFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            errFile.write(stderrData);
            errFile.close();
        }
    } else {
        QFile::remove(errFilePath);
    }

    int exitCode = proc.exitCode();
    QProcess::ExitStatus exitStatus = proc.exitStatus();
    QString stderrText = QString::fromLocal8Bit(stderrData);

    qDebug() << "[pgDump] exit code :" << exitCode;
    qDebug() << "[pgDump] exit status:" << (exitStatus == QProcess::NormalExit ? "normal" : "crashed");
    qDebug() << "[pgDump] stderr    :" << stderrText;
    qDebug() << "[pgDump] stdout len:" << stdoutData.size();

    if (exitStatus == QProcess::CrashExit) {
        QString err = "pg_dump crashed. Missing DLLs?";
        if (errorMessage) *errorMessage = err;
        return false;
    }

    if (exitCode != 0) {
        QString err = QString("pg_dump failed (exit=%1). Stderr: %2")
                          .arg(exitCode)
                          .arg(stderrText.isEmpty() ? "<empty>" : stderrText);
        qWarning() << err;
        if (errorMessage) *errorMessage = err;
        return false;
    }

    if (!fileOk || stdoutData.isEmpty()) {
        QString err = "pg_dump succeeded but produced no output.";
        if (errorMessage) *errorMessage = err;
        return false;
    }

    qDebug() << "[pgDump] SUCCESS:" << dumpFilePath
             << "Size:" << QFileInfo(dumpFilePath).size() << "bytes";
    return true;
}