cmake_minimum_required(VERSION 3.31.6)

project(neodymium LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

find_package(OpenGL REQUIRED)
find_package(glfw3 3.4 REQUIRED)

file(GLOB_RECURSE CXXMODULES ${PROJECT_SOURCE_DIR}/src/modules/*.cpp) 

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp ${CXXMODULES})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)