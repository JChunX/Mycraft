cmake_minimum_required(VERSION 3.13)

project(Mycraft VERSION 1.0)
add_subdirectory(shaders)
add_subdirectory(resources)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(APPLE)
    add_definitions(-Wno-deprecated-declarations) 
endif()

set(Boost_USE_DEBUG_LIBS         OFF)
set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

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
target_link_libraries(Mycraft ${Boost_LIBRARIES})

install(TARGETS Mycraft DESTINATION bin)