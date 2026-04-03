cmake_minimum_required(VERSION 3.20)
project(YourBlock VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)

add_library(glad STATIC libs/glad/src/glad.c)

target_link_directories(glad PUBLIC libs/glad/include)

set(IMGUI_SOURCES
    libs/Imgui/imgui.cpp
    libs/Imgui/imgui_draw.cpp
    libs/Imgui/imgui_tables.cpp
    libs/Imgui/imgui_widgets.cpp
    libs/Imgui/backends/imgui_impl_glfw.cpp
    libs/Imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_link_directories(imgui PUBLIC 
    libs/Imgui
    libs/Imgui/backends
)

add_executable(YourBlock src/main.cpp)

target_include_directories(YourBlock PRIVATE src/)
target_link_libraries(YourBlock PRIVATE glfw glad glm imgui)
target_compile_definitions(YourBlock PRIVATE GLFW_INCLUDE_NONE)

find_package(OpenGL REQUIRED)
target_link_directories(YourBlock PRIVATE OpenGL::GL)

#file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)