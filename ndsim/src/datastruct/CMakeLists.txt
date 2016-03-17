message(STATUS "datastruct ... ")

############### make dictionaries #####################
# set everything needed for the root dictionary and create the
# dictionary

set(GasTPCData_LINKDEF                    LinkDef.hh)

set(GasTPCData_DICTIONARY                 ${CMAKE_CURRENT_BINARY_DIR}/GasTPCDataDict.cxx)

set(GasTPCData_HEADERS               RunStats.hh
                                      GasTPCDataLib.hh)

set(INCLUDE_DIRECTORIES             ../utils
                                    ${ROOT_INCLUDE_DIR}
                                    ${CLHEP_INCLUDE_DIR}
				    ${Geant4_INCLUDE_DIR})


ROOT_GENERATE_DICTIONARY("${GasTPCData_HEADERS}" "${GasTPCData_LINKDEF}" "${GasTPCData_DICTIONARY}" "${INCLUDE_DIRECTORIES}")


# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              DATASTR_SOURCE_FILES)

############### build the library #####################

#set(CMAKE_CXX_FLAGS)
add_library (datastruct             ${DATASTR_SOURCE_FILES}
                                    ${GasTPCData_DICTIONARY})

target_link_libraries(datastruct    utils
                                    ${ROOT_LIBRARIES}
                                    ${CLHEP_LIBRARIES}
				    ${Geant4_LIBRARIES})

