#
# 2010 Nik Karpinsky
#

cmake_minimum_required(VERSION 2.6)

# Set package properties
project(PortalCalibration)

###
# CMake Modules Paths

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/../CMakeModules")

###
# Perform build configuration of dependencies
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set (LIBS ${LIBS} ${OpenCV_LIBS})

## QT Related stuff
find_package(Qt4 COMPONENTS QtCore REQUIRED)
include(${QT_USE_FILE})
set (LIBS ${LIBS} ${QT_LIBRARIES})

###
# Build the project

set(SOURCES
    main.cpp
	calibrate.cpp
	calibrate.h
	cameraInput.h
	cameraInput.cpp
	)
	
add_executable(${PROJECT_NAME}
	MACOSX_BUNDLE
	${SOURCES}
	)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

install (TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )
