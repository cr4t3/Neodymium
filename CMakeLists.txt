cmake_minimum_required(VERSION 3.31.6)

project(neodymium LANGUAGES CXX)

file(GLOB_RECURSE CXXMODULES ${PROJECT_SOURCE_DIR}/src/modules/*.cpp) 

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp ${CXXMODULES})
