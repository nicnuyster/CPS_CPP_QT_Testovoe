/****************************************************************************
** Meta object code from reading C++ file 'TableEnums.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.19)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/db/models/TableEnums.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TableEnums.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.19. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TableEnums_t {
    QByteArrayData data[9];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TableEnums_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TableEnums_t qt_meta_stringdata_TableEnums = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TableEnums"
QT_MOC_LITERAL(1, 11, 3), // "Sex"
QT_MOC_LITERAL(2, 15, 4), // "Male"
QT_MOC_LITERAL(3, 20, 6), // "Female"
QT_MOC_LITERAL(4, 27, 17), // "AppointmentStatus"
QT_MOC_LITERAL(5, 45, 9), // "Confirmed"
QT_MOC_LITERAL(6, 55, 9), // "Completed"
QT_MOC_LITERAL(7, 65, 8), // "Canceled"
QT_MOC_LITERAL(8, 74, 6) // "NoShow"

    },
    "TableEnums\0Sex\0Male\0Female\0AppointmentStatus\0"
    "Confirmed\0Completed\0Canceled\0NoShow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TableEnums[] = {

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
       2, uint(TableEnums::Sex::Male),
       3, uint(TableEnums::Sex::Female),
       5, uint(TableEnums::AppointmentStatus::Confirmed),
       6, uint(TableEnums::AppointmentStatus::Completed),
       7, uint(TableEnums::AppointmentStatus::Canceled),
       8, uint(TableEnums::AppointmentStatus::NoShow),

       0        // eod
};

QT_INIT_METAOBJECT const QMetaObject TableEnums::staticMetaObject = { {
    nullptr,
    qt_meta_stringdata_TableEnums.data,
    qt_meta_data_TableEnums,
    nullptr,
    nullptr,
    nullptr
} };

QT_WARNING_POP
QT_END_MOC_NAMESPACE
