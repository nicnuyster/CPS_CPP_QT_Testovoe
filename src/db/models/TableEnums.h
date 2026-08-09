#ifndef TABLEENUMS_H
#define TABLEENUMS_H

#include <QObject>

namespace TableEnums {

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

} // namespace TableEnums

inline QString appointmentStatusToDbString(TableEnums::AppointmentStatus s) {
    switch (s) {
    case TableEnums::AppointmentStatus::Confirmed: return QStringLiteral("Подтверждена");
    case TableEnums::AppointmentStatus::Completed: return QStringLiteral("Завершена");
    case TableEnums::AppointmentStatus::Canceled:  return QStringLiteral("Отменена");
    case TableEnums::AppointmentStatus::NoShow:    return QStringLiteral("Неявка");
    }
    return {};
}

#endif // TABLEENUMS_H