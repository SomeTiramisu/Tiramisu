TEMPLATE = app

CONFIG += qt
QT += quickcontrols2

TARGET = tiramisu

DESTDIR = ../../bin

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/Main.qml \
    qml/NavigationDrawer.qml \
    qml/ReaderView.qml \
    qml/background.png

SOURCES += \
    main.cpp

