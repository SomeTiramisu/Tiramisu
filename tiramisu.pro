QT       += core gui qml quick quickcontrols2

TARGET = Tiramisu

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 qtquickcompiler

android {
LIBS += -L/home/guillaume/reader/cpp/reader/android/libs/arm64-v8a/ -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr -lpoppler-cpp
INCLUDEPATH += /home/guillaume/reader/cpp/reader/include/
DEFINES += \
    ARCHIVE_FILENAME=\\\"/storage/emulated/0/b.cbr\\\" \
    ARCHIVE_DIR=\\\"/storage/emulated/0\\\" \
    BACKGROUND_FILENAME=\\\"/storage/emulated/0/b.png\\\"
}

!android {
LIBS += -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr -lpoppler-cpp
INCLUDEPATH += /usr/include/opencv4/
DEFINES += \
    ARCHIVE_FILENAME=\\\"/home/guillaume/reader/b.cbr\\\" \
    ARCHIVE_DIR=\\\"/home/guillaume/reader\\\" \
    BACKGROUND_FILENAME=\\\"/home/guillaume/reader/b.png\\\"
}



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/asyncpageimageprovider.cpp \
    src/backend.cpp \
    src/parsers/libarchiveparser.cpp \
    src/parsers/popplerparser.cpp \
    src/parsers/unarrparser.cpp \
    src/utils/imageproc.cpp \
    src/imagerunnable.cpp \
    src/main.cpp \
    src/pagecontroller.cpp \
    src/parsers/parser.cpp \
    src/strnatcmp/strnatcmp.c

HEADERS += \
    src/asyncpageimageprovider.h \
    src/backend.h \
    src/parsers/libarchiveparser.h \
    src/parsers/parseutils.h \
    src/parsers/popplerparser.h \
    src/parsers/unarrparser.h \
    src/utils/imageproc.h \
    src/imagerunnable.h \
    src/pagecontroller.h \
    src/parsers/parser.h \
    src/strnatcmp/strnatcmp.h \
    src/utils/utils.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = arm64-v8a

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    src/ui/ui.qml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
    src/ui/res.qrc

