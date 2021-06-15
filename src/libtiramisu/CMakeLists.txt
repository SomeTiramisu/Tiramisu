set(tiramisu_SRCS
    tiramisu.cpp
    pagepreloader.cpp
    pagescheduler.cpp
    parsers/parser.cpp
    parsers/parseutils.cpp
    parsers/libarchiveparser.cpp
    parsers/unarrparser.cpp
    utils/utils.cpp
    utils/imageproc.cpp
    strnatcmp/strnatcmp.c
    utils/crop.cpp
)

add_library(tiramisu SHARED ${tiramisu_SRCS})

target_link_libraries(tiramisu PRIVATE
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    ${UNARR_LINK_LIBRARIES}
    ${LibArchive_LIBRARIES}
    ${TURBOJPEG_LINK_LIBRARIES}
)

set_target_properties(tiramisu PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tiramisu)
