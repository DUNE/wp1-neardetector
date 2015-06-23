message(STATUS "utils ...")

############### make dictionaries #####################
# set everything needed for the root dictionary and create the
# dictionary

set(GasTPC_LINKDEF                    LinkDef.hh)

set(GasTPC_DICTIONARY                 ${CMAKE_CURRENT_BINARY_DIR}/GasTPCUtilsDict.cxx)

set(GasTPCUtils_HEADERS               GasTPCException.hh)

set(INCLUDE_DIRECTORIES             .
                                    ${ROOT_INCLUDE_DIR} 
				    ${CLHEP_INCLUDE_DIR}
				    ${LOG4CPP_INCLUDE_DIR})

ROOT_GENERATE_DICTIONARY("${GasTPCUtils_HEADERS}" "${GasTPC_LINKDEF}" "${GasTPC_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              UTILS_SOURCE_FILES)

############### build the library #####################

#set(CMAKE_CXX_FLAGS)
add_library (utils                  ${UTILS_SOURCE_FILES}
				    ${GasTPC_DICTIONARY})

target_link_libraries(utils         ${ROOT_LIBRARIES}
				    ${CLHEP_LIBRARIES}
				    ${LOG4CPP_LIBRARIES})
