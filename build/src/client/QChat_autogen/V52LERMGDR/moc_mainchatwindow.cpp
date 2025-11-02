/****************************************************************************
** Meta object code from reading C++ file 'mainchatwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "client/mainchatwindow.h"
#include <QtNetwork/QSslError>
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainchatwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14MainChatWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN14MainChatWindowE = QtMocHelpers::stringData(
    "MainChatWindow",
    "on_sendButton_clicked",
    "",
    "on_addfriendButton_clicked",
    "on_addgroupButton_clicked",
    "on_creategroupButton_clicked",
    "on_tabWidget_currentChanged",
    "index",
    "onServerNotifyPictureChanged",
    "json",
    "js",
    "onFriendWidgetContextMenuRequested",
    "pos",
    "onGroupWidgetContextMenuRequested",
    "onPictureLabelContextMenuRequested",
    "onGroupDissolved",
    "groupid",
    "onFriendDeleted",
    "friendid",
    "onFriendAdded",
    "id",
    "string",
    "name",
    "onGroupAdded",
    "onPictureDownloaded",
    "userid",
    "picture",
    "transferText",
    "onFriendClicked",
    "QListWidgetItem*",
    "item",
    "onGroupClicked",
    "onOfflineMsg",
    "jsStr",
    "appendOneChatMsg",
    "appendGroupChatMsg"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN14MainChatWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  134,    2, 0x08,    1 /* Private */,
       3,    0,  135,    2, 0x08,    2 /* Private */,
       4,    0,  136,    2, 0x08,    3 /* Private */,
       5,    0,  137,    2, 0x08,    4 /* Private */,
       6,    1,  138,    2, 0x08,    5 /* Private */,
       8,    1,  141,    2, 0x08,    7 /* Private */,
      11,    1,  144,    2, 0x08,    9 /* Private */,
      13,    1,  147,    2, 0x08,   11 /* Private */,
      14,    1,  150,    2, 0x08,   13 /* Private */,
      15,    1,  153,    2, 0x08,   15 /* Private */,
      17,    1,  156,    2, 0x08,   17 /* Private */,
      19,    2,  159,    2, 0x08,   19 /* Private */,
      23,    2,  164,    2, 0x08,   22 /* Private */,
      24,    2,  169,    2, 0x08,   25 /* Private */,
      27,    0,  174,    2, 0x08,   28 /* Private */,
      28,    1,  175,    2, 0x08,   29 /* Private */,
      31,    1,  178,    2, 0x08,   31 /* Private */,
      32,    1,  181,    2, 0x08,   33 /* Private */,
      34,    1,  184,    2, 0x08,   35 /* Private */,
      35,    1,  187,    2, 0x08,   37 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 21,   20,   22,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 21,   20,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::QPixmap,   25,   26,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainChatWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN14MainChatWindowE.offsetsAndSizes,
    qt_meta_data_ZN14MainChatWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN14MainChatWindowE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainChatWindow, std::true_type>,
        // method 'on_sendButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_addfriendButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_addgroupButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_creategroupButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tabWidget_currentChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onServerNotifyPictureChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'onFriendWidgetContextMenuRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onGroupWidgetContextMenuRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onPictureLabelContextMenuRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onGroupDissolved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onFriendDeleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onFriendAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<string, std::false_type>,
        // method 'onGroupAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<string, std::false_type>,
        // method 'onPictureDownloaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPixmap &, std::false_type>,
        // method 'transferText'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFriendClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'onGroupClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'onOfflineMsg'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'appendOneChatMsg'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'appendGroupChatMsg'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>
    >,
    nullptr
} };

void MainChatWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainChatWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_sendButton_clicked(); break;
        case 1: _t->on_addfriendButton_clicked(); break;
        case 2: _t->on_addgroupButton_clicked(); break;
        case 3: _t->on_creategroupButton_clicked(); break;
        case 4: _t->on_tabWidget_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onServerNotifyPictureChanged((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 6: _t->onFriendWidgetContextMenuRequested((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 7: _t->onGroupWidgetContextMenuRequested((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 8: _t->onPictureLabelContextMenuRequested((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 9: _t->onGroupDissolved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->onFriendDeleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->onFriendAdded((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<string>>(_a[2]))); break;
        case 12: _t->onGroupAdded((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<string>>(_a[2]))); break;
        case 13: _t->onPictureDownloaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QPixmap>>(_a[2]))); break;
        case 14: _t->transferText(); break;
        case 15: _t->onFriendClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 16: _t->onGroupClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 17: _t->onOfflineMsg((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->appendOneChatMsg((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 19: _t->appendGroupChatMsg((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainChatWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainChatWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN14MainChatWindowE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainChatWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
