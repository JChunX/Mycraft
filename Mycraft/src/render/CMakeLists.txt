target_sources(Mycraft PRIVATE BlockRenderer.cpp
                            MainRenderer.cpp
                            ChunkRenderer.cpp
                            Mesh.cpp)

target_include_directories (Mycraft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})