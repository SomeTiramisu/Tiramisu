QT       += core gui qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

android {
LIBS += -L/home/guillaume/ReaderProject/libarchive-3.4.3/android/prefix/lib/ -larchive
LIBS += -L/home/guillaume/ReaderProject/opencv-4.5.0/android/prefix/sdk/native/libs/arm64-v8a/ -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
LIBS += -L/home/guillaume/ReaderProject/unarr-1.0.1/android/prefix/lib/ -lunarr
INCLUDEPATH +=/home/guillaume/ReaderProject/libarchive-3.4.3/android/prefix/include/
INCLUDEPATH +=/home/guillaume/ReaderProject/opencv-4.5.0/android/prefix/sdk/native/jni/include/
INCLUDEPATH +=/home/guillaume/ReaderProject/unarr-1.0.1/android/prefix/include/
DEFINES += \
    ARCHIVE_FILENAME=\\\"/storage/emulated/0/b.cbr\\\" \
    BACKGROUND_FILENAME=\\\"/storage/emulated/0/b.png\\\"
}

!android {
LIBS += -larchive -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lunarr
INCLUDEPATH += /usr/include/opencv4/
DEFINES += \
    ARCHIVE_FILENAME=\\\"/home/guillaume/reader/b.cbr\\\" \
    BACKGROUND_FILENAME=\\\"/home/guillaume/reader/b.png\\\"
}



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backend.cpp \
    book.cpp \
    image.cpp \
    main.cpp \
    pagecontroller.cpp \
    pageimageprovider.cpp \
    pageworker.cpp \
    strnatcmp/strnatcmp.c

HEADERS += \
    backend.h \
    book.h \
    image.h \
    pagecontroller.h \
    pageimageprovider.h \
    pageworker.h \
    strnatcmp/strnatcmp.h

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
    ui.qml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
    res.qrc

