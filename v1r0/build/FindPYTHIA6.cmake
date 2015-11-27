MESSAGE(STATUS "\n Looking for Pythia6...")

FIND_PATH(PYTHIA6_LIBRARY_DIR   NAMES   libPythia6.so    PATHS
                                                         $ENV{PYTHIA6}
  NO_DEFAULT_PATH
)

if (PYTHIA6_LIBRARY_DIR)

  SET(PYTHIA6_LIBRARIES "-lPythia6")
  message(STATUS "found in ${PYTHIA6_LIBRARY_DIR}")

else (GENIE_FOUND)

  message(FATAL_ERROR "Looking for Pythia6... Could NOT find Pythia6")

endif (PYTHIA6_LIBRARY_DIR)