### CMakeLists.cxx -- gastpc/config

set(CONFIG_FILES g4vis.mac 
                 G4SimDuneGArND.mac)

install(FILES ${CONFIG_FILES} DESTINATION config)