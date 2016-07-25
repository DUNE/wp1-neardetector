### Check if cmake has the required version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

### Set the project name
project(GASTPC)

###  Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Append to the CMAKE_MODULE_PATH variable the path to the directory
### with additional CMake modules
set(CMAKE_MODULE_PATH "${GASTPC_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

### Look for external dependencies
find_package(Geant4 REQUIRED vis_opengl_x11)
find_package(ROOT REQUIRED Geom EGPythia6)
find_package(GENIE REQUIRED)
find_package(LibXml2)
find_package(Log4cpp)

include(${Geant4_USE_FILE})

if(EXISTS ${ROOT_USE_FILE})
  ### ROOT found using the ROOTConfig.cmake module 
  include(${ROOT_USE_FILE})
else()
  ### ROOT found using the FindROOT.cmake module
  include_directories(${ROOT_INCLUDE_DIRS})
endif()

include_directories(${GENIE_INCLUDE_DIRS})

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(gastpc)

