message(STATUS "geombuildernew ... ")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.             GEOMNEW_SOURCE_FILES)

############### build the library #####################
add_library (geombuildernew                    ${GEOMNEW_SOURCE_FILES})

target_link_libraries(geombuildernew	utils
				        datastruct
                                     	${ROOT_LIBRARIES}
                                     	${CLHEP_LIBRARIES})


