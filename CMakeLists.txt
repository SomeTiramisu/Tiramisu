cmake_policy(SET CMP0048 NEW)
project(Tiramisu VERSION 1.0)
cmake_minimum_required(VERSION 3.19)

set(QT6_DEP_VERSION 6.1)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic")


find_package(Qt6 COMPONENTS Core Qml Quick Gui QuickControls2 REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(LibArchive REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UNARR REQUIRED libunarr)
#pkg_check_modules(POPPLER REQUIRED poppler-cpp)

add_subdirectory(src)

