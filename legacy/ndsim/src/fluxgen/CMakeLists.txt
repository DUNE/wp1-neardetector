message(STATUS "fluxgen ... ")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              PROCESSORS_SOURCE_FILES)

############### build the library #####################
add_library (fluxgen                ${PROCESSORS_SOURCE_FILES})

target_link_libraries(fluxgen       utils
                                    ${ROOT_LIBRARIES}
                                    ${CLHEP_LIBRARIES})

