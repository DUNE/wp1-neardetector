# Check if cmake has the required version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

# Set name of our project to "geometry". Has to be done
project(geometry)

# Load some basic macros which are needed later on
MESSAGE(STATUS "\n Looking for third party libs ...")

include($ENV{LBNO}/build/FindCLHEP.cmake)
include($ENV{LBNO}/build/FindROOT.cmake)

MESSAGE(STATUS "\n Third party is loaded.")

#Check the compiler and set the compile and link flags
set(CMAKE_BUILD_TYPE Debug)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")


include_directories(.  utils
		       geom
                       datastruct
		       tools
                       "${ROOT_INCLUDE_DIR}" 
                       "${CLHEP_INCLUDE_DIR}")

link_directories(      "${ROOT_LIBRARY_DIR}"
                       "${CLHEP_LIBRARY_DIR}")

MESSAGE(STATUS "\n Entering directories ...")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utils)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/geom)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/datastruct)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools)

MESSAGE(STATUS "\n Done ...")
