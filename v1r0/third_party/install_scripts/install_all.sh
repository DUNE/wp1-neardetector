export SUCCESS=0

### install all ###
echo -e "\n Going to install third party libs. This may take some time ..."

if [ -z ${THIRD_PARTY_DIR} ];then
	echo "THIRD_PARTY_DIR variable not set, run install_env.sh first"
	return
fi

echo " Third party directory: " ${THIRD_PARTY_DIR}
cd ${THIRD_PARTY_DIR}/install_scripts

source ${THIRD_PARTY_DIR}/install_scripts/setNCPU.sh
source ${THIRD_PARTY_DIR}/install_scripts/install_env.sh

echo -e "\n***** Installing CLHEP *****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_clhep.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing Pythia6 ***"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_pythia6.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing LHAPDF ****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_lhapdf.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing LIBXML2 ****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_libxml2.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing LOG4CPP ****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_log4cpp.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing Root ******"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_root.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing GENIE *****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_genie.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing Xerces ****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_xercesc.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing Geant4 ****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_geant4.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


echo -e "\n***** Installing G4VMC ****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_g4vmc.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi

echo -e "\n***** Installing VGM ****"
cd $THIRD_PARTY_DIR/install_scripts/
time source install_vgm.sh
echo      "****************************"
if [ $SUCCESS -eq 0 ]; then
  return
fi


