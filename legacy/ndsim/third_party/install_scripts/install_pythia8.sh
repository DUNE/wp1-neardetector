echo -e "\n ***** Installing Pythia8 *****"
echo "going to place pythia8."${PYTHIA8_VERSION} "  in " ${THIRD_PARTY_DIR}"/pythia-v8."${PYTHIA8_VERSION}
mkdir ${THIRD_PARTY_DIR}/pythia-v8.${PYTHIA8_VERSION}
cd ${THIRD_PARTY_DIR}/pythia-v8.${PYTHIA8_VERSION}
wget home.thep.lu.se/~torbjorn/pythia8/pythia8${PYTHIA8_VERSION}.tgz 2>>${PYTHIA8_INSTALL_LOG}
echo "download done"

tar xvfz pythia8${PYTHIA8_VERSION}.tgz 1>>${PYTHIA8_INSTALL_LOG} 2>>${PYTHIA8_INSTALL_LOG}
mv pythia8${PYTHIA8_VERSION}.tgz ${THIRD_PARTY_DOWNLOADS}
cd pythia8${PYTHIA8_VERSION} 
echo "extract done"

./configure --prefix=${PYTHIA8_INSTALL_DIR} \
            --enable-shared true 1>>${PYTHIA8_INSTALL_LOG} 2>>${PYTHIA8_INSTALL_LOG}
echo "configure done"

gmake -j${NCPU} 1>>${PYTHIA8_INSTALL_LOG} 2>>${PYTHIA8_INSTALL_LOG}
echo "build done"

cd ${THIRD_PARTY_DIR}
#cp install_pythia8.sh ${THIRD_PARTY_DOWNLOADS}
echo "pythia8 installed in "${PYTHIA8_INSTALL_DIR}

