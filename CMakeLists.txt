cmake_minimum_required(VERSION 3.20)
project(YourBlock VERSION 0.1.0 LANGUAGES CXX C)

# 屏蔽pkg-config检索，消除Windows报错警告
set(PKG_CONFIG_FOUND OFF CACHE INTERNAL "")
find_package(PkgConfig QUIET)

# C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 全局静态库设置
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# freetype裁剪配置
set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)

# 前置查找OpenGL
find_package(OpenGL REQUIRED)

# 第三方子库
add_subdirectory(libs/freetype)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)

# glad静态库
add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

# imgui静态库
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

# 主程序源码
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(YourBlock ${SOURCES})

# 复制资源目录到构建输出
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)

# 头文件路径
target_include_directories(YourBlock PRIVATE 
    src/
    libs/
)

# 统一链接所有依赖（顺序：应用层→第三方→系统OpenGL）
target_link_libraries(YourBlock PRIVATE 
    freetype
    glfw 
    glad 
    glm 
    imgui 
    OpenGL::GL
)
target_compile_definitions(YourBlock PRIVATE GLFW_INCLUDE_NONE)