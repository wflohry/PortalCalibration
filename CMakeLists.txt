#
# 2012 Nik Karpinsky
#
###
# Set CMake Stuff
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/CMakeModules")

###
# Set project properties
project(PortalCalibration)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")

#   ---------------------------------------------------------------------------
#   Configure Dependencies
#   ---------------------------------------------------------------------------

#   OpenCV
#   -------------------------------------
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set (LIBS ${LIBS} ${OpenCV_LIBS})

#   QT
#   -------------------------------------
find_package(Qt4 COMPONENTS QtCore QtScript REQUIRED)
include(${QT_USE_FILE})
set (LIBS ${LIBS} ${QT_LIBRARIES})

#   Lens
#   -------------------------------------
add_subdirectory(Libraries/Lens/src/lens ${PROJECT_BINARY_DIR}/Libraries/Lens)
include_directories(Libraries/Lens/src)
include_directories(${LENS_INCLUDE_DIRS})
set(LIBS ${LIBS} lens "${LENS_LIBRARIES}")

#   ReelBlink
#   -------------------------------------
add_subdirectory(Libraries/ReelBlink/src/reelblink ${PROJECT_BINARY_DIR}/Libraries/ReelBlink)
include_directories(Libraries/ReelBlink/src)
include_directories(${REELBLINK_INCLUDE_DIRS})
set(LIBS ${LIBS} ReelBlink "${REELBLINK_LIBRARIES}")

#   ---------------------------------------------------------------------------
#   Add the source
#   ---------------------------------------------------------------------------
include_directories("include")
add_subdirectory(src)

set(LIBS ${LIBS} "${PROJECT_NAME}")

#   ---------------------------------------------------------------------------
#   Add the unit tests
option(PortalCalibration_UNIT_TESTS "Generate PortalCalibration unit tests" OFF)
if(PortalCalibration_UNIT_TESTS)
    # Fixes a bug with VS 2012 compiler and varadic templates
    add_definitions(-D_VARIADIC_MAX=10)

    add_subdirectory(Libraries/GTest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    add_subdirectory(test)
endif()

#   ---------------------------------------------------------------------------
#   Build and link the app!
#   ---------------------------------------------------------------------------
add_executable("${PROJECT_NAME}Launcher"
    MACOSX_BUNDLE
    src/Application.cpp)

add_dependencies("${PROJECT_NAME}Launcher" lens)
add_dependencies("${PROJECT_NAME}Launcher" ${PROJECT_NAME})

target_link_libraries("${PROJECT_NAME}Launcher" ${LIBS})

#   ---------------------------------------------------------------------------
#   Install
#   ---------------------------------------------------------------------------
install( TARGETS "${PROJECT_NAME}Launcher"
    RUNTIME DESTINATION bin/
    BUNDLE DESTINATION ./ )
