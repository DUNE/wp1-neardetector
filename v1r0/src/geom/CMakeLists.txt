message(STATUS "geometry ... ")

############### build the library #####################
add_library (geom                   GeometryLoader.cc)

target_link_libraries(geom          utils
				    datastruct
                                    ${ROOT_LIBRARIES}
                                    ${CLHEP_LIBRARIES})

