cmake_minimum_required(VERSION 3.5)

project(psvr
		VERSION 1.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(HIDAPI REQUIRED)
include_directories(${HIDAPI_INCLUDE_DIRS})

find_package(LIBVLC REQUIRED)
include_directories(${LIBVLC_INCLUDE_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")


set(SOURCE_FILES
		src/main.cpp
		include/mainwindow.h
		src/mainwindow.cpp
		src/psvr.cpp
		include/psvr.h
		src/psvrthread.cpp
		include/psvrthread.h
		src/hmdwindow.cpp
		include/hmdwindow.h
		src/hmdwidget.cpp
		include/hmdwidget.h
		src/videoplayer.cpp
		include/videoplayer.h)

set(UI_FILES
		src/mainwindow.ui)

set(QRC_FILES
		resources.qrc)



configure_file("${CMAKE_CURRENT_SOURCE_DIR}/project_version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/project_version.h")

add_executable(psvr ${UI_FILES} ${SOURCE_FILES}  ${QRC_FILES})
qt5_use_modules(psvr Core Widgets Gui)
target_link_libraries(psvr ${HIDAPI_LIBRARIES})
target_link_libraries(psvr ${LIBVLC_LIBRARY})

