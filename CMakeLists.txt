cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
project(Tracer)

add_subdirectory(vender)

set(SHADER_FILES
    shader/renderQuad.metal
    shader/triangle.metal
    shader/tracer.metal
)

set(HEADER_FILES
    mt_pch.h
    mt_window.h
    mt_application.h
    mt_window_connector.h
    mt_utils.h
)

set(SOURCE_FILES
    mt_pch.cpp
    main.cpp
    mt_window.cpp
    mt_application.cpp
    mt_window_connector.mm
    mt_utils.cpp
)

source_group(TREE ${CMAKE_SOURCE_DIR} PREFIX Header FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_SOURCE_DIR} PREFIX Source FILES ${SOURCE_FILES})
source_group(TREE ${CMAKE_SOURCE_DIR}/shader PREFIX Shader FILES ${SHADER_FILES})

add_executable(${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${SHADER_FILES}
)



target_link_libraries(${PROJECT_NAME} METAL_CPP_LIB)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} tinygltf)
target_link_libraries(${PROJECT_NAME} objc)



# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Game PREFIX Game FILES ${GAME_SRC})
