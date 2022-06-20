target_sources(Mycraft PRIVATE MainRenderer.cpp
                            ChunkRenderer.cpp
                            Mesh.cpp
                            Renderer.cpp
                            Skybox.cpp
                            SkyboxRenderer.cpp
                            Fluid.cpp)

target_include_directories (Mycraft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})