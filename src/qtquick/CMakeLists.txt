set(qmlplugin_SRCS
    qmlplugin.cpp
    pageitem.cpp
)

add_library(tiramisuqmlplugin SHARED ${qmlplugin_SRCS})

target_link_libraries(tiramisuqmlplugin PRIVATE
    Qt6::Qml
    Qt6::Gui
    Qt6::Quick
    Qt6::Core
    tiramisu
)

set_target_properties(tiramisuqmlplugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tiramisu)
file(COPY qmldir DESTINATION ${CMAKE_BINARY_DIR}/bin/tiramisu)
