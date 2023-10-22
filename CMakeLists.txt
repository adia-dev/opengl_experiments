cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME FirstOpenGLProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(${PROJECT_NAME} DESCRIPTION "First OpenGL Project !" LANGUAGES CXX)
include(cmake/StaticAnalyzers.cmake)

# GLFW
find_package(glfw3 3.3 REQUIRED)

add_compile_definitions(GLFW_INCLUDE_NONE)

set(SOURCES src/main.cpp)
set(GLAD_DIR external/)

add_executable(${PROJECT_NAME} ${SOURCES})
add_subdirectory(external/glad/)

target_include_directories(${PROJECT_NAME} PUBLIC src glfw deps include)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad)

message(STATUS "Source Files: ${SOURCES}")
