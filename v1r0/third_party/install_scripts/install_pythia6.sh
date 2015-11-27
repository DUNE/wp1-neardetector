SUCCESS=0

echo "going to install pythia6"${PYTHIA6_VERSION} "  in " ${PYTHIA6_DIR}

###################################################################################

if [ -d ${PYTHIA6_DIR} ];then
  echo "directory ${PYTHIA6_DIR} already exists. going to remove it."
  rm -rf ${PYTHIA6_DIR}
fi

mkdir -p ${PYTHIA6_DIR}
cd ${PYTHIA6_DIR}

if [ -f build_pythia6.sh ]; then
  echo "file build_pythia6.sh  already exists. going to remove it."
  rm build_pythia6.sh
fi

#wget http://projects.hepforge.org/genie/trac/export/3170/trunk/src/scripts/build/ext/build_pythia6.sh
wget --no-check-certificate genie.hepforge.org/trac/export/3631/trunk/src/scripts/build/ext/build_pythia6.sh \
1>${PYTHIA6_INSTALL_LOG} 2>${PYTHIA6_INSTALL_ERR}

if [ ! -f build_pythia6.sh ];then
  echo -e "download of the installation script is unsuccessful! \n Check err file in tmp dir for error \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

source build_pythia6.sh 1>>${PYTHIA6_INSTALL_LOG} 2>>${PYTHIA6_INSTALL_ERR}
cd ${PYTHIA6_DIR}
mv build_pythia6.sh ${THIRD_PARTY_DOWNLOADS}
mv ${PYTHIA6_DIR}/v6_424 ${PYTHIA6_INSTALL_DIR}

if [ -f ${PYTHIA6_INSTALL_DIR}/lib/libPythia6.so ]; then
  echo "pythia6 installed in "${PYTHIA6_INSTALL_DIR}
else 
  echo -e "install unsuccessful! \n Check err file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

SUCCESS=1

