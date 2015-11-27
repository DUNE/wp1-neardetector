SUCCESS=0
            
echo "going to place libxml2-v"${LIBXML2_VERSION} "  in " ${LIBXML2_DIR}

###################################################################################

if [ -d ${LIBXML2_DIR} ];
then
  echo "directory ${LIBXML2_DIR} already exists. going to remove it."
  rm -rf ${LIBXML2_DIR}
fi

mkdir -p ${LIBXML2_DIR}
cd ${LIBXML2_DIR}

wget ftp://xmlsoft.org/libxml2/libxml2-${LIBXML2_VERSION}.tar.gz 1>>${LIBXML2_INSTALL_LOG} 2>>${LIBXML2_INSTALL_ERR}

if [ -f libxml2-${LIBXML2_VERSION}.tar.gz ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n  Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

tar xzf libxml2-${LIBXML2_VERSION}.tar.gz 1>>${LIBXML2_INSTALL_LOG} 2>>${LIBXML2_INSTALL_ERR}
cd libxml2-${LIBXML2_VERSION}

./configure --prefix=${LIBXML2_INSTALL_DIR} 1>>${LIBXML2_INSTALL_LOG} 2>>${LIBXML2_INSTALL_ERR}

if [ -f Makefile ]; then
  echo "configure done"
else
  echo -e "configure unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

echo "Building....."
make 1>>${LIBXML2_INSTALL_LOG} 2>>${LIBXML2_INSTALL_ERR}
make install 1>>${LIBXML2_INSTALL_LOG} 2>>${LIBXML2_INSTALL_ERR}

cd ${THIRD_PARTY_DIR}

SUCCESS=1


