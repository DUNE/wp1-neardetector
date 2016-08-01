### CMakeLists.cxx -- gastpc/EvtRec

file(GLOB EvtRec_SRCS "*.cxx")
file(GLOB EvtRec_HDRS "*.h")
list(REMOVE_ITEM EvtRec_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h)

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
              "${CMAKE_CURRENT_BINARY_DIR}/libEvtRec.rootmap" 
        DESTINATION lib)