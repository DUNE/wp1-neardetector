message(STATUS "tools ...")

#### Needed for Ubuntu users on 32 bit machines #####
#### Uncomment the lines below if it doesn't compile ##

#SET(GCC_COVERAGE_LINK_FLAGS             "-Wl,--no-as-needed")
#SET(CMAKE_EXE_LINKER_FLAGS              "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

############### build the executables #################
#add_executable (GasTPCprocess           GasTPCProcess.cc)
#target_link_libraries (GasTPCprocess    processors)

#add_executable (GasTPCtracking          GasTPCTracking.cc)
#target_link_libraries (GasTPCtracking   processors
#                                        simul)

#add_executable(GasTPCmurange            GasTPCMuonRange.cc)
#target_link_libraries (GasTPCmurange    processors)

add_executable (GasTPCprimaries         GasTPCPrimaries.cc)
target_link_libraries (GasTPCprimaries  processors
				        evgen)

#add_executable (GasTPCinfo              GasTPCInfo.cc)
#target_link_libraries (GasTPCinfo       utils
#                                        datastruct)

#add_executable (GasTPCParticleGun	        GasTPCParticleGun.cc)
#target_link_libraries (GasTPCParticleGun        processors
#                                      		simul)

#add_executable (GasTPCplot              GasTPCPlot.cc)
#target_link_libraries (GasTPCplot       utils
#                                      	datastruct
#					geom)

#add_executable (GasTPCgunplot           gunPlot.cc)
#target_link_libraries (GasTPCgunplot    utils
#				      	recon
#                                      	datastruct
#				      	geom)

#add_executable (GasTPCcopytree          copyTree.cc)
#target_link_libraries (GasTPCcopytree   utils
#                                     	datastruct
#					geom)

#add_executable (GasTPCflux              GasTPCFlux.cc)
#target_link_libraries (GasTPCflux       utils
#                                     	datastruct
#				      	geom)

#add_executable (GasTPCreconstruct         GasTPCReconstruct.cc)
#target_link_libraries (GasTPCreconstruct  utils
#                                     	  datastruct
#					  geom
#					  recon)

#add_executable (showtracks            showTracks.cc)
#target_link_libraries (showtracks     utils
#				      recon
#                                      datastruct
#				      geom)

#add_executable (geanttest             GeantTest.cc)
#target_link_libraries (geanttest      processors
#				      simul)

add_executable (GasTPCmemtest           MemTest.cc)
target_link_libraries (GasTPCmemtest    processors
                                      	simul)

add_executable (geombuild           geombuilder.cc)
target_link_libraries (geombuild    utils
		      		    datastruct
				    geombuilder)

#add_executable (geomtest              GeomTest.cc)
#target_link_libraries (geomtest       utils
#                                      datastruct
#                                      simul evgen)
