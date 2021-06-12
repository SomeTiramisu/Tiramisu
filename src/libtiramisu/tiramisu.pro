TEMPLATE = lib

CONFIG += qt plugin qmltypes
QT += qml quick

TARGET = tiramisu

android {
LIBS += -L/home/guillaume/reader/cpp/reader/src/app/android/libs/arm64-v8a/ -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr -lturbojpeg
INCLUDEPATH += /home/guillaume/reader/cpp/reader/include/
}
!android {
LIBS += -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr -lturbojpeg
INCLUDEPATH += /usr/include/opencv4/
}

HEADERS += \
    pageitem.h \
    pagepreloader.h \
    pagescheduler.h \
    parsers/dummyparser.h \
    parsers/libarchiveparser.h \
    parsers/parser.h \
    parsers/parseutils.h \
    parsers/unarrparser.h \
    strnatcmp/strnatcmp.h \
    utils/imageproc.h \
    utils/utils.h \
    runnables/cropdetectrunnable.h \
    runnables/cropscalerunnable.h \
    utils/crop.h

SOURCES += \
    pagepreloader.cpp \
    pagescheduler.cpp \
    parsers/libarchiveparser.cpp \
    parsers/parser.cpp \
    parsers/parseutils.cpp \
    parsers/unarrparser.cpp \
    strnatcmp/strnatcmp.c \
    utils/imageproc.cpp \
    utils/utils.cpp \
    runnables/cropdetectrunnable.cpp \
    runnables/cropscalerunnable.cpp \
    utils/crop.cpp
