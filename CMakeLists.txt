cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME "glossy")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR src)

add_executable(${PROJECT_NAME} ${SRC_DIR}/main.cpp)

file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${SRC_DIR}/*.h ${SRC_DIR}/*.hpp)

target_sources(${PROJECT_NAME} PRIVATE
    ${SRC_FILES}
    ${HEADER_FILES}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
)
