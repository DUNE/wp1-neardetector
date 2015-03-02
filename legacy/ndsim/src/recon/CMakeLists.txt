message(STATUS "recon ...")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              RECON_SOURCE_FILES)

############### build the library #####################

#set(CMAKE_CXX_FLAGS)
add_library (recon                  ${RECON_SOURCE_FILES})

target_link_libraries(recon         utils
				    datastruct
                                    ${ROOT_LIBRARIES}
                                    ${CLHEP_LIBRARIES})
