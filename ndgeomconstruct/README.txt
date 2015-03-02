
*****************************************************

               LBNO Near Detector Geometry

*****************************************************

1. Third Party Software Needed:
   
   a.CLHEP
   b.PYTHIA6
   c.ROOT

If all this software is already installed, then simply add binaries to $PATH and libraries to $LD_LIBRARY_PATH and skip installation steps.
NOTE: make sure both pyhtia6 and gdml are enabled in your root build

If not installed follow instructions in software.txt located in the third_party directory

2. Run setup.sh from inside the main directory, otherwise if you require it to run at startup (.bashrc) then change the variables in setup.sh
   $GEOM and $THIRD_PARTY_DIR to user defined directories as they are currently set to $PWD

3. To compile:
   a. cd build directory
   b. type command "cmake ../src"
   c. type command "make"
   e. run "geomconstruct" with input cards 

4. Define input parameters through the cards

PARAMETER NAME		VALUE				DESCRIPTION
--------------------------------------------------------------------------
rootFile                geometry_2x2x2.root		output name root
gdmlFile                geometry_2x2x2.gdml		output name gdml

rockX                   40*m				dimensions of rock(world volume)
rockY                   40*m
rockZ                   60*m
rockDensity             2.32e3*kg/m3			denisty of rock

cavityGap               2.0*m				cavity gap
cavityDensity           1.29e-3*g/cm3			denisty of cavity (air)

magnetThickness         1.0*m				magnet thickness
magnetDensity           7.87*g/cm3			magnet density

tpcVesselRadiusRatio    0.35				Curvature of the tpc vessel
tpcVesselThickness      0.05*m				thickness of the tpc vessel
tpcVesselRadius         2.5*m				radius of the tpc Vessel
tpcVesselLength         5.0*m				length of the vessel
tpcVesselPlacementZ     0.0*m				not yet implemented

tpcMaterial             ArgonGas			material in the tpc - see Materials.cc for definitions
tpcFidX                 2.0 m				dimensions of tpc
tpcFidY                 2.0 m
tpcFidZ                 2.0 m
tpcPlacementZ           0.0 m				not yet implemented
tpcDensity              0.035*g/cm3			density of the gas

scintLayers             300				number of layers of scintillator, limited to vessel dims
scintThickness          20*mm				scint thickness of each layer
scintGap                0*mm				gap between layers
scintDensity            1.18*g/cm3			density of scintillator

mindGap                 2.0*m				gap between the mind and the vessel
mindX                   10.0*m				dimensions of the mind
mindY                   10.0*m
mindZ                   5.0*m
mindThicknessLayer1     30.0*mm				layer 1 thickness
mindThicknessLayer2     15.0*mm				layer 2 thickness
mindMaterial            Iron				mind material 1 - see Materials.cc for definitions
mindDensity             7.87*g/cm3			density of the material 1
mindScintMaterial       Scintillator			mind material 2 - see Materials.cc for definitions
mindScintDensity        1.18*g/cm3			density of the material 2


5. To draw the geometry simply run the draw.cxx script in root. i.e root -l draw.cxx
NOTE: If the output filename is changed from geometry.root then edit the draw.cxx to read in the new filename



