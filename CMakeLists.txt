cmake_minimum_required(VERSION 3.19)

project(Tiramisu VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Quick Core Widgets QuickControls2 REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(LibArchive REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UNARR REQUIRED libunarr)
pkg_check_modules(POPPLER REQUIRED poppler-cpp)

add_executable(Tiramisu
    src/main.cpp
    src/backend.cpp
    src/asyncpageimageprovider.cpp
    src/imagerunnable.cpp
    src/pagecontroller.cpp
    src/utils/imageproc.cpp
    src/parsers/parser.cpp
    src/parsers/libarchiveparser.cpp
    src/parsers/unarrparser.cpp
    src/parsers/popplerparser.cpp
    src/strnatcmp/strnatcmp.c
    src/ui/res.qrc
    )

target_link_libraries(Tiramisu
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    ${UNARR_LINK_LIBRARIES}
    ${POPPLER_LINK_LIBRARIES}
    ${LibArchive_LIBRARIES}
    )
