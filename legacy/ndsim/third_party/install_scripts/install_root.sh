SUCCESS=0

echo "going to place root"${ROOT_VERSION} "  in " ${ROOT_DIR}

###################################################################################

if [ -d ${ROOT_DIR} ];then
  echo "directory ${ROOT_DIR} already exists. going to remove it."
  rm -rf ${ROOT_DIR}
fi

mkdir -p ${ROOT_DIR}
cd ${ROOT_DIR}
wget ftp://root.cern.ch/root/root_v${ROOT_VERSION}.source.tar.gz 1>${ROOT_INSTALL_LOG} 2>${ROOT_INSTALL_ERR}

if [ -f root_v${ROOT_VERSION}.source.tar.gz ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n  Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

tar xvfz root_v${ROOT_VERSION}.source.tar.gz 1>>${ROOT_INSTALL_LOG} 2>>${ROOT_INSTALL_ERR}
mv root_v${ROOT_VERSION}.source.tar.gz ${THIRD_PARTY_DOWNLOADS}
echo "extract done"

###################################################################################

cd root
./configure --enable-roofit \
            --enable-minuit2 \
            --enable-gdml \
            --enable-pythia6 \
            --with-pythia6-libdir=${PYTHIA6_INSTALL_DIR}/lib/ \
1>>${ROOT_INSTALL_LOG} 2>>${ROOT_INSTALL_ERR}
echo "configure done"

if [ -f Makefile ];then
    echo "Building........"
    make -j${NCPU} 1>>${ROOT_INSTALL_LOG} 2>>${ROOT_INSTALL_ERR}
else 
    echo -e "configure not done! \n Check error file! \n exit"
    cd ${THIRD_PARTY_DIR}
    return
fi

if [ -f lib/libEGPythia6.so ] && [ -f lib/libEve.so ] && [ -f lib/libGdml.so ]; then
  echo "build done"
elif [ -f lib/libEGPythia6.a ] && [ -f lib/libEve.a ] && [ -f lib/libGdml.a ]; then
  echo "build done"
else
  echo -e "build unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

\\make install 1>>${ROOT_INSTALL_LOG} 2>>${ROOT_INSTALL_LOG}
\\cd ${THIRD_PARTY_DIR}
echo "root installed in " ${ROOT_INSTALL_DIR}

SUCCESS=1

