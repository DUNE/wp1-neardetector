export cpus=`cat /proc/cpuinfo | grep processor | wc -l`

if test $cpus -gt 2; then
  export NCPU=`expr $cpus - 1`
else
  export NCPU=1
fi

echo " will use" $NCPU "CPU cores"

