#
# 2010 Nik Karpinsky
#

###
# Add our sources
set(SOURCES
    ScriptInterface.cpp

    CalibrationEngine.cpp
	CalibrationData.cpp
    Display.cpp
	Utils.cpp
    
    JSSerializer.cpp

    NFringeStructuredLight.cpp
    TwoWavelengthPhaseUnwrapper.cpp
	)

file(GLOB
    HEADERS
    ../include/*.h
    )

#   Qt Files
set(MOCS
    ../include/ScriptInterface.h
    ../include/DynamicallyScriptableQObject.h
    ../include/CalibrationData.h
    ../include/CalibrationEngine.h
    ../include/ISerializer.h
    ../include/JSSerializer.h
    )

#   ---------------------------------------------------------------------------------------
#                           Qt Generation
#   ---------------------------------------------------------------------------------------

qt4_wrap_cpp(GENERATED_SOURCES ${MOCS})
source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

#
#   Add the library
#
add_library("${PROJECT_NAME}" ${SOURCES} ${HEADERS} ${GENERATED_SOURCES} ${MOCS})