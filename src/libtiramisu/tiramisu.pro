TEMPLATE = lib

CONFIG += qt
QT += core

TARGET = tiramisu

android {
LIBS += -L/home/guillaume/reader/cpp/reader/src/app/android.qt5/libs/arm64-v8a/ -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr
INCLUDEPATH += /home/guillaume/reader/cpp/reader/include/
}
!android {
LIBS += -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr
INCLUDEPATH += /usr/include/opencv4/
}

HEADERS += \
    pagescheduler.h \
    parsers/dummyparser.h \
    parsers/libarchiveparser.h \
    parsers/parser.h \
    parsers/parseutils.h \
    parsers/unarrparser.h \
    strnatcmp/strnatcmp.h \
    utils/imageproc.h \
    utils/utils.h \
    utils/crop.h \
    cropscalerunner.h \
    tiramisu.h

SOURCES += \
    pagescheduler.cpp \
    parsers/libarchiveparser.cpp \
    parsers/parser.cpp \
    parsers/parseutils.cpp \
    parsers/unarrparser.cpp \
    strnatcmp/strnatcmp.c \
    utils/imageproc.cpp \
    utils/utils.cpp \
    utils/crop.cpp \
    cropscalerunner.cpp \
    tiramisu.cpp
