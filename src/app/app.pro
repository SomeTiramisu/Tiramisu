TEMPLATE = app

CONFIG += qt qtquickcompiler
QT += quickcontrols2

TARGET = tiramisuapp

DESTDIR = ../../bin

RESOURCES += \
    resources.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    qml/Main.qml \
    qml/NavigationDrawer.qml \
    qml/ReaderView.qml \
    qml/background.png

SOURCES += \
    main.cpp

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

