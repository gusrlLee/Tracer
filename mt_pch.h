#pragma once
#ifndef __MT_PCH__
#define __MT_PCH__
// GLFW library
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

// Metal API
#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp> 
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <AppKit/AppKit.hpp>

// Standard headers
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <optional>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

// TinyglTF Library 
#include "tiny_gltf.h"

template<typename T>
using ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr ref<T> make(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif // __MT_PCH__