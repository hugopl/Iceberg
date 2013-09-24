if (QT5)
  cmake_minimum_required(VERSION 2.8)
else(QT5)
  cmake_minimum_required(VERSION 2.8.8)
endif(QT5)

project(iceberg)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

if (QT5)
  find_package(Qt 5.0 REQUIRED)
else(QT5)
  find_package(Qt 4.8 REQUIRED)
endif(QT5)

# uninstall target
configure_file("cmake_uninstall.cmake" "cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

add_subdirectory(src)
