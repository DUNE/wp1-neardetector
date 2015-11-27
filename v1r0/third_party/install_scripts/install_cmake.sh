SUCCESS=0

echo "going to place cmake."${CMAKE_VERSION} "  in " ${CMAKE_DIR}

###################################################################################

if [ -d ${CMAKE_DIR} ];then
  echo "directory ${CMAKE_DIR} already exists. going to remove it."
  rm -rf ${CMAKE_DIR}
fi

mkdir -p ${CMAKE_DIR}
cd ${CMAKE_DIR}

wget www.cmake.org/files/v2.8/cmake-${CMAKE_VERSION}.tar.gz \
1>${CMAKE_INSTALL_LOG} 2>${CMAKE_INSTALL_ERR}

if [ -f cmake-${CMAKE_VERSION}.tar.gz ];then
  echo "download done"
else
  echo -e "download unsuccessful! \n  exit"
  cd ${THIRD_PARTY_DIR}
  return
fi

###################################################################################

tar xvfz cmake-${CMAKE_VERSION}.tar.gz
mv cmake-${CMAKE_VERSION}.tar.gz ${THIRD_PARTY_DOWNLOADS}
cd cmake-${CMAKE_VERSION}

echo "configuring..."

./bootstrap --prefix=${CMAKE_INSTALL_DIR}

#cmake .

echo "Building...."
make
echo "build done"
make install
echo "done"

SUCCESS=1

cd ${THIRD_PARTY_DIR}
