#!/bin/bash

# This is an example on how to set up the enviromental variables needed for GasTPC simulation
 
export GasTPC=${PWD}
export ParentDir=/your/path

if [ ! -z  "${GasTPC}" ]; then
    # CMAKE >= v2.8.0 
    export PATH=${ParentDir}/cmake-2.8.6/bin:$PATH
    # GENIE
    export GENIE=${ParentDir}/gastpc/genie/R-2_8_4
    # ROOT
    source ${ParentDir}/gastpc/root/root_v5.34.26/bin/thisroot.sh
    # GEANT
    export Geant4_DIR=${ParentDir}/gastpc/geant/geant4.10.01-install
    source ${Geant4_DIR}/share/Geant4-10.1.0/geant4make/geant4make.sh
    # CLHEP
    export CLHEP=${Geant4_DIR}
    # PYTHIA
    export PYTHIA6=${ParentDir}/genie/R-2_8_0/src/scripts/build/ext/v6_428/lib
    # LHAPPDF
    export LHAPDF_INSTALL_DIR=${ParentDir}/lhapdf-5.9.1/stage
    #export LHAPATH=${LHAPDF_INSTALL_DIR}/share/lhapdf/PDFsets
    export LHAPATH=$GENIE/data/evgen/pdfs
    export LHAPDF_INC=${LHAPDF_INSTALL_DIR}/include
    export LHAPDF_LIB=${LHAPDF_INSTALL_DIR}/lib
    # LIBXML
    export LIBXML_INC=${ParentDir}/gastpc/libxml2/libxml2-2.9.2-install/include/libxml2
    export LIBXML_LIB=${ParentDir}/gastpc/libxml2/libxml2-2.9.2-install/lib
    # GENIE SPLINES
    #export GSPLOAD=${ParentDir}/genie/R-2_8_4/xssplines/gxspl-NuMIbig-v2.8.4.xml
    export GMSGCONF=${GasTPC}/src/evgen/GasTPC-genieMessenger.xml
    # GEANT4_VMC
    export G4_VMC_INSTALL_DIR=${ParentDir}/gastpc/geant4_vmc/geant4_vmc.3.1.p1-install
    export VGM_DIR=${ParentDir}/gastpc/geant4_vmc/vgm-install
    export VGM_INSTALL_DIR=${VGM_DIR}

    #DK2NU
    export DK2NU=path/to/dk2nu/trunk/dk2nu/dk2nu-install
    export LIBXML2INC=${LIBXML_INC}
 
    export PATH=$GENIE/bin:$ROOTSYS/bin:${GasTPC}/build/bin:${PATH}

    export LD_LIBRARY_PATH=$LHAPDF_LIB:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=$LIBXML_LIB:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=$PYTHIA6:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=$GENIE/lib:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=$DK2NU/lib:$LD_LIBRARY_PATH
    
    # XERCESC
    export XERCESC_INSTALL_DIR=/usr

    # LOG4CPP
    export LOG4CPP=/usr

  echo "SUCCESS: GasTPC setup"
else
  echo "WARNING: GasTPC not set - make sure to set the GasTPC path first before running this script!"
fi
