TEMPLATE = lib

CONFIG += qt plugin qmltypes
QT += qml quick

QML_IMPORT_NAME = tiramisu
QML_IMPORT_MAJOR_VERSION = 1

TARGET = tiramisuqmlplugin

#DESTDIR = ../../bin/qtquick/tiramisu

android {
LIBS += -L$$PWD/../app/android/libs/arm64-v8a/ -L$$OUT_PWD/../libtiramisu/ -lopencv_core -ltiramisu_arm64-v8a
message($$OUT_PWD)
INCLUDEPATH += /home/guillaume/reader/cpp/reader/include/
}
!android {
LIBS += -L$$OUT_PWD/../libtiramisu/ -lopencv_core -ltiramisu
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

