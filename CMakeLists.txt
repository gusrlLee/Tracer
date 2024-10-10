cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
project(Tracer)

add_subdirectory(vender)

add_executable(${PROJECT_NAME}
    mt_pch.h
    mt_pch.cpp
    main.cpp

    mt_window.h
    mt_window.cpp

    mt_application.h
    mt_application.cpp
    
    mt_window_connector.h
    mt_window_connector.mm

    mt_utils.h
    mt_utils.cpp
)

target_link_libraries(${PROJECT_NAME} METAL_CPP_LIB)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} tinygltf)
target_link_libraries(${PROJECT_NAME} objc)
