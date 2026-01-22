cmake_minimum_required(VERSION 3.31.6)

project(neodymium LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

if(NOT DEFINED ARCH)
    set(ARCH neo8)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/archs/${ARCH}/${ARCH}.cmake)
    include(${PROJECT_SOURCE_DIR}/archs/${ARCH}/${ARCH}.cmake)
else()
    message(FATAL_ERROR "${ARCH}.CMAKE file not found")
endif()

file(GLOB_RECURSE CXXMODULES ${PROJECT_SOURCE_DIR}/src/modules/*.cpp) 
file(GLOB_RECURSE MAIN_CXX   ${PROJECT_SOURCE_DIR}/src/main_${ARCH}.cpp)
file(GLOB_RECURSE GLOBALS    ${PROJECT_SOURCE_DIR}/globals/*.cpp)

configure_file(${PROJECT_SOURCE_DIR}/src/main.cpp.in ${PROJECT_SOURCE_DIR}/src/main_${ARCH}.cpp @ONLY)

file(GLOB_RECURSE ARCH_LOC       ${PROJECT_SOURCE_DIR}/archs/${ARCH}/*.cpp)

add_executable(${PROJECT_NAME}_${ARCH} ${MAIN_CXX} ${CXXMODULES} ${ARCH_LOC} ${GLOBALS})

if(DEFINED REQ_LIBRARIES)
    target_link_libraries(${PROJECT_NAME}_${ARCH} PRIVATE ${REQ_LIBRARIES})
endif()