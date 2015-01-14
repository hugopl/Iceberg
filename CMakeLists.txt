cmake_minimum_required(VERSION 2.8.8)

project(iceberg)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

if (NOT QT4)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Widgets REQUIRED)
else(NOT QT4)
  find_package(Qt 4.8 REQUIRED)
endif(NOT QT4)

# uninstall target
configure_file("cmake_uninstall.cmake" "cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

add_subdirectory(src)
