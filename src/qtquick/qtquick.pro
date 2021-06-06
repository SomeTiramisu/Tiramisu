TEMPLATE = lib

CONFIG += qt plugin qmltypes
QT += qml quick

QML_IMPORT_NAME = tiramisu
QML_IMPORT_MAJOR_VERSION = 1

TARGET = tiramisuqmlplugin

#DESTDIR = ../../bin/qtquick/tiramisu

android {
LIBS += -L/home/guillaume/reader/cpp/reader/src/app/android/libs/arm64-v8a/ -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr -lturbojpeg
INCLUDEPATH += /home/guillaume/reader/cpp/reader/include/
}
!android {
LIBS += -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr -lturbojpeg
INCLUDEPATH += /usr/include/opencv4/
}


DISTFILES += \
    qmldir

HEADERS += \
    #asyncpageimageprovider.h \
    backend.h \
    #runnables/classicimagerunnable.h \
    #runnables/croprunnable.h \
    #runnables/losslesscroprunnable.h \
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
    #runnables/scalerunnable.h \
    #runnables/simpleimagerunnable.h \
    strnatcmp/strnatcmp.h \
    utils/imageproc.h \
    utils/utils.h \
    runnables/cropdetectrunnable.h \
    runnables/cropscalerunnable.h

SOURCES += \
    #asyncpageimageprovider.cpp \
    backend.cpp \
    #runnables/classicimagerunnable.cpp \
    #runnables/croprunnable.cpp \
    #runnables/losslesscroprunnable.cpp \
    pageitem.cpp \
    pagepreloader.cpp \
    pagescheduler.cpp \
    parsers/libarchiveparser.cpp \
    parsers/parser.cpp \
    parsers/parseutils.cpp \
    #parsers/popplerparser.cpp \
    parsers/unarrparser.cpp \
    qmlplugin.cpp \
    #runnables/scalerunnable.cpp \
    #runnables/simpleimagerunnable.cpp \
    strnatcmp/strnatcmp.c \
    utils/imageproc.cpp \
    utils/utils.cpp \
    runnables/cropdetectrunnable.cpp \
    runnables/cropscalerunnable.cpp
