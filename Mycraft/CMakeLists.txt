# See "With CMake and installed GLFW binaries" in the GLFW readme.

cmake_minimum_required(VERSION 3.13)

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
add_executable(Mycraft)
target_sources(Mycraft PRIVATE src/main.cpp 
                        src/glad.c
                        src/Application.cpp
                        src/Context.cpp
                        src/Helpers.cpp
                        src/stb.cpp)

target_include_directories(Mycraft PRIVATE "include" "src")

add_subdirectory(src/core)
add_subdirectory(src/render)
add_subdirectory(src/shader)
add_subdirectory(src/texture)
add_subdirectory(src/world)

target_link_libraries(Mycraft glfw)
target_link_libraries(Mycraft OpenGL::GL)

install(TARGETS Mycraft DESTINATION bin)