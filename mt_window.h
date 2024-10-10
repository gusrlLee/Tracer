#pragma once 
#include "mt_pch.h"

class MtWindow {
    public:
        void prepare(MTL::Device* pDevice);
        void update();

        int getWidth() { return m_width; }
        int getHeight() { return m_height; }
        MTL::PixelFormat getFormat() { return m_windowFormat; }

        bool isShouldClose();
        CA::MetalDrawable* nextDrawable();
        MTL::RenderPassDescriptor* getCurrentRenderPassDiscriptor(CA::MetalDrawable* drawable);

        void release();

    private:
        int m_width;
        int m_height;
        MTL::PixelFormat m_windowFormat;
        MTL::RenderPassDescriptor* m_renderPassDescriptor;
        GLFWwindow* m_window;
        CA::MetalLayer* m_layer;
        NS::Window* m_nsWindow;
};  