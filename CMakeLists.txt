cmake_minimum_required(VERSION 2.8)

enable_testing()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(pecoffresourcelib)

add_definitions(-Wall -Wsign-compare)

#add_executable(pecoffresourcelib main.c pcrio.cpp)
#add_executable(debug_info debug_info.c pcrio.cpp)

add_subdirectory(tests)

