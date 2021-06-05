TEMPLATE = lib

CONFIG += qt plugin qmltypes
QT += qml quick

QML_IMPORT_NAME = tiramisu
QML_IMPORT_MAJOR_VERSION = 1

TARGET = tiramisuqmlplugin

#DESTDIR = ../../bin/qtquick/tiramisu

android {
LIBS += -L/home/guillaume/reader/cpp/reader/src/app/android/libs/arm64-v8a/ -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr
INCLUDEPATH += /home/guillaume/reader/cpp/reader/include/
}
!android {
LIBS += -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr
INCLUDEPATH += /usr/include/opencv4/
}


DISTFILES += \
    qmldir

HEADERS += \
    #asyncpageimageprovider.h \
    backend.h \
    classicimagerunnable.h \
    croprunnable.h \
    pageitem.h \
    pagepreloader.h \
    pagescheduler.h \
    parsers/dummyparser.h \
    parsers/libarchiveparser.h \
    parsers/parser.h \
    parsers/parseutils.h \
    #parsers/popplerparser.h \
    parsers/unarrparser.h \
    qmlplugin.h \
    scalerunnable.h \
    simpleimagerunnable.h \
    strnatcmp/strnatcmp.h \
    utils/imageproc.h \
    utils/utils.h

SOURCES += \
    #asyncpageimageprovider.cpp \
    backend.cpp \
    classicimagerunnable.cpp \
    croprunnable.cpp \
    pageitem.cpp \
    pagepreloader.cpp \
    pagescheduler.cpp \
    parsers/libarchiveparser.cpp \
    parsers/parser.cpp \
    parsers/parseutils.cpp \
    #parsers/popplerparser.cpp \
    parsers/unarrparser.cpp \
    qmlplugin.cpp \
    scalerunnable.cpp \
    simpleimagerunnable.cpp \
    strnatcmp/strnatcmp.c \
    utils/imageproc.cpp \
    utils/utils.cpp
