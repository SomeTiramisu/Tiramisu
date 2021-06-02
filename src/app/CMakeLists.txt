set(APP_SRCS
    main.cpp
)

qt_add_resources(APP_SRCS resources.qrc)

add_executable(tiramisuapp ${APP_SRCS})

target_link_libraries(tiramisuapp
    Qt::QuickControls2
    tiramisuqmlplugin
)

set_target_properties(tiramisuapp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
