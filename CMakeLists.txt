cmake_policy(SET CMP0048 NEW)
project(Tiramisu VERSION 1.0)
cmake_minimum_required(VERSION 3.19)

set(QT5_DEP_VERSION 5.15)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic")


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 ${QT5_DEP_VERSION} REQUIRED COMPONENTS Qml Quick Gui QuickControls2)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(LibArchive REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UNARR REQUIRED libunarr)
#pkg_check_modules(POPPLER REQUIRED poppler-cpp)

add_subdirectory(src)

