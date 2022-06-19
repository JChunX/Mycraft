target_sources(Mycraft PRIVATE BlockTexture.cpp
                               SkyboxTexture.cpp
                               TextureManager.cpp)

target_include_directories (Mycraft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})