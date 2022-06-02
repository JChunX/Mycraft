target_sources(Mycraft PRIVATE EBO.cpp
                    InputBroadcaster.cpp
                    InputListener.cpp
                    VAO.cpp
                    VBO.cpp
                    EBO.cpp)

target_include_directories (Mycraft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})