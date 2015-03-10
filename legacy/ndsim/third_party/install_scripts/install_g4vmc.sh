SUCCESS=0

echo "going to place geant4_vmc"${G4_VMC_VERSION} "  in " ${G4_VMC_DIR}

###################################################################################

if [ -d ${G4_VMC_DIR} ];then
  echo "directory ${G4_VMC_DIR} already exists. going to remove it."
  rm -rf ${G4_VMC_DIR}
fi

mkdir -p ${G4_VMC_DIR}
cd ${G4_VMC_DIR}

wget ftp://root.cern.ch/root/vmc/geant4_vmc.${G4_VMC_VERSION}.tar.gz 1>$G4_VMC_INSTALL_LOG 2>$G4_VMC_INSTALL_ERR

if [ -f geant4_vmc.${G4_VMC_VERSION}.tar.gz ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

tar xvfz geant4_vmc.${G4_VMC_VERSION}.tar.gz 1>>${G4_VMC_INSTALL_LOG} 2>>${G4_VMC_INSTALL_ERR}
mv geant4_vmc.${G4_VMC_VERSION}.tar.gz ${THIRD_PARTY_DOWNLOADS}
echo "extract done"

###################################################################################

cd geant4_vmc
export G4INCLUDE=$GEANT4_INSTALL_DIR/include/Geant4/
export G4INSTALL=$GEANT4_INSTALL_DIR
export CLHEP_BASE_DIR=$CLHEP_INSTALL_DIR
source ${ROOT_DIR}/root/bin/thisroot.sh

echo "Building...."

make -j${NCPU} 1>>${G4_VMC_INSTALL_LOG} 2>>${G4_VMC_INSTALL_ERR}

if [ -f lib/tgt_linuxx8664gcc/libgeant4vmc.so ]; then echo "build done"
else
  echo -e "build unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

SUCCESS=1

