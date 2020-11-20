QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

LIBS += -L/run/media/guillaume/DATA/ReaderProject/libarchive-3.4.3/android/prefix/lib/ -larchive
LIBS += -L/run/media/guillaume/DATA/ReaderProject/opencv-4.5.0/android/prefix/sdk/native/libs/arm64-v8a/ -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
INCLUDEPATH +=/run/media/guillaume/DATA/ReaderProject/libarchive-3.4.3/android/prefix/include/
INCLUDEPATH +=/run/media/guillaume/DATA/ReaderProject/opencv-4.5.0/android/prefix/sdk/native/jni/include/



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    book.cpp \
    bookpage.cpp \
    image.cpp \
    main.cpp \
    mainwindowreader.cpp \
    pagescene.cpp \
    pageview.cpp

HEADERS += \
    book.h \
    bookpage.h \
    image.h \
    mainwindowreader.h \
    pagescene.h \
    pageview.h

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
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

