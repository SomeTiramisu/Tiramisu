TEMPLATE = lib

CONFIG += qt plugin qmltypes
QT += qml quick

QML_IMPORT_NAME = tiramisu
QML_IMPORT_MAJOR_VERSION = 1

TARGET = tiramisuqmlplugin

DESTDIR = ../../bin/qtquick/tiramisu

LIBS += -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr -lpoppler-cpp
android {
LIBS += -L/home/guillaume/reader/cpp/reader/android/libs/arm64-v8a/
INCLUDEPATH += /home/guillaume/reader/cpp/reader/include/
}
!android {
INCLUDEPATH += /usr/include/opencv4/
}

DISTFILES += \
    qmldir

HEADERS += \
    asyncpageimageprovider.h \
    backend.h \
    classicimagerunnable.h \
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
    simpleimagerunnable.h \
    strnatcmp/strnatcmp.h \
    utils/imageproc.h \
    utils/utils.h

SOURCES += \
    asyncpageimageprovider.cpp \
    backend.cpp \
    classicimagerunnable.cpp \
    pageitem.cpp \
    pagepreloader.cpp \
    pagescheduler.cpp \
    parsers/libarchiveparser.cpp \
    parsers/parser.cpp \
    parsers/parseutils.cpp \
    #parsers/popplerparser.cpp \
    parsers/unarrparser.cpp \
    qmlplugin.cpp \
    simpleimagerunnable.cpp \
    strnatcmp/strnatcmp.c \
    utils/imageproc.cpp \
    utils/utils.cpp
