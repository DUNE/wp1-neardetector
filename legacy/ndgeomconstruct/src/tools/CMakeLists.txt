message(STATUS "tools ...")

#### Needed for Ubuntu users on 32 bit machines #####
#### Uncomment the lines below if it doesn't compile ##

#SET(GCC_COVERAGE_LINK_FLAGS             "-Wl,--no-as-needed")
#SET(CMAKE_EXE_LINKER_FLAGS              "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

############### build the executables #################
add_executable (geomconstruct           construct.cc)
target_link_libraries (geomconstruct    geom)

add_executable (geomdraw           draw.cc)
target_link_libraries (geomdraw    geom)
