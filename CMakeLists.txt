cmake_minimum_required(VERSION 3.20)
project(YourBlock VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)

add_library(glad STATIC libs/glad/src/glad.c)

target_include_directories(glad PUBLIC libs/glad/include)

set(IMGUI_SOURCES
    libs/Imgui/imgui.cpp
    libs/Imgui/imgui_draw.cpp
    libs/Imgui/imgui_tables.cpp
    libs/Imgui/imgui_widgets.cpp
    libs/Imgui/backends/imgui_impl_glfw.cpp
    libs/Imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC 
    libs/Imgui
    libs/Imgui/backends
)

target_link_libraries(imgui PRIVATE glfw OpenGL::GL glad)
target_compile_definitions(imgui PRIVATE GLFW_INCLUDE_NONE)


file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(YourBlock ${SOURCES})
# 将 assets 文件夹复制到可执行文件所在的构建目录
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)


find_package(OpenGL REQUIRED)
target_include_directories(YourBlock PRIVATE src/)
target_include_directories(YourBlock PRIVATE libs/)
target_link_libraries(YourBlock PRIVATE glfw glad glm imgui OpenGL::GL)
target_compile_definitions(YourBlock PRIVATE GLFW_INCLUDE_NONE)


#file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)