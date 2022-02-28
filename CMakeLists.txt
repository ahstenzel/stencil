# CMakeList.txt : CMake project for stencil, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.8)
project ("stencil")

## Specify library output directory
set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin/static/${CMAKE_SYSTEM_PROCESSOR}")
if (BUILD_SHARED_LIBS)
  set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin/shared/${CMAKE_SYSTEM_PROCESSOR}")
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

# Add source to this project's executable
add_executable(stencil src/image.cpp src/stencil.cpp include/common.hpp include/image.hpp)
target_include_directories(stencil PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")