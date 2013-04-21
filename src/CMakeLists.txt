#
# 2010 Nik Karpinsky
#

###
# Add our sources
set(SOURCES
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

add_library("${PROJECT_NAME}" ${SOURCES} ${HEADERS})
