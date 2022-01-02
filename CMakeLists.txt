cmake_minimum_required(VERSION 3.4.3)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)

project(app)

file(CREATE_LINK ${PROJECT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res SYMBOLIC)

add_executable(myapp main.cpp util.cpp graphics/graphics.cpp graphics/simple.cpp graphics/texture.cpp)

target_link_libraries(myapp glfw GLEW GL png)

install(TARGETS myapp DESTINATION bin)

