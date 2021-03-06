cmake_minimum_required(VERSION 3.17)
project(OpenGLEngine)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(OpenGLEngine
        main.cpp
        src/stdopengl.h
        src/main/define.h
        src/main/stb/stb_image.h
        src/main/common/camera.h
        src/main/common/object.h
        src/main/common/window.h
        src/main/component/renderer.h
        src/main/component/filter.h
        src/main/component/transform.h
        src/main/component/light.h
        src/main/component/direction_light.h
        src/main/component/point_light.h
        src/main/component/spot_light.h
        src/main/component/material.h
        src/main/component/mesh.h)

target_link_libraries(OpenGLEngine ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)
