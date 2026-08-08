/****************************************************************************
** Meta object code from reading C++ file 'table_enums.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.19)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/db/models/table_enums.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'table_enums.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.19. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DbEnums_t {
    QByteArrayData data[9];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DbEnums_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DbEnums_t qt_meta_stringdata_DbEnums = {
    {
QT_MOC_LITERAL(0, 0, 7), // "DbEnums"
QT_MOC_LITERAL(1, 8, 3), // "Sex"
QT_MOC_LITERAL(2, 12, 4), // "Male"
QT_MOC_LITERAL(3, 17, 6), // "Female"
QT_MOC_LITERAL(4, 24, 17), // "AppointmentStatus"
QT_MOC_LITERAL(5, 42, 9), // "Confirmed"
QT_MOC_LITERAL(6, 52, 9), // "Completed"
QT_MOC_LITERAL(7, 62, 8), // "Canceled"
QT_MOC_LITERAL(8, 71, 6) // "NoShow"

    },
    "DbEnums\0Sex\0Male\0Female\0AppointmentStatus\0"
    "Confirmed\0Completed\0Canceled\0NoShow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DbEnums[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       2,   14, // enums/sets
       0,    0, // constructors
       4,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x2,    2,   24,
       4,    4, 0x2,    4,   28,

 // enum data: key, value
       2, uint(DbEnums::Sex::Male),
       3, uint(DbEnums::Sex::Female),
       5, uint(DbEnums::AppointmentStatus::Confirmed),
       6, uint(DbEnums::AppointmentStatus::Completed),
       7, uint(DbEnums::AppointmentStatus::Canceled),
       8, uint(DbEnums::AppointmentStatus::NoShow),

       0        // eod
};

QT_INIT_METAOBJECT const QMetaObject DbEnums::staticMetaObject = { {
    nullptr,
    qt_meta_stringdata_DbEnums.data,
    qt_meta_data_DbEnums,
    nullptr,
    nullptr,
    nullptr
} };

QT_WARNING_POP
QT_END_MOC_NAMESPACE
