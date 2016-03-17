# - Try to find LOG4CPP
# Once done this will define
#
#  LOG4CPP_FOUND - system has LOG4CPP
#  LOG4CPP_INCLUDE_DIR - the LOG4CPP include directory
#  LOG4CPP_LIBRARIES - The libraries needed to use LOG4CPP
#  LOG4CPP_DEFINITIONS - Compiler switches required for using LOG4CPP
#

MESSAGE(STATUS "\n Looking for LOG4CPP...")
                   # Find LOG4CPP_DIR
#message("LOG4CPP_LIBRARIES : ${LOG4CPP_LIBRARIES}")

FIND_PATH(LOG4CPP_INCLUDE_DIR NAMES log4cpp             PATHS
                                                        $ENV{LOG4CPP}/include/
                                                        NO_DEFAULT_PATH)

#MESSAGE("LOG4CPP_INCLUDE_DIR is ${LOG4CPP_INCLUDE_DIR}")

FIND_PATH(LOG4CPP_LIBRARY_DIR NAMES liblog4cpp.so liblog4cpp.dylib    PATHS
                                                        $ENV{LOG4CPP}/lib64/
                                                        $ENV{LOG4CPP}/lib/
                                                        NO_DEFAULT_PATH)

#MESSAGE("LOG4CPP_LIBRARY_DIR is ${LOG4CPP_LIBRARY_DIR}")

FIND_PROGRAM(LOG4CPP_CONFIG_EXECUTABLE NAMES log4cpp-config-x86_64 PATHS
                                                        $ENV{LOG4CPP}/bin/
                                                        $ENV{LOG4CPP_BIN}/
                                                        NO_DEFAULT_PATH)
# check for an alternate form of the executable name
IF(NOT ${LOG4CPP_CONFIG_EXECUTABLE})
FIND_PROGRAM(LOG4CPP_CONFIG_EXECUTABLE NAMES log4cpp-config PATHS
                                                        $ENV{LOG4CPP}/bin/
                                                        $ENV{LOG4CPP_BIN}/
                                                        NO_DEFAULT_PATH)
ENDIF()

#message("LOG4CPP_CONFIG_EXECUTABLE : ${LOG4CPP_CONFIG_EXECUTABLE}  ")

if (LOG4CPP_CONFIG_EXECUTABLE)

  EXEC_PROGRAM(${LOG4CPP_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE LOG4CPPVERSION)
  message(STATUS "found in ${LOG4CPP_LIBRARY_DIR}")
  message(STATUS "version : ${LOG4CPPVERSION}")
  # message(STATUS "libraries : ${LOG4CPP_LIBRARIES}")
  # message(STATUS "definitions : ${LOG4CPP_DEFINITIONS}")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${LOG4CPP_LIBRARY_DIR})
  SET(LOG4CPP_LIBRARIES "-llog4cpp")

else (LOG4CPP_CONFIG_EXECUTABLE)

  message(FATAL_ERROR "Could NOT find LOG4CPP!")

endif (LOG4CPP_CONFIG_EXECUTABLE)

