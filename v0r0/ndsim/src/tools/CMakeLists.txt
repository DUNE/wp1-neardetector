message(STATUS "tools ...")

#### Needed for Ubuntu users on 32 bit machines #####
#### Uncomment the lines below if it doesn't compile ##

#SET(GCC_COVERAGE_LINK_FLAGS             "-Wl,--no-as-needed")
#SET(CMAKE_EXE_LINKER_FLAGS              "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

############### build the executables #################
add_executable (GasTPCTracking          GasTPCTracking.cc)
target_link_libraries (GasTPCTracking   processors
                                        simul)

add_executable (GasTPCParticleGun	        GasTPCParticleGun.cc)
target_link_libraries (GasTPCParticleGun        processors
                                     		simul)

add_executable (GasTPCMemtest           MemTest.cc)
target_link_libraries (GasTPCMemtest    processors
                                      	simul)

add_executable (GasTPCGeombuild		   GasTPCGeombuild.cc)
target_link_libraries (GasTPCGeombuild	   utils
					   datastruct
					   geombuilder)

add_executable (GasTPCAnalysis           GasTPCAnalysis.cc)
target_link_libraries (GasTPCAnalysis    utils
		      		         datastruct
					 geom
					 recon)
