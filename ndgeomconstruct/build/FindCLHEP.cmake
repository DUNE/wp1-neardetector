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
                                                        $ENV{CLHEP}/include/
                                                        NO_DEFAULT_PATH)

FIND_PATH(CLHEP_LIBRARY_DIR NAMES libCLHEP.a            PATHS
                                                        $ENV{CLHEP}/lib/
                                                        NO_DEFAULT_PATH)

FIND_PROGRAM(CLHEP_CONFIG_EXECUTABLE NAMES clhep-config PATHS
                                                        $ENV{CLHEP}/bin/
                                                        NO_DEFAULT_PATH)

#message("CLHEP_CONFIG_EXECUTABLE : ${CLHEP_CONFIG_EXECUTABLE}  ")

if (CLHEP_CONFIG_EXECUTABLE)

  EXEC_PROGRAM(${CLHEP_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE CLHEPVERSION)
  message(STATUS "found in ${CLHEP_LIBRARY_DIR}")
  message(STATUS "version : ${CLHEPVERSION}")
  # message(STATUS "libraries : ${CLHEP_LIBRARIES}")
  # message(STATUS "definitions : ${CLHEP_DEFINITIONS}")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${CLHEP_LIBRARY_DIR})

else (CLHEP_CONFIG_EXECUTABLE)

  message(FATAL_ERROR "Could NOT find CLHEP!")

endif (CLHEP_CONFIG_EXECUTABLE)

