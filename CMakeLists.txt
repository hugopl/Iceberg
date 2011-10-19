cmake_minimum_required(VERSION 2.8)

project(icemon)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

find_package(Qt 4.7 REQUIRED)

add_subdirectory(src)
