# - Try to find CLHEP
# Once done this will define
#
#  CLHEP_FOUND - system has CLHEP
#  CLHEP_INCLUDE_DIR - the CLHEP include directory
#  CLHEP_LIBRARIES - The libraries needed to use CLHEP
#  CLHEP_DEFINITIONS - Compiler switches required for using CLHEP
#

MESSAGE(STATUS "\n Looking for CLHEP...")
                   # Find CLHEP_DIR
#message("CLHEP_LIBRARIES : ${CLHEP_LIBRARIES}")

FIND_PATH(CLHEP_INCLUDE_DIR NAMES CLHEP                 PATHS
                                                        $ENV{CLHEP}/include/Geant4/
                                                        $ENV{CLHEP_INC}/
                                                        NO_DEFAULT_PATH)

#MESSAGE(STATUS "\n\t CLHEP_INCLUDE_DIR set to ${CLHEP_INCLUDE_DIR}")

FIND_PATH(CLHEP_LIBRARY_DIR NAMES libG4clhep.so libG4clhep.dylib PATHS
                                                        	 $ENV{CLHEP}/lib64/
                                                           $ENV{CLHEP}/lib/
								                                           $ENV{CLHEP_LIB_DIR}/
                                                        	 NO_DEFAULT_PATH)

# did not find libG4clhep.so - possibly using a build of CLHEP outside of G4 v10
# so look for libCLHEP.so instead
IF(NOT ${CLHEP_LIBRARY_DIR})
FIND_PATH(CLHEP_LIBRARY_DIR NAMES libCLHEP.so       PATHS
                                                        $ENV{CLHEP}/lib64/
							$ENV{CLHEP_LIB_DIR}/
                                                        NO_DEFAULT_PATH)
ENDIF()

#MESSAGE(STATUS "\n\t CLHEP_LIBRARY_DIR set to ${CLHEP_LIBRARY_DIR}")


#FIND_PROGRAM(CLHEP_CONFIG_EXECUTABLE NAMES clhep-config PATHS
#                                                        $ENV{CLHEP}/bin/
#                                                        NO_DEFAULT_PATH)

if (CLHEP_LIBRARY_DIR)
   message(STATUS "found in ${CLHEP_INCLUDE_DIR}")

else (CLHEP_LIBRARY_DIR)
   message(FATAL_ERROR "Could NOT find CLHEP!")

endif (CLHEP_LIBRARY_DIR)

#message("CLHEP_CONFIG_EXECUTABLE : ${CLHEP_CONFIG_EXECUTABLE}  ")

#if (CLHEP_CONFIG_EXECUTABLE)

#  EXEC_PROGRAM(${CLHEP_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE CLHEPVERSION)
#  message(STATUS "found in ${CLHEP_LIBRARY_DIR}")
#  message(STATUS "version : ${CLHEPVERSION}")
  # message(STATUS "libraries : ${CLHEP_LIBRARIES}")
  # message(STATUS "definitions : ${CLHEP_DEFINITIONS}")

#  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${CLHEP_LIBRARY_DIR})

#else (CLHEP_CONFIG_EXECUTABLE)

#  message(FATAL_ERROR "Could NOT find CLHEP!")

#endif (CLHEP_CONFIG_EXECUTABLE)

