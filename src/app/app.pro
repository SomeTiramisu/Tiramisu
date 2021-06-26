TEMPLATE = app

CONFIG += qt qtquickcompiler
QT += quickcontrols2

TARGET = tiramisuapp

DESTDIR = ../../bin

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/Main.qml \
    qml/NavigationDrawer.qml \
    qml/ReaderView.qml \
    qml/background.png

equals(QT_MAJOR_VERSION, 5) {
    DISTFILES += \
        android.qt5/AndroidManifest.xml \
        android.qt5/build.gradle \
        android.qt5/gradle.properties \
        android.qt5/gradle/wrapper/gradle-wrapper.jar \
        android.qt5/gradle/wrapper/gradle-wrapper.properties \
        android.qt5/gradlew \
        android.qt5/gradlew.bat \
        android.qt5/res/values/libs.xml
}

equals(QT_MAJOR_VERSION, 6) {
    DISTFILES += \
        android.qt6/AndroidManifest.xml \
        android.qt6/build.gradle \
        android.qt6/gradle.properties \
        android.qt6/gradle/wrapper/gradle-wrapper.jar \
        android.qt6/gradle/wrapper/gradle-wrapper.properties \
        android.qt6/gradlew \
        android.qt6/gradlew.bat \
        android.qt6/res/values/libs.xml
}

SOURCES += \
    main.cpp

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    equals(QT_MAJOR_VERSION, 5) {
        ANDROID_PACKAGE_SOURCE_DIR = \
            $$PWD/android.qt5
    }
    equals(QT_MAJOR_VERSION, 6) {
        ANDROID_PACKAGE_SOURCE_DIR = \
            $$PWD/android.qt6
    }
}

