### CMakeLists.txt -- gastpc/ndtfdst

add_library(NdtfDst SHARED DstWriter.cxx)

target_link_libraries(NdtfDst   ${ROOT_LIBRARIES}
                                ${GENIE_LIBRARIES}
                                ${LIBXML2_LIBRARIES}
                                ${Log4cpp_LIBRARIES})

install(TARGETS NdtfDst
        LIBRARY DESTINATION lib)

