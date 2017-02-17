### CMakeLists.cxx -- gastpc/EvtRec

set(EvtRec_HDRS   EventRecord.h
                  MCGenInfo.h
                  MCParticle.h
                  MCTrack.h
                  MCHit.h
                  Vector3D.h
                  Vector4D.h )

set(EvtRec_SRCS   EventRecord.cxx
                  MCGenInfo.cxx
                  MCParticle.cxx
                  MCTrack.cxx
                  MCHit.cxx
                  Vector3D.cxx
                  Vector4D.cxx
                  RootFileReader.cxx
                  RootFileWriter.cxx )

root_generate_dictionary(G__GasTPCEvtRec ${EvtRec_HDRS} LINKDEF LinkDef.h)

add_library(GasTPCEvtRec SHARED ${EvtRec_SRCS} G__GasTPCEvtRec.cxx)

target_link_libraries(GasTPCEvtRec ${ROOT_LIBRARIES}
                                   ${GENIE_LIBRARIES}
                                   ${LIBXML2_LIBRARIES}
                                   ${Log4cpp_LIBRARIES})

install(TARGETS GasTPCEvtRec
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

file(GLOB GasTPCEvtRecDicts ${CMAKE_CURRENT_BINARY_DIR}/*.pcm)
install(FILES ${GasTPCEvtRecDicts} DESTINATION lib)
