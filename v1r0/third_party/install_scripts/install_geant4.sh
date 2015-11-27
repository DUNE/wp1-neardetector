SUCCESS=0

echo "going to place geant4."${GEANT4_VERSION} "  in "${GEANT4_DIR}

###################################################################################

if [ -d ${GEANT4_DIR} ];then
  echo "directory ${GEANT4_DIR} already exists. going to remove it."
  rm -rf ${GEANT4_DIR}
fi

mkdir -p ${GEANT4_DIR}
cd ${GEANT4_DIR}
wget http://geant4.cern.ch/support/source/geant4.${GEANT4_VERSION}.tar.gz \
1>${GEANT4_INSTALL_LOG} 2>${GEANT4_INSTALL_ERR}

if [ -f geant4.${GEANT4_VERSION}.tar.gz ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

tar xvfz geant4.${GEANT4_VERSION}.tar.gz 1>>${GEANT4_INSTALL_LOG} 2>>${GEANT4_INSTALL_ERR}
mv geant4.${GEANT4_VERSION}.tar.gz ${THIRD_PARTY_DOWNLOADS}
echo "extract done"

###################################################################################

mkdir geant4-build
cd geant4-build
cmake -DCMAKE_INSTALL_PREFIX=${GEANT4_INSTALL_DIR} -DGEANT4_USE_GDML=ON -DGEANT4_INSTALL_DATA=ON \
-DGEANT4_USE_G3TOG4=ON -DGEANT4_USE_OPENGL_X11=ON -DXERCESC_ROOT_DIR=${XERCESC_INSTALL_DIR} \
../geant4.${GEANT4_VERSION} 1>>${GEANT4_INSTALL_LOG} 2>>${GEANT4_INSTALL_ERR}

if [ -f Makefile ]; then
  echo "configure done"
else
  echo -e "configure unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

echo "Building....."

make -j${NCPU} 1>>${GEANT4_INSTALL_LOG} 2>>${GEANT4_INSTALL_ERR}

if [ -f outputs/library/Linux-g++/libG4OpenGL.so ]; then
  echo "build done"
else
  echo -e "build unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

make install  1>>${GEANT4_INSTALL_LOG} 2>>${GEANT4_INSTALL_ERR}

if [ -d ${GEANT4_INSTALL_DIR}/bin ]; then
  echo "geant installed in "${GEANT4_INSTALL_DIR}
else 
  echo -e "install unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

cd ${THIRD_PARTY_DIR}

SUCCESS=1

