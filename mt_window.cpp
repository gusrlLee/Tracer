#include "mt_window.h"
#include "mt_window_connector.h"

#include "Metal/MTLPixelFormat.hpp"


void MtWindow::prepare(MTL::Device* pDevice) 
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_width = 800;
    m_height = 600;
    m_windowFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm;
    m_window = glfwCreateWindow(m_width, m_height, "Tracer", NULL, NULL);
    if (!m_window)
    {
        std::runtime_error("Failed to create window! :-(");
    }

    m_layer = CA::MetalLayer::layer()->retain();
    m_layer->setDevice(pDevice);
    m_layer->setPixelFormat(m_windowFormat);
    m_nsWindow = MTL::ConnectWindow(m_window, m_layer)->retain();

    m_renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    MTL::RenderPassColorAttachmentDescriptor* colorAttachment = m_renderPassDescriptor->colorAttachments()->object(0);
    colorAttachment->setLoadAction(MTL::LoadActionClear);
    colorAttachment->setClearColor(MTL::ClearColor(0.75f, 0.25f, 0.125f, 1.0f));
    colorAttachment->setStoreAction(MTL::StoreActionStore);
}

void MtWindow::update() 
{
    glfwPollEvents();
}

bool MtWindow::isShouldClose()
{
    return glfwWindowShouldClose(m_window);
}

CA::MetalDrawable* MtWindow::nextDrawable()
{
    return m_layer->nextDrawable();
}

MTL::RenderPassDescriptor* MtWindow::getCurrentRenderPassDiscriptor(CA::MetalDrawable* drawable)
{
    MTL::RenderPassColorAttachmentDescriptor* colorAttachment = m_renderPassDescriptor->colorAttachments()->object(0);
    colorAttachment->setTexture(drawable->texture());
    return m_renderPassDescriptor;
}

void MtWindow::release()
{
    m_renderPassDescriptor->release();
    m_nsWindow->release();
    m_layer->release();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}