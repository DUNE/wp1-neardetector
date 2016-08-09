### CMakeLists.cxx -- gastpc/EvtRec

set(EvtRec_HDRS   EventRecord.h
                  NuInteraction.h
                  MCParticle.h
                  MCTrack.h
                  MCHit.h )

set(EvtRec_SRCS   EventRecord.cxx
                  NuInteraction.cxx
                  MCParticle.cxx
                  MCTrack.cxx
                  MCHit.cxx 
                  RootFileReader.cxx 
                  RootFileWriter.cxx )

root_generate_dictionary(G__EvtRec ${EvtRec_HDRS} LINKDEF LinkDef.h)

add_library(GasTPCEvtRec SHARED ${EvtRec_SRCS} G__EvtRec.cxx)

target_link_libraries(GasTPCEvtRec ${ROOT_LIBRARIES}
                                   ${GENIE_LIBRARIES}
                                   ${LIBXML2_LIBRARIES}
                                   ${Log4cpp_LIBRARIES})

install(TARGETS GasTPCEvtRec
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/G__EvtRec_rdict.pcm"
#              "${CMAKE_CURRENT_BINARY_DIR}/libEvtRec.rootmap" 
        DESTINATION lib)
