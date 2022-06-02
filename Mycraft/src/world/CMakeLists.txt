target_sources(Mycraft PRIVATE Block.cpp
                            Camera.cpp
                            WorldObject.cpp)

target_include_directories (Mycraft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})