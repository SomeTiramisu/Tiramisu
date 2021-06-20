TEMPLATE = lib

CONFIG += qt plugin qmltypes
QT += qml quick

QML_IMPORT_NAME = tiramisu
QML_IMPORT_MAJOR_VERSION = 1

TARGET = tiramisuqmlplugin

#DESTDIR = ../../bin/qtquick/tiramisu

android {
LIBS += -L/home/guillaume/reader/cpp/reader/src/app/android/libs/arm64-v8a/ -lopencv_core
INCLUDEPATH += /home/guillaume/reader/cpp/reader/include/
}
!android {
LIBS += -lopencv_core
INCLUDEPATH += /usr/include/opencv4/
}

DISTFILES += \
    qmldir

HEADERS += \
    pageitem.h \
    qmlplugin.h

SOURCES += \
    pageitem.cpp \
    qmlplugin.cpp

