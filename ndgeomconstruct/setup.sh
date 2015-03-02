#!/bin/bash

#                                                    #
# Run this script in the geom_construction directory #
# Otherwise change the $GEOM and $THIRD_PARTY_DIR to #
#                 user defined paths                 #
#                                                    #

####### setup environment #######
export GEOM=${PWD}
export THIRD_PARTY_DIR=${PWD}/third_party

# add binaries to path
export PATH=${GEOM}/build/bin:$PATH

source $THIRD_PARTY_DIR/scripts/install_env.sh

### SOFTWARE ###
#set up root
source ${THIRD_PARTY_DIR}/root-v${ROOT_VERSION}/root/bin/thisroot.sh

export CLHEP=${THIRD_PARTY_DIR}/clhep-v${CLHEP_VERSION}/clhep-install/
export PYTHIA6=${THIRD_PARTY_DIR}/pythia-v6.424
export DYLD_LIBRARY_PATH=${PYTHIA6}/lib:${DYLD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PYTHIA6}/lib:${LD_LIBRARY_PATH}
