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
Tom Stainer <tstainer \at liv.ac.uk> - current developer
University of Liverpool

Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
University of Geneva

2. About:
-----------------------------------------------------------
This software is designed for internal use for the LAGUNA-LBNO 
proposed experiment and is tailored for this use primarily.
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

+ A program must loop over each processor in the chain, this is handled by the LbnoProcessManager
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

  + CLHEP 
  + pythia6
  + lhapdf
  + libxml2
  + log4pp
  + root
  + genie
  + xerces
  + geant4

The majority of the packages are required for GENIE, so if you have genie installed it should be a lot easier. 
However this software has only been tested with version <=2.6.6 and versions higher i.e 2.8.0 is new release, may not work, 
as there were some changes to the way the splines are read in.

All the above software is needed to use the LBNO-ND software. It will give you an error otherwise.

If you already have the software installed then simply add the paths of this to env.sh.
Then make sure you have set the $LBNO variable to point to this directory and then source setup.sh.

Otherwise you can run the install_all.sh script to install all the software into the third_party directory, however this is very time consuming.

5. Install
------------
Once all third party software is installed you must make sure that each is on your $PATH and libraries on $LD_LIBRARY_PATH. 
There is a script to help with this, setup.sh.
Once you have the branch and everything is setup do the following to install lbno-nd software:

cd build
cmake ../src
make

If all went well then it should now run. You can add $LBNO/build/bin to your $PATH and then you can perform lbnoprocess.

6. How to run the default program
-----------------------------------------------------------
The program used in the LAGUNA-LBNO experiment for the Near Detector
is called:

*** lbnoprocess ***

This is ran by:
lbnoprocess input-card.in

or by:
lbnoprocess card.1 card.2 card.3 etc

This runs 3 processors sequentially:
LoadFluxProcessor 	- Reads the beam file,loads and projects the neutrinos to 
		   	  a detector specified in a root or gdml format.

GenieNuEventProcessor 	- Generates the neutrino verticies in the geometry

Geant4TrackingProcessor - Performs secondary tracking using geant4

6.1 The Flux Processor
---------------------------------------------------------------------------------------
The parameters for the LoadFluxProcessor can be summarised below:

INPUT NAME				EXAMPLE VALUE 			EXPLANATION
--------------------------------------------------------------------------------------------------------------------------------------------
loadFluxBasketSize                      64000  				basket size for writing
inputBeamFile                           path/to/beam/file.root		location of input neutrino beam
fluxFileExposure                        212000				corresponding exposure in p.o.t of file
exposure                                1e6				wanted exposure of run
nuMaxEnergy                             30*GeV				maximum neutrino energy cut 
distance                                800 m				distance of detector from origin
radiusCut                               30 m				radius cut at specified distance
flavour                                 14				select only a specific neutrino flavour, pdg code, 0 is all flavours
fluxVerbose                             -1				level of verbose needed, -1 shows only the progress bar, do not use for batch running

6.2 The Input beam format
-------------------------
The way the file is read in the software is from a tree with the following branch names:

BRANCH NAME		DESCRIPTION
--------------------------------------------------
nuPx 			x momentum of the neutrino
nuPy			y momentum of the neutrino
nuPz			z momentum of the neutrino
nuX			x position of pion/kaon decay vertex - neutrino origin
nuY			y position of pion/kaon decay vertex - neutrino origin
nuZ			z position of pion/kaon decay vertex - neutrino origin
nuPID			PID of neutrino, will take pdg format, but we use different coding
			pid ==27 -> numu (14 pdg)
			pid ==28 -> anumu (-14 pdg)
			pid ==5 -> nue (12 pdg)
			pid ==6 -> anue (-12 pdg)

Each file has a corresponding exposure and this needs to be input via the parameter 'fluxFileExposure'
which is in p.o.t. To run to a specific exposure then use the 'exposure' parameter to get the desired 
exposure.

6.3 The Genie Event Processor
------------------------------------
The parameters for input are:

INPUT NAME				EXAMPLE VALUE 				EXPLANATION
--------------------------------------------------------------------------------------------------------------------------------------------
runNumber                               1					Specify run number
nIterations                             0       				use <=0 to use exposure instead, or number of events 
geomFileName                            /path/to/geom.root			location of the geometry file, root or gdml
GenieXSecSplines                        /path/to/crosssectionspline.xml		Specify spline input file for GENIE, only used with version <=2.6.6
targetVolume                            tpcFidVolume				Volume in geometry to generate events in
wantGntp                                0					Do you want the gntp default GENIE file also? boolean: 0 -no, 1-yes
eventVerbose                            -1					level of verbose needed, -1 shows only the progress bar, do not use for batch running

6.4 The Geant4 Tracking Processor
------------------------------------
The parameters for input are:

INPUT NAME				EXAMPLE VALUE 				EXPLANATION
--------------------------------------------------------------------------------------------------------------------------------------------
runVisMacro                             0					Boolean - do you want heprep vis file? - Very large, do not do for large event numbers
runGunMacro                             0					Boolean - do you want to use the geant gun - not been tested
qeccOnly                                0					Boolean - CCQE events only?
simGeomFileName                         /path/to/geom.gdml			location of the geometry in gdml format, MUST BE GDML, root implementation not yet working 										for geant4
physicsList                             FTFP_BERT				Physics List names
hadroniclist                            QGSP_BIC_HP				hadronic physics
emlist                                  EM4					em physics
trackPiZero                             0					Boolean - truth tracking of Pi Zero photons enabled, produces very large files
magFieldStrength                        0.5*tesla				Magnetic field for tpc and MIND
simTargetVolume                         tpcFidVolume				Specific target volume for measuring flux in and out of boundary
simEnergyCut                            1.0*keV					Energy cut
physicsDefaultCut                       0.1*mm					Step cuts
gammaCut                                0.1*mm
electronCut                             0.1*mm
positronCut                             0.1*mm
neutronCut                              0.1*mm
cutEM                                   1.0*keV
trackingVerbose                            -1					level of verbose needed, -1 shows only the progress bar, do not use for batch running

6.5 General Parameters
--------------------------
Also needed is the output file name location parameter, this can be added to either card.

INPUT NAME				EXAMPLE VALUE 				EXPLANATION
--------------------------------------------------------------------------------------------------------------------------------------------
outputFileName                          /path/to/output.root			Output file name and location

6.6 Run
-----------------------
These data cards can all be input in one file i.e. card.in or split over several cards i.e. card.1 card.2 etc...
lbnoprocess card.1 card.2 card.3 will then run the program. This will then output the file specified in outputFileName and should have three branches:
	NuHits			-	Flux
	NuInteractions		-	Neutrino Events
	Tracking		-	Tracking of secondaries
	
Along with the DataCard object and the Runstats object.

6.7 Other programs
----------------------
Other programs exist that only primaries are generated and ignores tracking via geant, this is lbnoprimaries, and does not require inputs from the Geant4 Tracking Processor.
Also there is the lbnotracking which ignores the flux processor and reads in previous neutrino verticies from a previous run and only performs the tracking from geant.

7. Reading outputs/Analysis
--------------------------------
Some programs exist for reading the outputs, however you can make your own. lbnoplot is a basic program to read file(s), from a batch run and loop over them and make some histograms.
