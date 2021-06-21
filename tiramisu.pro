TEMPLATE = subdirs

#CONFIG += ordered

SUBDIRS = \
    app \
    qtquick \
    libtiramisu

app.file = src/app/app.pro
qtquick.file = src/qtquick/qtquick.pro
libtiramisu.file = src/libtiramisu/tiramisu.pro

app.depends = qtquick
qtquick.depends = libtiramisu

CONFIG += c++17
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
