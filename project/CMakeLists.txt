cmake_minimum_required(VERSION 3.19)

project(visualize-sorting-alg LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt_add_executable(visualize-sorting-alg
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow_ui.cpp
    mainwindow_anim.cpp
    mainwindow_slots.cpp
    mainwindow_ui.h
    bar.h
    mainwindow_ui.ui
    rs.qrc
)

target_link_libraries(visualize-sorting-alg
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

install(TARGETS visualize-sorting-alg
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET visualize-sorting-alg
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})