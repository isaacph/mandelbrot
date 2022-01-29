#!/bin/bash
# but for windows/msys2 specifically, not for linux
# clone my repo :D
# git clone https://github.com/isaacph/basicopengl.git

# first: install msys2
# then run this script in the MSYS2 terminal
# /mingw64.exe # this line is where I try to make sure we have the right compiler selected

# install the compiler
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake git mingw-w64-x86_64-glfw mingw-w64-x86_64-freetype mingw-w64-x86_64-libpng mingw-w64-x86_64-glm
# pacman -S cmake

# compile the setup project
cmake -G"MinGW Makefiles" . # must be run every time CMakeLists.txt is updated
# generates these files: CMakeCache.txt CMakeFiles/* Makefile cmake_install.cmake compile_commands.json - they can deleted and regenerated with the same command
# note the reason I don't use a separate build folder is because I hate having to mess with resources (aka the res folder) being in two places
cmake --build . # build the project, can also use "make myapp" or "mingw32-make myapp" (with msys2)
# should make myapp.exe
./myapp.exe

exit 0
# compile glfw
cd glfw
cmake -G"MinGW Makefiles" .
cmake --build .
# the important build file is now at src/libglfw3.a
cd ..

# compile libpng
cd libpng
cmake -G"MinGW Makefiles" .
cmake --build .
# build file is now just libpng.a
cd ..

cd freetype
# this one wants you to use a separate build folder
cmake -Bbuild -G"MinGW Makefiles" .
cmake --build build
# build file is build/libfreetype.a
cd ..
