#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

message(STATUS "simul ... ")

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

add_library (simul             Geant4TrackingAlgorithm.cc
                               EventFilterAlgorithm.cc
                               PhysicsList.cc
                               EventAction.cc
                               RunAction.cc
                               SteppingAction.cc
			       ApplyCuts.cc
			       StackingAction.cc
                               TrackingAction.cc
			       TrackInformation.cc
                               PrimaryGeneratorAction.cc
                               DetectorConstruction.cc
			       MyUserPostDetConstruction.cc
			       DetectorHit.cc
			       DetectorSD.cc
			       MagneticField.cc
                               MINDSensitiveDetector.cc
                               TpcFidSensitiveDetector.cc
                               ScintillatorSensitiveDetector.cc)

target_link_libraries(simul    ${Geant4_LIBRARIES}
                               ${ROOT_LIBRARIES}
                               ${CLHEP_LIBRARIES}
                               g4root
                               #geant4vmc
                               #G3toG4
                               #VMC
                               #Geant4GM
                               )

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

#add_executable(lbnondsimul lbnoSimul.cc    PhysicsList.cc
#                                           PrimaryGeneratorAction.cc
#                                           EventAction.cc
#                                           GHEPFileReader.cc
#                                           RunAction.cc
#                                           RunManager.cc
#                                           SteppingAction.cc
#                                           TrackingAction.cc)

#target_link_libraries(lbnondsimul          ${Geant4_LIBRARIES}
#                                           ${ROOT_LIBRARIES}
#                                           ${GENIE_LIBRARIES}
#                                           ${CLHEP_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SIMUL_SCRIPTS              vis.mac)

foreach(_script ${SIMUL_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/simul/${_script}
    ${PROJECT_BINARY_DIR}/simul/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS lbnondsimul DESTINATION bin)


