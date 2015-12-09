# - Try to find LHAPDF
# Once done this will define
#
#  LHAPDF_FOUND - system has LHAPDF
#  LHAPDF_INCLUDE_DIR - the LHAPDF include directory
#  LHAPDF_LIBRARIES - The libraries needed to use LHAPDF
#  LHAPDF_DEFINITIONS - Compiler switches required for using LHAPDF
#

MESSAGE(STATUS "\n Looking for LHAPDF...")
                   # Find LHAPDF_DIR
#message("LHAPDF_LIBRARIES : ${LHAPDF_LIBRARIES}")

FIND_PATH(LHAPDF_INCLUDE_DIR NAMES   LHAPDF                  PATHS
                                                             $ENV{LHAPDF_INSTALL_DIR}/include/
                                                             NO_DEFAULT_PATH)

FIND_PATH(LHAPDF_LIBRARY_DIR NAMES   libLHAPDF.so            PATHS
                                                             $ENV{LHAPDF_INSTALL_DIR}/lib/
                                                             NO_DEFAULT_PATH)

FIND_PROGRAM(LHAPDF_CONFIG_EXECUTABLE NAMES  lhapdf-config   PATHS
                                                             $ENV{LHAPDF_INSTALL_DIR}/bin/
                                                             NO_DEFAULT_PATH)

#message("LHAPDF_CONFIG_EXECUTABLE : ${LHAPDF_CONFIG_EXECUTABLE}  ")

if (LHAPDF_CONFIG_EXECUTABLE)

  EXEC_PROGRAM(${LHAPDF_CONFIG_EXECUTABLE} ARGS "--version"  OUTPUT_VARIABLE LHAPDFVERSION)
  message(STATUS "found in ${LHAPDF_LIBRARY_DIR}")
  message(STATUS "version : ${LHAPDFVERSION}")
  # message(STATUS "libraries : ${LHAPDF_LIBRARIES}")
  # message(STATUS "definitions : ${LHAPDF_DEFINITIONS}")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${LHAPDF_LIBRARY_DIR})

else (LHAPDF_CONFIG_EXECUTABLE)

  message(FATAL_ERROR "Could NOT find LHAPDF!")

endif (LHAPDF_CONFIG_EXECUTABLE)

