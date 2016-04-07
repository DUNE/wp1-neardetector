MESSAGE(STATUS "\n Looking for XERCES-C ...")
                   # Find CLHEP_DIR


FIND_PATH(XERCESC_INCLUDE_DIR NAMES    xercesc          PATHS
                                                        $ENV{XERCESC_INSTALL_DIR}/include/
                                                        NO_DEFAULT_PATH)

FIND_PATH(XERCESC_LIBRARY_DIR NAMES libxerces-c.so libxerces-c.dylib   PATHS
                                                        $ENV{XERCESC_INSTALL_DIR}/lib64/
                                                        $ENV{XERCESC_INSTALL_DIR}/lib/
                                                        NO_DEFAULT_PATH)


#message("XERCESC_LIBRARY_DIR : ${XERCESC_LIBRARY_DIR}  ")

if (XERCESC_LIBRARY_DIR AND XERCESC_INCLUDE_DIR)

  message(STATUS "found in ${XERCESC_LIBRARY_DIR}")
  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${XERCESC_LIBRARY_DIR})

else (XERCESC_LIBRARY_DIR AND XERCESC_INCLUDE_DIR)

  message(FATAL_ERROR "Could NOT find XERCES-C!")

endif (XERCESC_LIBRARY_DIR AND XERCESC_INCLUDE_DIR)
