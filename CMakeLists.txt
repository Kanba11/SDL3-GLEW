cmake_minimum_required(VERSION 3.31)
project(OpenGLTutorials LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

function(add_tutorial_project PROJECT_NAME)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/Source")
        file(GLOB SOURCES "${PROJECT_NAME}/Source/*.cpp")
        add_executable(${PROJECT_NAME} ${SOURCES})
        target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_NAME}/Header")
    else()
        add_executable(${PROJECT_NAME} "${PROJECT_NAME}/main.cpp")
    endif()
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
    )

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/Shaders")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/Shaders"
             DESTINATION "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/Resources")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/Resources"
             DESTINATION "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
    endif()

    target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3 GLEW OpenGL::GL)
endfunction()

add_tutorial_project(01-Window)
add_tutorial_project(02-Triangle)
add_tutorial_project(03-Quad)
add_tutorial_project(04-Shaders)
add_tutorial_project(05-Organizing)
add_tutorial_project(06-Textures)
add_tutorial_project(07-3DTextures)
add_tutorial_project(08-Camera)
add_tutorial_project(09-Lighting)
