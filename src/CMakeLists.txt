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

## Lens Stuff
add_subdirectory(../Libraries/Lens/src/lens ${PROJECT_BINARY_DIR}/Libraries/Lens)
include_directories(../Libraries/Lens/src)
include_directories(${LENS_INCLUDE_DIRS})
set(LIBS ${LIBS} lens "${LENS_LIBRARIES}")

###
# Build the project

set(SOURCES
    Main.cpp
    CalibrationEngine.h
    CalibrationEngine.cpp
	CalibrationData.h
	CalibrationData.cpp
    Display.h
    Display.cpp
	Utils.h
	Utils.cpp
	
    NFringePhaseWrapper.h
    NFringePhaseWrapper.cpp
    TwoWavelengthPhaseUnwrapper.h
    TwoWavelengthPhaseUnwrapper.cpp
	)
	
add_executable(${PROJECT_NAME}
	MACOSX_BUNDLE
	${SOURCES}
	)

target_link_libraries(${PROJECT_NAME} ${LIBS})

install (TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )
