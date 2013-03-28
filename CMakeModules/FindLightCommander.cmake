# - Try to find LightCommander Projector API
# Once done this will define
#  
#  LIGHTCOMMANDER_FOUND        - System has the LightCommander projector API
#  LIGHTCOMMANDER_INCLUDE_DIR  - The LightCommander projector APIs include directory
#  LIGHTCOMMANDER_LIBRARY_DIR  - The LightCommander projector APIs library directory
#  LIGHTCOMMANDER_LIBRARIES    - Link these to use the LightCommander projector API
#   

IF (LIGHTCOMMANDER_FOUND)
    SET( LIGHTCOMMANDER_INCLUDE_DIR )
    SET( LIGHTCOMMANDER_LIBRARY_DIR )
    SET( LIGHTCOMMANDER_LIBRARIES)
ENDIF (LIGHTCOMMANDER_FOUND)

if( WIN32 )
    if( MSVC80 )
        set( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ 8/VC" )
    endif( MSVC80 )
    if( MSVC71 )
        set( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ .NET\ 2003/Vc7" )
    endif( MSVC71 )

    SET( LIGHTCOMMANDER_NAMES PortabilityLayer USBlib)

    # Check which libs we should compile against
    option(LIGHTCOMMANDER_USE_64 "Build with 64 bit libraries" OFF)
    if(LIGHTCOMMANDER_USE_64)
        FIND_PATH( LIGHTCOMMANDER_INCLUDE_DIR PortabilityLayer.h
            PATHS "C:/Program\ Files/Texas\ Instruments-DLP/DLP\ LightCommander\ API\ SW/Include"
            ${COMPILER_PATH}/PlatformSDK/Include )

        FIND_LIBRARY( LIGHTCOMMANDER_LIBRARIES
            NAMES ${LIGHTCOMMANDER_NAMES}
            PATHS "C:/Program\ Files/Texas\ Instruments-DLP/DLP\ LightCommander\ API\ SW/Lib"
            ${COMPILER_PATH}/PlatformSDK/Lib )      
    else(LIGHTCOMMANDER_USE_64)
        FIND_PATH( LIGHTCOMMANDER_INCLUDE_DIR PortabilityLayer.h
            PATHS "C:/Program\ Files\ (x86)/Texas\ Instruments-DLP/DLP\ LightCommander\ API\ SW/Include" 
            ${COMPILER_PATH}/PlatformSDK/Include )

        FIND_LIBRARY( LIGHTCOMMANDER_LIBRARIES
            NAMES ${LIGHTCOMMANDER_NAMES}
            PATHS "C:/Program\ Files\ (x86)/Texas\ Instruments-DLP/DLP\ LightCommander\ API\ SW/Lib" 
            ${COMPILER_PATH}/PlatformSDK/Lib )      

    endif(LIGHTCOMMANDER_USE_64)

    GET_FILENAME_COMPONENT( LIGHTCOMMANDER_LIBRARY_DIR ${LIGHTCOMMANDER_LIBRARIES} PATH )

    IF (LIGHTCOMMANDER_INCLUDE_DIR AND LIGHTCOMMANDER_LIBRARIES)
        SET(LIGHTCOMMANDER_FOUND TRUE)
    ELSE (LIGHTCOMMANDER_INCLUDE_DIR AND LIGHTCOMMANDER_LIBRARIES)
        SET( LIGHTCOMMANDER_FOUND FALSE )
        SET( LIGHTCOMMANDER_LIBRARY_DIR )
        SET( LIGHTCOMMANDER_LIBARIES )
    ENDIF (LIGHTCOMMANDER_INCLUDE_DIR AND LIGHTCOMMANDER_LIBRARIES)

    MARK_AS_ADVANCED(
        LIGHTCOMMANDER_LIBRARIES
        LIGHTCOMMANDER_LIBRARY_DIR
        LIGHTCOMMANDER_INCLUDE_DIR
        )

else( WIN32 )
    message(ERROR "LightCommander projector not supported on this OS")
endif( WIN32 )

