SUCCESS=0

echo "going to place log4cpp."${LOG4CPP_VERSION} "  in " ${LOG4CPP_DIR}

###################################################################################

if [ -d ${LOG4CPP_DIR}/log4cpp-v${LOG4CPP_VERSION} ];then
  echo "directory log4cpp-v${LOG4CPP_VERSION} already exists. going to remove it."
  rm -rf ${LOG4CPP_DIR}/log4cpp-v${LOG4CPP_VERSION}
fi

mkdir -p ${LOG4CPP_DIR}/log4cpp-v${LOG4CPP_VERSION}
cd ${LOG4CPP_DIR}/log4cpp-v${LOG4CPP_VERSION}
wget http://sourceforge.net/projects/log4cpp/files/log4cpp-${LOG4CPP_VERSION}.x%20%28new%29/log4cpp-${LOG4CPP_VERSION}/log4cpp-${LOG4CPP_VERSION}.tar.gz \ 
1>${LOG4CPP_INSTALL_LOG} 2>${LOG4CPP_INSTALL_ERR}

if [ -f log4cpp-${LOG4CPP_VERSION}.tar.gz ];then
  echo "download  done"
else
  echo -e "download unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi
###################################################################################

tar -xvf log4cpp-${LOG4CPP_VERSION}.tar.gz 1>>${LOG4CPP_INSTALL_LOG} 2>>${LOG4CPP_INSTALL_ERR}
mv log4cpp-${LOG4CPP_VERSION}.tar.gz ${THIRD_PARTY_DOWNLOADS}
echo "extract done"

###################################################################################

mkdir -p ${LOG4CPP_INSTALL_DIR}
cd ${LOG4CPP_DIR}/log4cpp-v${LOG4CPP_VERSION}/log4cpp
./configure --prefix=${LOG4CPP_INSTALL_DIR} \
1>>${LOG4CPP_INSTALL_LOG} 2>>${LOG4CPP_INSTALL_ERR}

if [ -f Makefile ]; then
  echo "configure done"
else
  echo -e "configure unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################
echo "Building....."

make -j${NCPU}  1>>${LOG4CPP_INSTALL_LOG} 2>>${LOG4CPP_INSTALL_ERR}

make check  1>>${LOG4CPP_INSTALL_LOG} 2>>${LOG4CPP_INSTALL_LOG}
make install  1>>${LOG4CPP_INSTALL_LOG} 2>>${LOG4CPP_INSTALL_LOG}

if [ -f ${LOG4CPP_INSTALL_DIR}/lib/liblog4cpp.so ]; then
  echo "log4cpp installed in "${LOG4CPP_INSTALL_DIR}
else 
  echo -e "install unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

cd ${THIRD_PARTY_DIR}

export SUCCESS=1

