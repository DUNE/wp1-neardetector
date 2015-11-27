SUCCESS=0

echo "going to place xerces-c"${XERCESC_VERSION} "  in " ${XERCESC_DIR}

###################################################################################

if [ -d ${XERCESC_DIR} ];then
  echo "directory ${XERCESC_DIR} already exists. going to remove it."
  rm -rf ${XERCESC_DIR}
fi

mkdir -p ${XERCESC_DIR}
cd ${XERCESC_DIR}
wget www.apache.org/dist/xerces/c/3/sources/xerces-c${XERCESC_VERSION}.tar.gz \
1>${XERCESC_INSTALL_LOG} 2>${XERCESC_INSTALL_ERR}

if [ -f xerces-c${XERCESC_VERSION}.tar.gz ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

tar xvfz xerces-c${XERCESC_VERSION}.tar.gz 1>>${XERCESC_INSTALL_LOG} 2>>${XERCESC_INSTALL_ERR}
mv xerces-c${XERCESC_VERSION}.tar.gz ${THIRD_PARTY_DOWNLOADS}
mv xerces-c${XERCESC_VERSION} xercesc
cd xercesc
echo "extract done"

###################################################################################

./configure --prefix=${XERCESC_INSTALL_DIR} 1>>${XERCESC_INSTALL_LOG} 2>>${XERCESC_INSTALL_ERR}

if [ -f Makefile ]; then
  echo "configure done"
else
  echo -e "configure unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

echo "Building...."

make -j${NCPU} 1>>${XERCESC_INSTALL_LOG} 2>>${XERCESC_INSTALL_ERR}

if [ -f src/.libs/libxerces-c.so ]; then
  echo "build done"
else
  echo -e "build unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

make install 1>>${XERCESC_INSTALL_LOG} 2>>${XERCESC_INSTALL_ERR}

if [ -d ${XERCESC_INSTALL_DIR}/lib ]; then
  echo "xerces-c installed in " ${XERCESC_INSTALL_DIR}
else 
  echo -e "install unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

cd ${THIRD_PARTY_DIR}

SUCCESS=1

