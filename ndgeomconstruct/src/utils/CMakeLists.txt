message(STATUS "utils ...")

############### make dictionaries #####################
set(INCLUDE_DIRECTORIES             .
                                    ${ROOT_INCLUDE_DIR} 
				    ${CLHEP_INCLUDE_DIR})

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              UTILS_SOURCE_FILES)

############### build the library #####################

#set(CMAKE_CXX_FLAGS)
add_library (utils                  ${UTILS_SOURCE_FILES})

target_link_libraries(utils         ${ROOT_LIBRARIES}
				    ${CLHEP_LIBRARIES})
