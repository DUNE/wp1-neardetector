message(STATUS "emcalc ... ")

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

add_library (emcalc            RangeEstimationAlgorithm.cc
                               EMCalcPhysicsList.cc
                               #EMCalcRunAction.cc
                               RunAction.cc
                               EMCalcPrimaryGeneratorAction.cc
                               EMCalcDetectorConstruction.cc
                               DetectorMessenger.cc
                               PhysicsListMessenger.cc
                               PhysListEmStandard.cc)

target_link_libraries(emcalc   ${Geant4_LIBRARIES}
                               ${ROOT_LIBRARIES}
                               ${CLHEP_LIBRARIES})