SUCCESS=0

echo "going to place VGM"${VGM_VERSION} "  in " ${VGM_DIR}

###################################################################################

if [ -d ${VGM_DIR} ];then
  echo "directory ${VGM_DIR} already exists. going to remove it."
  rm -rf ${VGM_DIR}
fi

mkdir -p ${VGM_DIR}
cd ${VGM_DIR}

svn co https://svn.code.sf.net/p/vgm/code/tags/v${VGM_VERSION}/vgm \
1>$VGM_INSTALL_LOG 2>$VGM_INSTALL_ERR

if [ -f vgm/packages/XmlVGM/CMakeLists.txt ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

cd vgm
mkdir build
cd build

cmake -DGEANT4_DIR:PATH=$GEANT4_INSTALL_DIR -DCMAKE_INSTALL_PREFIX=${VGM_INSTALL_DIR} ../ \
1>$VGM_INSTALL_LOG 2>$VGM_INSTALL_ERR

if [ -f Makefile ]; then
  echo "configure done"
else
  echo -e "configure unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

echo "Building...."

make -j${NCPU} 1>$VGM_INSTALL_LOG 2>$VGM_INSTALL_ERR

if [ -f packages/BaseVGM/libBaseVGM.so ]; then
  echo "build done"
else
  echo -e "build unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

make install 1>$VGM_INSTALL_LOG 2>$VGM_INSTALL_ERR
cd ${THIRD_PARTY_DIR}
echo "vgm installed in " ${VGM_INSTALL_DIR}


SUCCESS=1

