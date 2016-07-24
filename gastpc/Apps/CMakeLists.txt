### CMakeLists.txt -- gastpc/Apps

add_executable(VertexStats VertexStats.cxx)

target_link_libraries(VertexStats ${GENIE_LIBRARIES}
                                  ${ROOT_LIBRARIES})

