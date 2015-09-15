/****************************************************************************
** Meta object code from reading C++ file 'maindesktop.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../BackupSys/maindesktop.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'maindesktop.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainDesktop_t {
    QByteArrayData data[15];
    char stringdata0[237];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainDesktop_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainDesktop_t qt_meta_stringdata_MainDesktop = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MainDesktop"
QT_MOC_LITERAL(1, 12, 16), // "send_path_toback"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 9), // "from_path"
QT_MOC_LITERAL(4, 40, 7), // "to_path"
QT_MOC_LITERAL(5, 48, 15), // "thread_bp_start"
QT_MOC_LITERAL(6, 64, 15), // "thread_cp_start"
QT_MOC_LITERAL(7, 80, 14), // "thread_cp_done"
QT_MOC_LITERAL(8, 95, 21), // "on_exitButton_clicked"
QT_MOC_LITERAL(9, 117, 27), // "on_editfromPath_textChanged"
QT_MOC_LITERAL(10, 145, 4), // "arg1"
QT_MOC_LITERAL(11, 150, 19), // "on_fromTool_clicked"
QT_MOC_LITERAL(12, 170, 25), // "on_edittoPath_textChanged"
QT_MOC_LITERAL(13, 196, 17), // "on_toTool_clicked"
QT_MOC_LITERAL(14, 214, 22) // "on_startButton_clicked"

    },
    "MainDesktop\0send_path_toback\0\0from_path\0"
    "to_path\0thread_bp_start\0thread_cp_start\0"
    "thread_cp_done\0on_exitButton_clicked\0"
    "on_editfromPath_textChanged\0arg1\0"
    "on_fromTool_clicked\0on_edittoPath_textChanged\0"
    "on_toTool_clicked\0on_startButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainDesktop[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       5,    0,   69,    2, 0x06 /* Public */,
       6,    0,   70,    2, 0x06 /* Public */,
       7,    0,   71,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   72,    2, 0x08 /* Private */,
       9,    1,   73,    2, 0x08 /* Private */,
      11,    0,   76,    2, 0x08 /* Private */,
      12,    1,   77,    2, 0x08 /* Private */,
      13,    0,   80,    2, 0x08 /* Private */,
      14,    0,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainDesktop::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainDesktop *_t = static_cast<MainDesktop *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_path_toback((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->thread_bp_start(); break;
        case 2: _t->thread_cp_start(); break;
        case 3: _t->thread_cp_done(); break;
        case 4: _t->on_exitButton_clicked(); break;
        case 5: _t->on_editfromPath_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->on_fromTool_clicked(); break;
        case 7: _t->on_edittoPath_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->on_toTool_clicked(); break;
        case 9: _t->on_startButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainDesktop::*_t)(const QString , const QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainDesktop::send_path_toback)) {
                *result = 0;
            }
        }
        {
            typedef void (MainDesktop::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainDesktop::thread_bp_start)) {
                *result = 1;
            }
        }
        {
            typedef void (MainDesktop::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainDesktop::thread_cp_start)) {
                *result = 2;
            }
        }
        {
            typedef void (MainDesktop::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainDesktop::thread_cp_done)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject MainDesktop::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainDesktop.data,
      qt_meta_data_MainDesktop,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainDesktop::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainDesktop::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainDesktop.stringdata0))
        return static_cast<void*>(const_cast< MainDesktop*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainDesktop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MainDesktop::send_path_toback(const QString _t1, const QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainDesktop::thread_bp_start()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MainDesktop::thread_cp_start()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void MainDesktop::thread_cp_done()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
