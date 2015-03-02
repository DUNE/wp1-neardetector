message(STATUS "utils ...")

############### make dictionaries #####################
# set everything needed for the root dictionary and create the
# dictionary

set(lbno_LINKDEF                    LinkDef.hh)

set(lbno_DICTIONARY                 ${CMAKE_CURRENT_BINARY_DIR}/lbnoUtilsDict.cxx)

set(lbnoUtils_HEADERS               LbnoException.hh)

set(INCLUDE_DIRECTORIES             .
                                    ${ROOT_INCLUDE_DIR} 
				    ${CLHEP_INCLUDE_DIR})

ROOT_GENERATE_DICTIONARY("${lbnoUtils_HEADERS}" "${lbno_LINKDEF}" "${lbno_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              UTILS_SOURCE_FILES)

############### build the library #####################

#set(CMAKE_CXX_FLAGS)
add_library (utils                  ${UTILS_SOURCE_FILES}
				    ${lbno_DICTIONARY})

target_link_libraries(utils         ${ROOT_LIBRARIES}
				    ${CLHEP_LIBRARIES})
