unset THIRD_PARTY_DIR

export THIRD_PARTY_DIR=${LBNO}/third_party
export THIRD_PARTY_DOWNLOADS=$THIRD_PARTY_DIR/Downloads

if [ ! -d $THIRD_PARTY_DOWNLOADS  ]; then 
  mkdir $THIRD_PARTY_DOWNLOADS 
fi

if [ ! -d "$THIRD_PARTY_DIR/tmp" ]; then
  mkdir $THIRD_PARTY_DIR/tmp
fi

export NCPU=1

### select which versions you want and where to install ###
###  geant4_vmc ###
export G4_VMC_VERSION='2.13a'
export G4_VMC_DIR=${THIRD_PARTY_DIR}/g4vmc-v${G4_VMC_VERSION}
export G4_VMC_INSTALL_DIR=${G4_VMC_DIR}/geant4_vmc
export G4_VMC_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_g4vmc.txt
export G4_VMC_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_g4vmc.txt

export VGM_VERSION='3-06'
export VGM_DIR=${THIRD_PARTY_DIR}/vgm-v${G4_VMC_VERSION}/
export VGM_INSTALL_DIR=${VGM_DIR}
export VGM_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_vgm.txt
export VGM_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_vgm.txt

###  geant4 ###
export GEANT4_VERSION='9.6.p01'
export GEANT4_DIR=${THIRD_PARTY_DIR}/geant4-v${GEANT4_VERSION}
export GEANT4_INSTALL_DIR=${GEANT4_DIR}/geant4-install
export GEANT4_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_geant4.txt
export GEANT4_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_geant4.txt

### xerces-c ###
export XERCESC_VERSION='-3.1.1'
export XERCESC_DIR=${THIRD_PARTY_DIR}/xercesc-v${XERCESC_VERSION}/
export XERCESC_INSTALL_DIR=${XERCESC_DIR}/xercesc-install
export XERCESC_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_xercesc.txt
export XERCESC_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_xercesc.txt

###  root   ###
export ROOT_VERSION='5.34.05'
export ROOT_DIR=${THIRD_PARTY_DIR}/root-v${ROOT_VERSION}/
export ROOT_INSTALL_DIR=${ROOT_DIR}/root
export ROOT_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_root.txt
export ROOT_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_root.txt

###  genie  ###
export GENIE_VERSION='2_6_6'
export GENIE_DIR=${THIRD_PARTY_DIR}/genie-v${GENIE_VERSION}
export GENIE_INSTALL_DIR=${GENIE_DIR}/genie-install
export GENIE_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_genie.txt
export GENIE_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_genie.txt
export GENIE=${GENIE_DIR}/genie
export GSPLOAD=/path/to/splines.xml

###  clhep  ###
export CLHEP_VERSION='2.1.3.1'
export CLHEP_DIR=${THIRD_PARTY_DIR}/clhep-v${CLHEP_VERSION}
export CLHEP_INSTALL_DIR=${CLHEP_DIR}/clhep-install
export CLHEP_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_clhep.txt
export CLHEP_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_clhep.txt

###  cmake  ###
export CMAKE_VERSION='2.8.8'
export CMAKE_DIR=${THIRD_PARTY_DIR}/cmake-v${CMAKE_VERSION}
export CMAKE_INSTALL_DIR=${CMAKE_DIR}/cmake-install
export CMAKE_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_cmake.txt
export CMAKE_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_cmake.txt

### pythia6 ###
export PYTHIA6_VERSION='.424'
export PYTHIA6_DIR=${THIRD_PARTY_DIR}/pythia-v6${PYTHIA6_VERSION}/
export PYTHIA6_INSTALL_DIR=${PYTHIA6_DIR}/pythia-install
export PYTHIA6_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_pythia.txt
export PYTHIA6_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_pythia.txt

### pythia8 ###
export PYTHIA8_VERSION='175'
export PYTHIA8_DIR=${THIRD_PARTY_DIR}/pythia-v8${PYTHIA8_VERSION}/
export PYTHIA8_INSTALL_DIR=${PYTHIA8_DIR}
export PYTHIA8_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_pythia8.txt
export PYTHIA8_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_pythia8.txt

### lhapdf  ###
export LHAPDF_VERSION='5.8.7'
export LHAPDF_DIR=${THIRD_PARTY_DIR}/lhapdf-v${LHAPDF_VERSION}
export LHAPDF_INSTALL_DIR=${LHAPDF_DIR}/lhapdf-install
export LHAPDF_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_lhapdf.txt
export LHAPDF_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_lhapdf.txt

### log4cpp  ###
export LOG4CPP_VERSION='1.1'
export LOG4CPP_DIR=${THIRD_PARTY_DIR}/log4cpp-v${LOG4CPP_VERSION}
export LOG4CPP_INSTALL_DIR=${LOG4CPP_DIR}/log4cpp-install
export LOG4CPP_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_log4cpp.txt
export LOG4CPP_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_log4cpp.txt

### libxml2 ###
export LIBXML2_VERSION='2.8.0'
export LIBXML2_DIR=${THIRD_PARTY_DIR}/libxml2-v${LIBXML2_VERSION}
export LIBXML2_INSTALL_DIR=${LIBXML2_DIR}/libxml2-install
export LIBXML2_INSTALL_LOG=${THIRD_PARTY_DIR}/tmp/log_install_libxml2.txt
export LIBXML2_INSTALL_ERR=${THIRD_PARTY_DIR}/tmp/err_install_libxml2.txt


### write these directories to env.sh

if [ -f ${LBNO}/env.sh ];then
  echo "env.sh already exists. Overwriting the current env.sh file."
fi

cat > ${LBNO}/env.sh <<EOF
export LOG4CPP_INSTALL_DIR=${LOG4CPP_INSTALL_DIR}
export LHAPDF_INSTALL_DIR=${LHAPDF_INSTALL_DIR}
export LIBXML2_INSTALL_DIR=${LIBXML2_INSTALL_DIR}
export PYTHIA8_INSTALL_DIR=${PYTHIA8_INSTALL_DIR}
export PYTHIA6_INSTALL_DIR=${PYTHIA6_INSTALL_DIR}
export CLHEP_INSTALL_DIR=${CLHEP_INSTALL_DIR}
export GENIE_INSTALL_DIR=${GENIE_INSTALL_DIR}
export ROOT_INSTALL_DIR=${ROOT_INSTALL_DIR}
export GEANT4_INSTALL_DIR=${GEANT4_INSTALL_DIR}
export XERCESC_INSTALL_DIR=${XERCESC_INSTALL_DIR}
export G4_VMC_INSTALL_DIR=${G4_VMC_INSTALL_DIR}
export VGM_INSTALL_DIR=${VGM_INSTALL_DIR}

## GENIE required variables ###
export GENIE=${GENIE}
export GSPLOAD=${GSPLOAD}
EOF


#end
