MESSAGE(STATUS "\n Looking for Pythia8...")

FIND_PATH(PYTHIA8_LIBRARY_DIR   NAMES   libPythia8.so    PATHS
                                                         $ENV{PYTHIA8}/lib
  NO_DEFAULT_PATH
)

if (PYTHIA8_LIBRARY_DIR)

  SET(PYTHIA8_LIBRARIES "-lPythia8")
  message(STATUS "found in ${PYTHIA8_LIBRARY_DIR}")

else (GENIE_FOUND)

  message(FATAL_ERROR "Looking for Pythia8... Could NOT find Pythia8")

endif (PYTHIA8_LIBRARY_DIR)
