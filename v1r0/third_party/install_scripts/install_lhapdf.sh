SUCCESS=0

echo "going to place lhapdf"${LHAPDF_VERSION} "  in " ${THIRD_PARTY_DIR}"/lhapdf-v"${LHAPDF_VERSION}
cd ${THIRD_PARTY_DIR}


###################################################################################

if [ -d lhapdf-v${LHAPDF_VERSION} ];then
  echo "directory lhapdf-v${LHAPDF_VERSION} already exists. going to remove it."
  rm -rf lhapdf-v${LHAPDF_VERSION}
fi

mkdir lhapdf-v${LHAPDF_VERSION}
cd lhapdf-v${LHAPDF_VERSION}
wget www.hepforge.org/archive/lhapdf/lhapdf-${LHAPDF_VERSION}.tar.gz \
 1>${LHAPDF_INSTALL_LOG} 2>${LHAPDF_INSTALL_ERR}

if [ -f lhapdf-${LHAPDF_VERSION}.tar.gz ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n  Check error file! exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

tar xvfz lhapdf-${LHAPDF_VERSION}.tar.gz  1>>${LHAPDF_INSTALL_LOG} 2>>${LHAPDF_INSTALL_ERR}
mv lhapdf-${LHAPDF_VERSION}.tar.gz ${THIRD_PARTY_DOWNLOADS}
mv lhapdf-${LHAPDF_VERSION} lhapdf

echo "extract done"
###################################################################################

cd lhapdf
./configure --prefix=${LHAPDF_INSTALL_DIR} \
            --enable-low-memory \
1>>${LHAPDF_INSTALL_LOG} 2>>${LHAPDF_INSTALL_ERR}

if [ -f Makefile ]; then
  echo "configure done"
else
  echo -e "configure unsuccessful! \n  Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

make -j${NCPU}  1>>${LHAPDF_INSTALL_LOG} 2>>${LHAPDF_INSTALL_ERR}

if [ -f lib/.libs/libLHAPDF.so ]; then
  echo "build done"
else
  echo -e "build unsuccessful! \n  Check error file! \n exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

make install 1>>${LHAPDF_INSTALL_LOG} 2>>${LHAPDF_INSTALL_LOG}
if [ -d ${LHAPDF_INSTALL_DIR}/lib ]; then
  echo "lhapdf installed in " ${LHAPDF_INSTALL_DIR}
else 
  echo -e "install unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

cd ${LHAPDF_INSTALL_DIR}/share/lhapdf/
../../../lhapdf/bin/lhapdf-getdata GRV98lo.LHgrid GRV98nlo.LHgrid 1>>${LHAPDF_INSTALL_LOG} 2>>${LHAPDF_INSTALL_ERR}
if [ -f GRV98lo.LHgrid ] && [ -f GRV98nlo.LHgrid ]; then
  echo "pdfs downloaded in " ${PWD}
else
  echo -e "pdfs download unsuccessful! \n  Check error file! \nexit"
  cd ${THIRD_PARTY_DIR}
  return
fi

cd ${THIRD_PARTY_DIR}

SUCCESS=1

