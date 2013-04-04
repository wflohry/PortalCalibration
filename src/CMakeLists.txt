#
# 2010 Nik Karpinsky
#

###
# Add our sources
set(PROJECT_SOURCES
    Main.cpp
    CalibrationEngine.cpp
    CalibrationEngine.h
	CalibrationData.cpp
    CalibrationData.h
    Display.cpp
    Display.h
	Utils.cpp
    Utils.h
    
    ISerializer.h
    JSSerializer.h
    JSSerializer.cpp

    NFringeStructuredLight.cpp
    NFringestructuredLight.h
    TwoWavelengthPhaseUnwrapper.cpp
    TwoWavelengthPhaseUnwrapper.h
	)

add_executable(${PROJECT_NAME}
	MACOSX_BUNDLE
    ${PROJECT_SOURCES}
	)

target_link_libraries(${PROJECT_NAME} ${LIBS})

install (TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )
