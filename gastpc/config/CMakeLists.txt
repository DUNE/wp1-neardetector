### CMakeLists.cxx -- gastpc/config

set(CONFIG_FILES g4vis.mac
                 g4sim_dune.mac 
                 G4SimDuneGArND.mac)

install(FILES ${CONFIG_FILES} DESTINATION config)