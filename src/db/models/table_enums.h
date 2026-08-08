#ifndef DB_ENUMS_H
#define DB_ENUMS_H

#include <QObject>

namespace DbEnums {

Q_NAMESPACE

enum class Sex {
    Male   = 'M',
    Female = 0x0416   // Cyrillic Ж (U+0416)
};
Q_ENUM_NS(Sex)

enum class AppointmentStatus {
    Confirmed,  // "Подтверждена"
    Completed,  // "Завершена"
    Canceled,   // "Отменена"
    NoShow      // "Неявка"
};
Q_ENUM_NS(AppointmentStatus)

} // namespace DbEnums

// Optional: helper to get the DB string for status
inline QString appointmentStatusToDbString(DbEnums::AppointmentStatus s) {
    switch (s) {
    case DbEnums::AppointmentStatus::Confirmed: return QStringLiteral("Подтверждена");
    case DbEnums::AppointmentStatus::Completed: return QStringLiteral("Завершена");
    case DbEnums::AppointmentStatus::Canceled:  return QStringLiteral("Отменена");
    case DbEnums::AppointmentStatus::NoShow:    return QStringLiteral("Неявка");
    }
    return {};
}

#endif // DB_ENUMS_H