#include "AppExmpl.h"

#include <QSqlDatabase>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QProcess>

AppExmpl::AppExmpl(DatabaseManager *dbManager)
    : m_dbManager(dbManager)
{
}

bool AppExmpl::applyMigrations(QString &errorMessage) const
{
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

    SpecialityRepo       specialityRepo(m_dbManager);
    DoctorRepo           doctorRepo(m_dbManager);
    PatientRepo          patientRepo(m_dbManager);
    DoctorSpecialityRepo doctorSpecialityRepo(m_dbManager);
    AppointmentsRepo     appointmentsRepo(m_dbManager);

    if (!specialityRepo.createTable(errorMessage)) {
        qWarning() << "AppExmpl: specialties table failed:" << errorMessage;
        return false;
    }
    if (!doctorRepo.createTable(errorMessage)) {
        qWarning() << "AppExmpl: doctors table failed:" << errorMessage;
        return false;
    }
    if (!patientRepo.createTable(errorMessage)) {
        qWarning() << "AppExmpl: patients table failed:" << errorMessage;
        return false;
    }
    if (!doctorSpecialityRepo.createTable(errorMessage)) {
        qWarning() << "AppExmpl: doctor_specialties table failed:" << errorMessage;
        return false;
    }
    if (!appointmentsRepo.createTable(errorMessage)) {
        qWarning() << "AppExmpl: appointments table failed:" << errorMessage;
        return false;
    }

    qDebug() << "AppExmpl: all migrations applied successfully.";
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

    QVector<Doctor> doctors;
    if (!doctorRepo.listAll(doctors, errorMessage)) {
        return false;
    }

    QVector<DoctorSpecialty> doctorSpecialties;
    if (!doctorSpecialityRepo.listAll(doctorSpecialties, errorMessage)) {
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
    if (doctors.isEmpty()) {
        errorMessage = "AppExmpl: no doctors to seed appointments.";
        qWarning() << errorMessage;
        return false;
    }
    if (doctorSpecialties.isEmpty()) {
        errorMessage = "AppExmpl: no doctor_specialties to seed appointments.";
        qWarning() << errorMessage;
        return false;
    }

    int createdCount = 0;
    for (int i = 0; i < 5; ++i) {
        Appointments appt;
        appt.patient_id          = patients.at(i % patients.size()).id;
        appt.doctor_id           = doctors.at(i % doctors.size()).id;
        appt.doctor_specialty_id = doctorSpecialties.at(i % doctorSpecialties.size()).id;
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

    qDebug() << "AppExmpl: created" << createdCount << "out of 5 appointments.";
    errorMessage.clear();
    return true;
}

bool AppExmpl::pgDumpAll() {
    QProcess proc;
    proc.setStandardOutputFile("dump.txt");
    proc.start("pg_dumpall", {
        "-h", "127.0.0.1",
        "-p", "6321",
        "-U", "postgres"
    });
    proc.waitForFinished(-1);
    qDebug() << proc.exitCode() << proc.readAllStandardError();
    return (proc.exitCode() == 0);
}