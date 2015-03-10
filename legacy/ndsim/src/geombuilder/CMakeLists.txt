message(STATUS "geombuilder ... ")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.             GEOM_SOURCE_FILES)

############### build the library #####################
add_library (geombuilder                    ${GEOM_SOURCE_FILES})

target_link_libraries(geombuilder	utils
				        datastruct
                                     	${ROOT_LIBRARIES}
                                     	${CLHEP_LIBRARIES})


