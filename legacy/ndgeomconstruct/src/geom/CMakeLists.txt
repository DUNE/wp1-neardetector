message(STATUS "geom ... ")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.             GEOM_SOURCE_FILES)

############### build the library #####################
add_library (geom                    ${GEOM_SOURCE_FILES})

target_link_libraries(geom           utils
                                     datastruct
                                     ${ROOT_LIBRARIES}
                                     ${CLHEP_LIBRARIES})


