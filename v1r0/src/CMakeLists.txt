# Check if cmake has the required version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)

#set the compiler for gcc
#set(CMAKE_CXX_COMPILER "/batchsoft/gcc/gcc-4.8.1-x86_64-sl6/bin/gcc")
#set(CMAKE_CXX_FLAGS LIBS=-/batchsoft/gcc/gcc-4.8.1-x86_64-sl6/lib64/libstdc++.so.6)

#set(CMAKE_C_COMPILER "/batchsoft/gcc/gcc-4.8.1-x86_64-sl6/bin/gcc")

# Set name of our project to "Gas-TPC". Has to be done
project(Gas-TPC)

#include(FortranCInterface)

# Load some basic macros which are needed later on
MESSAGE(STATUS "\n Looking for third party libs ...")
#MESSAGE(STATUS "$ENV{GasTPC}/build/ ...")

include($ENV{GasTPC}/build/FindCLHEP.cmake)
include($ENV{GasTPC}/build/FindPYTHIA6.cmake)
#include($ENV{GasTPC}/build/FindPYTHIA8.cmake)
include($ENV{GasTPC}/build/FindLOG4CPP.cmake)
include($ENV{GasTPC}/build/FindXERCESC.cmake)
include($ENV{GasTPC}/build/FindLHAPDF.cmake)
include($ENV{GasTPC}/build/FindROOT.cmake)
include($ENV{GasTPC}/build/FindGENIE.cmake)
include($ENV{GasTPC}/build/FindGEANT4.cmake)
include($ENV{GasTPC}/build/FindVMC.cmake)

MESSAGE(STATUS "\n Third party is loaded.")

#Check the compiler and set the compile and link flags
set(CMAKE_BUILD_TYPE Debug)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")


include_directories(.  utils
                       geom
                       evgen
		       recon
                       simul
                       datastruct
                       processors
		       geombuilder
                       "${ROOT_INCLUDE_DIR}"
                       "${GEANT4_INCLUDE_DIR}"
                       "${GENIE_INCLUDE_DIR}"
                       "${CLHEP_INCLUDE_DIR}"
                       "${LOG4CPP_INCLUDE_DIR}"
                       "${XERCESC_INCLUDE_DIR}"
                       "${LHAPDF_INCLUDE_DIR}"
                       "${GEANT4VMC_INCLUDE_DIR}"
                       "${G4ROOT_INCLUDE_DIR}")

link_directories(      "${ROOT_LIBRARY_DIR}"
                       "${GEANT4_LIBRARY_DIR}"
                       "${GENIE_LIBRARY_DIR}"
                       "${CLHEP_LIBRARY_DIR}"
                       "${LOG4CPP_LIBRARY_DIR}"
                       "${PYTHIA6_LIBRARY_DIR}"
                       "${XERCESC_LIBRARY_DIR}"
                       "${LHAPDF_LIBRARY_DIR}"
                       "${GEANT4VMC_LIBRARY_DIR}"
                       "${G4ROOT_LIBRARY_DIR}")

MESSAGE(STATUS "\n Entering directories ...")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utils)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/geom)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/evgen)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/recon)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simul)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/datastruct)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/geombuilder)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/processors)

MESSAGE(STATUS "\n Done ...")
