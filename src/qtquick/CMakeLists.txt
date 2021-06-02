set(qmlplugin_SRCS
    qmlplugin.cpp
    asyncpageimageprovider.cpp
    backend.cpp
    classicimagerunnable.cpp
    pageitem.cpp
    pagepreloader.cpp
    pagescheduler.cpp
    simpleimagerunnable.cpp
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
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    ${UNARR_LINK_LIBRARIES}
    #${POPPLER_LINK_LIBRARIES}
    ${LibArchive_LIBRARIES}
)

install(TARGETS tiramisuqmlplugin DESTINATION ${QML_INSTALL_DIR}/tiramisu)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/tiramisu)
