export LBNO=$PWD
echo "LBNO set to " ${LBNO}
export THIRD_PARTY_DIR=${LBNO}/third_party
echo "Third Party goes in " ${THIRD_PARTY_DIR}

export SUCCESS=$false

cd $THIRD_PARTY_DIR/install_scripts/

source install_all.sh

if [ $SUCCESS -eq 0 ]; then
  return
fi


cd $THIRD_PARTY_DIR/../

if [ ! $LBNO ]; then
  return
fi

source setup.sh
cd $LBNO
echo -e "\n*** preparing for building ***"
if [ ! -f "build/Makefile" ]; then
  cd build
  cmake ../src/
  cd -
else
  echo "Nothing to do."
fi
echo -e   "****************************** \n"

cd build
make
cd $LBNO
