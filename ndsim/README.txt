//////////////////////////////////////////////////////////////////////////////////
//										//
//										//
//	 	  _____  ______          _____  __  __ ______ 			//
// 		 |  __ \|  ____|   /\   |  __ \|  \/  |  ____|                  //
//		 | |__) | |__     /  \  | |  | | \  / | |__                     //
//		 |  _  /|  __|   / /\ \ | |  | | |\/| |  __|                    //
//		 | | \ \| |____ / ____ \| |__| | |  | | |____ 			//
//		 |_|  \_\______/_/    \_\_____/|_|  |_|______|			//
//										//
//										//
//										//
//										//
//////////////////////////////////////////////////////////////////////////////////

CONTENTS:
------------------
1. Authors
2. About
3. Software Structure
4. Third party software
5. Install
6. How to run default Program
7. Reading outputs

1. Authors:
-----------------------------------------------------------
Georgios Christodoulou <georgios at hep.ph.liv.ac.uk> - current developer
University of Liverpool

Tom Stainer <tstainer \at liv.ac.uk>
University of Liverpool

Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
University of Geneva

2. About:
-----------------------------------------------------------
This software is designed for internal use for the DUNE-GasTPC 
proposed near detector and is tailored for this use primarily.
However the framework allows for users to add classes at their will
whether it be for adding new algorithms, processors, data types, etc.
This can then be adapted fairly easily to fit with other requirements
form other experiments. 

3. Software Structure:
-----------------------------------------------------------

BASIC COMPOSITION
===========
Algorithm   
===========
The procedure of acting on some input i.e. The code that does something

===========
Processor   
===========
Code that takes as input an entry from a TTree and outputs the processed
information into another TTree.

The relationship is one processor to many algorithms.

+ A program must loop over each processor in the chain, this is handled by the GasTPCProcessManager
+ Each processor records in its own TTree, entries will not necessarily match in each tree
+ The ability to match events from trees is done via the backtracer
+ Parameters for each run are input at run time via DataCards
+ New parameters can be easily included into the code
+ Default parameters must be set in the code to avoid crashes
+ Input cards will read parameters and override defaults
+ All parameters are recorded in the output file in the form of a DataCard

4. Third Party Software
----------------------------------------------------------
In order to run the software you also need some external packages. There are scripts for these but may be outdated, they lie in the third_party dir. 
If you have problems with the scripts the software needed is:

  + CLHEP (included in the latest versions of geant)
  + pythia6
  + lhapdf
  + libxml2
  + log4pp
  + root (tested with root_v5.34.26)
  + genie (tested with R-2_8_4)
  + xerces
  + geant4 (tested with geant4.10.01)
  + geant4_vmc (tested with geant4_vmc.3.1.p1)
  + VGM (tested with vgm.4.2)
  + dk2nu (to run with gevgen_numi for lbnf flux)
  to compile
  mkdir dk2nu-build
  cd dk2nu-build
cmake -DCMAKE_INSTALL_PREFIX=/hepstore/georgios/gastpc/genie/dk2nu/trunk/dk2nu/dk2nu-install /hepstore/georgios/gastpc/genie/dk2nu/trunk/dk2nu
make
make install


The majority of the packages are required for GENIE, so if you have genie installed it should be a lot easier. 

All the above software is needed to use the GasTPC-ND software. It will give you an error otherwise.

If you already have the software installed then simply add the paths of this to setup.sh.
Then make sure you have set the $GasTPC variable to point to this directory and then source setup.sh.

Otherwise you can run the install_all.sh script to install all the software into the third_party directory, however this is very time consuming.

The versions of geant4_vmc and VGM are root and geant4 depended. If you want to use an older version of root you have to find the corresponding 
version of geant4_vmc and VGM and they must also be compatible with the geant4.10.* versions.

5. Install
------------
Once all third party software is installed you must make sure that each is on your $PATH and libraries on $LD_LIBRARY_PATH. 
There is a script to help with this, setup.sh.
Once you have the branch and everything is setup do the following to install GasTPC-nd software:

cd build
cmake ../src
make

If all went well then it should now run. You can add $GasTPC/build/bin to your $PATH and then you can perform GasTPCprocess.

6. How to run the default program
-----------------------------------------------------------
First you need to build the geometry by running
GasTPCGeombuild

The application is controlled by an xml file in src/config/GeometryBuild.xml
This will create a .root file and a .gdml file
This will generate the detector geometry, but it won't add the detector in the cavity
Alternative you can use the geometry files in src/config/GasTPCDetector.gdml
The complete geometry with the detector in the cavity is here src/config/site.gdml

Step 1: Run genie with the geometry file: This is not depending on this software

Step2: Propagate the genie output to geant. This will same all the g4 hits (energy depositions)
GasTPCTracking -i inputgeniefile.root -o geantfile.root 

Step3: Do the mock reconstruction and write the output in flat tree
GasTPCAnalysis -i geantfile.root -o analfile.root
