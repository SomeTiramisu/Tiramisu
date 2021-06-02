TEMPLATE = subdirs

SUBDIRS = \
    app \
    qtquick

app.file = src/app/app.pro
qtquick.file = src/qtquick/qtquick.pro

app.depends = qtquick

CONFIG += c++17

ANDROID_ABIS = arm64-v8a
ANDROID_PACKAGE_SOURCE_DIR = ./android
