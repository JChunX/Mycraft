file(GLOB VERTEX_SHADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.vert"
)

file(GLOB FRAGMENT_SHADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
)

file(COPY ${VERTEX_SHADERS}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${FRAGMENT_SHADERS}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})