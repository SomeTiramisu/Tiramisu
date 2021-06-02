set(APP_SRCS
    main.cpp
)

qt_add_resources(APP_SRCS resources.qrc)

add_executable(tiramisu ${APP_SRCS})

target_link_libraries(tiramisu
    Qt5::Core
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Gui
    Qt5::QuickControls2
    tiramisuqmlplugin
)


    
