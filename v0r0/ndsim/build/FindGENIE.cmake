# - Try to find GENIE
# Once done this will define
#
#  GENIE_FOUND - system has GENIE
#  GENIE_INCLUDE_DIR - the GENIE include directory
#  GENIE_LIBRARIES - The libraries needed to use GENIE
#  GENIE_DEFINITIONS - Compiler switches required for using GENIE
#

MESSAGE(STATUS "\n Looking for GENIE...")

if (GENIE_INCLUDE_DIR AND GENIE_LIBRARY_DIR)
  SET (GENIE_INCLUDE_DIR GENIE_INCLUDE_DIR-NOTFOUND)
  SET (GENIE_LIB_DIR GENIE_LIB_DIR-NOTFOUND)
  SET (GENIE_PLISTS_LIB_DIR GENIE_PLISTS_LIB_DIR-NOTFOUND)
endif (GENIE_INCLUDE_DIR AND GENIE_LIBRARY_DIR)


FIND_PATH(GENIE_INCLUDE_DIR   NAMES    GHEP             PATHS
                                                        $ENV{GENIE}/src
                                                        NO_DEFAULT_PATH
)
#message("GENIE_INCLUDE_DIR : ${GENIE_INCLUDE_DIR}")

FIND_PATH(GENIE_LIBRARY_DIR  NAMES   libGBase.so         PATHS
                                                         $ENV{GENIE}/lib/
                                                         NO_DEFAULT_PATH
)
#message("GENIE_LIBRARY_DIR : ${GENIE_LIBRARY_DIR}")

FIND_PROGRAM(GENIE_CONFIG_EXECUTABLE NAMES genie-config  PATHS
                                                         $ENV{GENIE}/bin
                                                         NO_DEFAULT_PATH)
#message("GENIE_CONFIG_EXECUTABLE : ${GENIE_CONFIG_EXECUTABLE}")


if (GENIE_INCLUDE_DIR AND GENIE_LIBRARY_DIR AND GENIE_CONFIG_EXECUTABLE)
   set(GENIE_FOUND TRUE)
endif (GENIE_INCLUDE_DIR AND GENIE_LIBRARY_DIR AND GENIE_CONFIG_EXECUTABLE)

if (GENIE_FOUND)

  EXEC_PROGRAM(${GENIE_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE GENIEVERSION)

  # ask genie-config for the library varaibles
  EXEC_PROGRAM( ${GENIE_CONFIG_EXECUTABLE}
    ARGS "--libs" 
    OUTPUT_VARIABLE genie_flags )
  SET(GENIE_LIBRARIES ${genie_flags} "-lxml2 -llog4cpp")
  # message(STATUS "GENIE flags: " ${GENIE_LIBRARIES})

  message(STATUS "found in ${GENIE_INCLUDE_DIR}")
  message(STATUS "version : ${GENIEVERSION}")
  # message(STATUS "libraries : ${GENIE_LIBRARIES} \n")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${GENIE_LIBRARY_DIR})

else (GENIE_FOUND)

  message(FATAL_ERROR "Looking for GENIE... Could NOT find GENIE")

endif (GENIE_FOUND)

