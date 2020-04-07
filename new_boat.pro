QT       += core gui
QT       += serialport
QT       += webenginewidgets
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cameramodel.cpp \
    collect_garbage.cpp \
    http_manager.cpp \
    httpcommunication.cpp \
    httpthread.cpp \
    internet.cpp \
    joystick_set.cpp \
    localsetting.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    map.cpp \
    menu.cpp \
    opennetstream.cpp \
    read_data.cpp \
    send_data.cpp \
    serial_set.cpp \
    solve_data.cpp \
    util/json/json_reader.cpp \
    util/json/json_value.cpp \
    util/json/json_writer.cpp \
    video.cpp

HEADERS += \
    GPS.h \
    cameramodel.h \
    http_manager.h \
    httpcommunication.h \
    httpthread.h \
    include/OpenNetStreamDefine.h \
    include/OpenNetStreamError.h \
    include/OpenNetStreamInterFace.h \
    localsetting.h \
    loginwindow.h \
    mainwindow.h \
    opennetstream.h \
    opennetstream_p.h \
    send_data.h \
    util/json/autolink.h \
    util/json/config.h \
    util/json/features.h \
    util/json/forwards.h \
    util/json/json.h \
    util/json/json_batchallocator.h \
    util/json/json_internalarray.inl \
    util/json/json_internalmap.inl \
    util/json/json_valueiterator.inl \
    util/json/reader.h \
    util/json/value.h \
    util/json/writer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

include ($$PWD/QJoysticks/QJoysticks.pri)

RESOURCES += \
    picture.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/win32/ -lOpenNetStream
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/win32/ -lOpenNetStreamd
else:unix: LIBS += -L$$PWD/win32/ -lOpenNetStream

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
