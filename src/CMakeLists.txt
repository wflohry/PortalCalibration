#
# 2010 Nik Karpinsky
#

###
# Add our sources

set(
    HEADERS
    CalibrationEngine.h
    CalibrationData.h
    Display.h
    Utils.h
    NFringeStructuredLight.h
    TwoWavelengthPhaseUnwrapper.h
    )

set(SOURCES
    Main.cpp
    CalibrationEngine.cpp
	CalibrationData.cpp
    Display.cpp
	Utils.cpp
	
    NFringeStructuredLight.cpp
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
