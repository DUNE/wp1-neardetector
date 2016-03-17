MESSAGE(STATUS "\n Looking for Geant4...")

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set(Geant4_DIR $ENV{GEANT4_INSTALL_DIR}/lib64/Geant4-9.6.1/)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set(Geant4_DIR $ENV{GEANT4_INSTALL_DIR}/lib/Geant4-9.6.1/)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

 #message("Geant4 : ${Geant4_DIR}")

find_package(Geant4 REQUIRED)                       # Find Geant4

add_definitions(${Geant4_DEFINITIONS})

SET(GEANT4_INCLUDE_DIR ${Geant4_INCLUDE_DIR})
 #message("Geant4_INCLUDE_DIRS : ${Geant4_INCLUDE_DIRS}")

FIND_PATH(GEANT4_LIBRARY_DIR NAMES libG4event.so libG4event.dylib          PATHS
                                                             ${Geant4_INCLUDE_DIR}/../../lib64/
                                                             ${Geant4_INCLUDE_DIR}/../../lib/
                                                             NO_DEFAULT_PATH)
 #message("GEANT4_LIBRARY_DIR : ${GEANT4_LIBRARY_DIR}")

FIND_PROGRAM(GEANT4_CONFIG_EXECUTABLE  NAMES  geant4-config  PATHS
                                                             ${Geant4_INCLUDE_DIR}/../../bin
                                                             NO_DEFAULT_PATH)
 #message("GEANT4_CONFIG_EXECUTABLE : ${GEANT4_CONFIG_EXECUTABLE}")

if (GEANT4_INCLUDE_DIR AND GEANT4_LIBRARY_DIR AND GEANT4_CONFIG_EXECUTABLE)
   set(GEANT4_FOUND TRUE)
endif (GEANT4_INCLUDE_DIR AND GEANT4_LIBRARY_DIR AND GEANT4_CONFIG_EXECUTABLE)

if (GEANT4_FOUND)

  EXEC_PROGRAM(${GEANT4_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE GEANT4VERSION)

  # ask root-config for the library varaibles
  EXEC_PROGRAM( ${GEANT4_CONFIG_EXECUTABLE} ARGS "--libs" OUTPUT_VARIABLE genie_flags )
  SET(GEANT4_LIBRARIES ${genie_flags} "-lxml2 -llog4cpp")
  # message(STATUS "GEANT4 flags: " ${GEANT4_LIBRARIES})

  message(STATUS "found in ${Geant4_INCLUDE_DIR}")
  message(STATUS "version : ${GEANT4VERSION}")
  # message(STATUS "libraries : ${Geant4_LIBRARIES}")
  # message(STATUS "definitions : ${Gean4_DEFINITIONS} \n")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${GEANT4_LIBRARY_DIR})

else (GEANT4_FOUND)

  message(FATAL_ERROR "Could NOT find Gean4!")

endif (GEANT4_FOUND)
