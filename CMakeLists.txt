cmake_minimum_required(VERSION 3.24)
project(untitled C)

set(CMAKE_C_STANDARD 11)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_executable(untitled main_project.c)

target_link_libraries(untitled ${CURL_LIBRARIES})
