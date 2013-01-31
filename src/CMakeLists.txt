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

###
# Build the project

set(SOURCES
    main.cpp
	)
	
add_executable(${PROJECT_NAME}
	MACOSX_BUNDLE
	${SOURCES}
	)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

install (TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )
