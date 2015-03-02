if [ ! -z  "${LBNO}" ]; then

  export PATH=${LBNO}/build/bin:${PATH}

  #set up the enviroment 
  source ${LBNO}/env.sh
  export CLHEP=${CLHEP_INSTALL_DIR}
  export LOG4CPP=${LOG4CPP_INSTALL_DIR}
  export PYTHIA6=${PYTHIA6_INSTALL_DIR}
  export LIBXML2=${LIBXML2_INSTALL_DIR}
  export LHAPATH=${LHAPDF_INSTALL_DIR}
  
  #set up root
  source ${ROOT_INSTALL_DIR}/bin/thisroot.sh

  #set up genie
  source ${GENIE}/setupthisGenie.sh

  #setup geant4
  source ${GEANT4_INSTALL_DIR}/share/Geant4-9.6.1/geant4make/geant4make.sh

  #echo "SUCCESS: LBNO setup"
else
  echo "WARNING: LBNO not set - make sure to set the LBNO path first before running this script!"
fi
