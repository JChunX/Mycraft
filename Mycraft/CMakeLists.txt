# See "With CMake and installed GLFW binaries" in the GLFW readme.

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Mycraft VERSION 1.0)
add_subdirectory(shaders)
add_subdirectory(textures)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Suppress warnings of the deprecation of glut functions on macOS. 
if(APPLE)
    add_definitions(-Wno-deprecated-declarations) 
endif()

# Find the packages we need.  
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

# add executables
add_executable(Mycraft src/main.cpp 
                        src/glad.c
                        src/VBO.cpp 
                        src/VAO.cpp 
                        src/EBO.cpp
                        src/Shader.cpp
                        src/stb.cpp
                        src/Texture.cpp
                        src/Camera.cpp
                        src/WorldObject.cpp
                        src/InputBroadcaster.cpp
                        src/InputListener.cpp)

target_include_directories(Mycraft PRIVATE "include" "src")

target_link_libraries(Mycraft glfw)
target_link_libraries(Mycraft OpenGL::GL)

install(TARGETS Mycraft DESTINATION bin)