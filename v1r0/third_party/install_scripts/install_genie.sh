SUCCESS=0

echo "going to place genie"${GENIE_VERSION} "  in " ${GENIE_DIR}

###################################################################################

if [ -d ${GENIE_DIR} ];
then
  echo "directory ${GENIE_DIR} already exists. going to remove it."
  rm -rf ${GENIE_DIR}
fi

mkdir -p ${GENIE_DIR}
cd ${GENIE_DIR}

svn co http://genie.hepforge.org/svn/branches/R-${GENIE_VERSION} 1>${GENIE_INSTALL_LOG} 2>${GENIE_INSTALL_ERR}

mv R-${GENIE_VERSION} genie

if [ -f ${GENIE_DIR}/genie/src/Algorithm/Algorithm.cxx ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n  Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

#compress to tar.gz file and store in downloads
tar cvfz genie.${GENIE_VERSION}.source.tar.gz genie 1>>${GENIE_INSTALL_LOG} 2>>${GENIE_INSTALL_ERR}
mv genie.${GENIE_VERSION}.source.tar.gz ${THIRD_PARTY_DOWNLOADS}

cd ${GENIE_DIR}/genie

#patch version 2.6.6
if [ "${GENIE_VERSION}" == "2_6_6" ];then
  cp ${THIRD_PARTY_DIR}/install_scripts/genie${GENIE_VERSION}.patch .
  cp ${THIRD_PARTY_DIR}/install_scripts/genie${GENIE_VERSION}.mute.patch .
  patch -p0 <genie${GENIE_VERSION}.patch
  patch -p0 <genie${GENIE_VERSION}.mute.patch
fi

source ${THIRD_PARTY_DIR}/install_scripts/setupthisGenie.sh
cp ${THIRD_PARTY_DIR}/install_scripts/setupthisGenie.sh .
source ${ROOT_INSTALL_DIR}/bin/thisroot.sh
./configure --prefix=${GENIE_INSTALL_DIR} \
            --enable-flux-drivers \
            --enable-geom-drivers \
            --enable-rwght \
            --with-pythia6-lib=${PYTHIA6_INSTALL_DIR}/lib/ \
            --with-lhapdf-lib=${LHAPDF_INSTALL_DIR}/lib/ \
            --with-lhapdf-inc=${LHAPDF_INSTALL_DIR}/include/ \
	    --with-log4cpp-lib=${LOG4CPP_INSTALL_DIR}/lib/ \
	    --with-log4cpp-inc=${LOG4CPP_INSTALL_DIR}/include/ \
	    --with-libxml2-lib=${LIBXML2_INSTALL_DIR}/lib/ \
	    --with-libxml2-inc=${LIBXML2_INSTALL_DIR}/include/libxml2 \
1>>${GENIE_INSTALL_LOG} 2>>${GENIE_INSTALL_ERR}

if [ -f Makefile ]; then
  echo "configure done"
else
  echo -e "configure unsuccessful! \n Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

echo "Building....."
make 1>>${GENIE_INSTALL_LOG} 2>>${GENIE_INSTALL_ERR}

if [ -f bin/gevgen ]; then
  echo "build done"
else
  echo -e "build unsuccessful! \n Check error file ! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################


make install 1>>${GENIE_INSTALL_LOG} 2>>${GENIE_INSTALL_ERR}

if [ -d ${GENIE_INSTALL_DIR}/lib ]; then
  echo "genie installed in "${GENIE_INSTALL_DIR}
else 
  echo -e "install unsuccessful! \n Check error file exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

cd ${THIRD_PARTY_DIR}

SUCCESS=1

