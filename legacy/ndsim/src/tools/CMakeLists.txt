message(STATUS "tools ...")

#### Needed for Ubuntu users on 32 bit machines #####
#### Uncomment the lines below if it doesn't compile ##

#SET(GCC_COVERAGE_LINK_FLAGS             "-Wl,--no-as-needed")
#SET(CMAKE_EXE_LINKER_FLAGS              "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

############### build the executables #################
add_executable (lbnoprocess           lbnoProcess.cc)
target_link_libraries (lbnoprocess    processors)

add_executable (lbnotracking          lbnoTracking.cc)
target_link_libraries (lbnotracking   processors
                                      simul)

add_executable(lbnomurange            lbnoMuonRange.cc)
target_link_libraries (lbnomurange    processors)

add_executable (lbnoprimaries         lbnoPrimaries.cc)
target_link_libraries (lbnoprimaries  processors
				      evgen)

add_executable (lbnoinfo              lbnoInfo.cc)
target_link_libraries (lbnoinfo       utils
                                      datastruct)

add_executable (lbnoplot              lbnoPlot.cc)
target_link_libraries (lbnoplot       utils
                                      datastruct)

add_executable (lbnocopytree          copyTree.cc)
target_link_libraries (lbnocopytree   utils
                                      datastruct)

add_executable (lbnoflux              lbnoFlux.cc)
target_link_libraries (lbnoflux       utils
                                      datastruct
				      geom)

add_executable (lbnoreconstruct         lbnoReconstruct.cc)
target_link_libraries (lbnoreconstruct  utils
                                      	datastruct
					geom
					recon)

add_executable (showtracks            showTracks.cc)
target_link_libraries (showtracks     utils
				      recon
                                      datastruct)

add_executable (geanttest             GeantTest.cc)
target_link_libraries (geanttest      processors
				      simul)

add_executable (lbnomemtest           MemTest.cc)
target_link_libraries (lbnomemtest    processors
                                      simul)

#add_executable (geomtest              GeomTest.cc)
#target_link_libraries (geomtest       utils
#                                      datastruct
#                                      simul evgen)
