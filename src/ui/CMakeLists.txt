find_package(Qt6 COMPONENTS Widgets Network QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Note: We are using lowercase mainwindow to match your actual files on disk
add_library(ui_lib STATIC
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    LoginWindow.cpp
    LoginWindow.h
    LoginWindow.ui
)

target_include_directories(ui_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ui_lib PUBLIC
	core_lib
	Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

