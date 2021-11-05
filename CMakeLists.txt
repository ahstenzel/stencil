# CMakeList.txt : CMake project for stencil, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("stencil")

# Add source to this project's executable.
add_executable (stencil "src/stencil.cpp"  "include/common.hpp" "include/image.hpp" "src/image.cpp")

# TODO: Add tests and install targets if needed.
