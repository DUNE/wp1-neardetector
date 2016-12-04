### CMakeLists.txt -- gastpc/ndtfdst

root_generate_dictionary(G__NdtfDst DstEntry.h LINKDEF LinkDef.h)

add_library(NdtfDst SHARED DstWriter.cxx G__NdtfDst.cxx)

target_link_libraries(NdtfDst   ${ROOT_LIBRARIES}
                                ${GENIE_LIBRARIES}
                                ${LIBXML2_LIBRARIES}
                                ${Log4cpp_LIBRARIES})

install(TARGETS NdtfDst
        LIBRARY DESTINATION lib)

