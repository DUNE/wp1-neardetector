echo -e "\n preparing for building"

if [ ! $LBNO ]; then
  source ../setup.sh
fi

if [ ! -f "../build/Makefile" ]; then
  cd ../build
  cmake ../src/
  cd -
else
  echo "Nothing to do."
fi

