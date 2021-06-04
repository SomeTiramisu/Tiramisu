set(qmlplugin_SRCS
    qmlplugin.cpp
    #asyncpageimageprovider.cpp
    backend.cpp
    classicimagerunnable.cpp
    pageitem.cpp
    pagepreloader.cpp
    pagescheduler.cpp
    #simpleimagerunnable.cpp
    parsers/parser.cpp
    parsers/parseutils.cpp
    parsers/libarchiveparser.cpp
    parsers/unarrparser.cpp
    #parsers/popplerparser.cpp
    utils/utils.cpp
    utils/imageproc.cpp
    strnatcmp/strnatcmp.c
)

add_library(tiramisuqmlplugin SHARED ${qmlplugin_SRCS})

target_link_libraries(tiramisuqmlplugin
    Qt6::Qml
    Qt6::Gui
    Qt6::Quick
    Qt6::Core
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    ${UNARR_LINK_LIBRARIES}
    #${POPPLER_LINK_LIBRARIES}
    ${LibArchive_LIBRARIES}
)

set_target_properties(tiramisuqmlplugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tiramisu)
file(COPY qmldir DESTINATION ${CMAKE_BINARY_DIR}/bin/tiramisu)
