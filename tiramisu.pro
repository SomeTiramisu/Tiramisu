TEMPLATE = subdirs

SUBDIRS = \
    app \
    qtquick

app.file = src/app/app.pro
qtquick.file = src/qtquick/qtquick.pro

app.depends = qtquick

CONFIG += c++20
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
