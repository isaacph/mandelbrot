cmake_minimum_required(VERSION 3.4.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)

project(app)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(Freetype REQUIRED)

# file(CREATE_LINK ${PROJECT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res SYMBOLIC)

add_executable(myapp main.cpp util.cpp graphics/graphics.cpp graphics/simple.cpp graphics/texture.cpp src/glad/glad.c)

target_include_directories(myapp PUBLIC include)
target_link_libraries(myapp glfw3 ${OpenGL_gl_LIBRARY} png freetype gcc stdc++ )

install(TARGETS myapp DESTINATION bin)
target_link_options(myapp PRIVATE "-static")
