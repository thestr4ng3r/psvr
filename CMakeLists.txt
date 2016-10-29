cmake_minimum_required(VERSION 3.5)

project(psvr)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(HIDAPI REQUIRED)
include_directories(${HIDAPI_INCLUDE_DIRS})

find_package(LIBVLC REQUIRED)
include_directories(${LIBVLC_INCLUDE_DIR})


set(SOURCE_FILES
		main.cpp
		mainwindow.cpp
		psvr.cpp
		psvr.h
		psvrthread.cpp
		psvrthread.h
		hmdwindow.cpp
		hmdwindow.h
		hmdwidget.cpp
		hmdwidget.h videoplayer.cpp videoplayer.h)

set(UI_FILES
		mainwindow.ui)


add_executable(psvr ${SOURCE_FILES} ${UI_FILES})
qt5_use_modules(psvr Core Widgets Gui)
target_link_libraries(psvr ${HIDAPI_LIBRARIES})
target_link_libraries(psvr ${LIBVLC_LIBRARY})

