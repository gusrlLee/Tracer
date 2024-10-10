
FILE(GLOB FOUNDATION_HEADERS "Foundation/**.hpp")
FILE(GLOB METAL_HEADERS "Metal/**.hpp")
FILE(GLOB METALFX_HEADERS "MetalFX/**.hpp")
FILE(GLOB QUARTZ_CORE_HEADERS "QuartzCore/**.hpp")
FILE(GLOB APPKIT_HEADERS "AppKit/**.hpp")
FILE(GLOB METALKIT_HEADERS "MetalKit/**.hpp")

add_library(METAL_CPP_LIB
    ${CMAKE_CURRENT_SOURCE_DIR}/metal_defination.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/metal_defination.h
    ${FOUNDATION_HEADERS}
    ${METAL_HEADERS}
    ${METALFX_HEADERS}
    ${QUARTZ_CORE_HEADERS}
    ${APPKIT_HEADERS}
    ${METALKIT_HEADERS}
)

target_include_directories(METAL_CPP_LIB PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_link_libraries(METAL_CPP_LIB PUBLIC
    "-framework Metal"
    "-framework MetalKit"
    "-framework AppKit"
    "-framework Foundation"
    "-framework QuartzCore"
)
