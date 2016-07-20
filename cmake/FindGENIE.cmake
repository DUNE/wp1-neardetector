## --- FindGENIE.cmake ---
## Try to find GENIE's headers and libraries using the genie-config script
## (the user must have set the GENIE environment variable for the script
## to work). Once this is done, the following variables will be defined:
##  - GENIE_FOUND: true if GENIE was found, and false otherwise.
##  - GENIE_LIBRARIES: list of libraries to link against
##  - GENIE_INCLUDE_DIRS: path to headers
##

## Find the GENIE headers
find_path(GENIE_INCLUDE_DIR GHEP
          HINTS $ENV{GENIE}/src $ENV{GENIE}/include
          DOC "Specify the path to the GENIE headers.")
mark_as_advanced(GENIE_INCLUDE_DIR)

## Find the GENIE libraries relying on the information provided 
## by the genie-config script.
find_program(GENIE_CONFIG_EXECUTABLE 
             NAMES genie-config
             HINTS $ENV{GENIE}/bin
             DOC "Specify the path to the GENIE binaries.")

if(EXISTS "${GENIE_CONFIG_EXECUTABLE}")
  execute_process(COMMAND ${GENIE_CONFIG_EXECUTABLE} --libs
                  OUTPUT_VARIABLE GENIE_LIBRARY
                  RESULT_VARIABLE GENIE_LIBRARY_RESULT
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

  ## Parse the library names and directories.
  if("${GENIE_LIBRARY_RESULT}" STREQUAL "0")
    string(REGEX REPLACE " +" ";"
      GENIE_LIBRARY "${GENIE_LIBRARY}")

    ## Look for -L flags for directories and -l flags for library names.
    set(GENIE_LIBRARY_DIRS)
    set(GENIE_LIBRARY_NAMES)
    foreach(flag ${GENIE_LIBRARY})
      if("${flag}" MATCHES "^-L(.+)")
        file(TO_CMAKE_PATH "${CMAKE_MATCH_1}" DIR)
        list(APPEND GENIE_LIBRARY_DIRS "${DIR}")
      elseif("${flag}" MATCHES "^-l(.+)")
        list(APPEND GENIE_LIBRARY_NAMES "${CMAKE_MATCH_1}")
      endif()
    endforeach()

    ## Search for each library needed using the directories given.
    foreach(name ${GENIE_LIBRARY_NAMES})
      find_library(GENIE_${name}_LIBRARY
                   NAMES ${name}
                   HINTS ${GENIE_LIBRARY_DIRS})
      mark_as_advanced(GENIE_${name}_LIBRARY)

      ## If any library is not found then the whole package is not found.
      if(NOT GENIE_${name}_LIBRARY)
        unset(GENIE_LIBRARY)
      endif()

      ## Build an ordered list of all the libraries needed.
      set(GENIE_LIBRARIES ${GENIE_LIBRARIES} "${GENIE_${name}_LIBRARY}")
    endforeach()
  
  else()
    unset(GENIE_LIBRARY)
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GENIE 
                                  FOUND_VAR     GENIE_FOUND
                                  REQUIRED_VARS GENIE_LIBRARY
                                                GENIE_INCLUDE_DIR
                                  FAIL_MESSAGE "Could not find GENIE. Make sure you have set the GENIE environment variable pointing to your GENIE installation.")
mark_as_advanced(GENIE_LIBRARY)

if(GENIE_FOUND)
  set(GENIE_INCLUDE_DIRS ${GENIE_INCLUDE_DIR})
endif()



