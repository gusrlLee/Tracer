#include "mt_application.h"
#include "mt_utils.h"

#include "Metal/MTLPixelFormat.hpp"
#include <simd/simd.h>

void MtApplication::prepare()
{
    m_pDevice = MTL::CreateSystemDefaultDevice();
    if (!m_pDevice)
    {
        std::runtime_error("Failed to find default device! :-(");
    }
    m_window = make<MtWindow>();
    m_window->prepare(m_pDevice);

    m_queue = m_pDevice->newCommandQueue()->retain();

    // Render Quad 
    buildBasicRenderQuadShader();
    // Render Frame
    buildRenderFrame();
}

void MtApplication::run() 
{
    CA::MetalDrawable* bIsDrawable;
    while (!m_window->isShouldClose())
    {
        m_window->update(); // this function call glfwPollEvent();
        NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
        MTL::CommandBuffer* pCmb = m_queue->commandBuffer();
        bIsDrawable = m_window->nextDrawable();

        

        MTL::RenderPassDescriptor* pRpd = m_window->getCurrentRenderPassDiscriptor(bIsDrawable);
        MTL::RenderCommandEncoder* pEnc = pCmb->renderCommandEncoder(pRpd);
        {
            MTL::PrimitiveType typeTriangle = MTL::PrimitiveTypeTriangle;
            pEnc->setRenderPipelineState(m_pPSO);
            pEnc->setFragmentTexture(m_renderFrame, 0);
            NS::UInteger vertexStart = 0;
            NS::UInteger vertexCount = 6;
            pEnc->drawPrimitives(typeTriangle, vertexStart, vertexCount);
        }
        pEnc->endEncoding();

        pCmb->presentDrawable(bIsDrawable);
        pCmb->commit();
        pCmb->waitUntilCompleted();
        pool->release();
    }
}

void MtApplication::buildRenderFrame() 
{
    MTL::TextureDescriptor* pTextureDesc = MTL::TextureDescriptor::alloc()->init();
    pTextureDesc->setWidth(m_window->getWidth());
    pTextureDesc->setHeight(m_window->getHeight());
    pTextureDesc->setPixelFormat(m_window->getFormat());

    MTL::Texture *pTexture = m_pDevice->newTexture( pTextureDesc );
    m_renderFrame = pTexture;
    pTextureDesc->release();
}

void MtApplication::buildBasicRenderQuadShader() 
{
    NS::Error *pError = nullptr;
    std::string basicRenderQuadShaderSrc = readShaderFile("../Shader/renderQuad.metal");
    MTL::Library* pLibrary = m_pDevice->newLibrary(NS::String::string(basicRenderQuadShaderSrc.c_str(), NS::ASCIIStringEncoding), nullptr, &pError);
    if (!pLibrary)
    {   
        std::cout << pError->localizedDescription()->utf8String() << std::endl;
        throw std::runtime_error("Failed to create basic render quad library!");
    }
    MTL::Function* pVertexFunction = pLibrary->newFunction(NS::String::string("copyVertex", NS::ASCIIStringEncoding));
    if (pVertexFunction == nullptr) {
        std::cout << pError->localizedDescription()->utf8String() << std::endl;
        throw std::runtime_error("Failed to create vertex shader function");
    }
    MTL::Function* pFragmentFunction = pLibrary->newFunction(NS::String::string("copyFragment", NS::ASCIIStringEncoding));
    if (pFragmentFunction == nullptr) {
        std::cout << pError->localizedDescription()->utf8String() << std::endl;
        throw std::runtime_error("Failed to create fragment shader function");
    }

    MTL::RenderPipelineDescriptor* pRenderQuadPipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    pRenderQuadPipelineDesc->setLabel(NS::String::string("Basic RenderQuad", NS::ASCIIStringEncoding));
    pRenderQuadPipelineDesc->setVertexFunction(pVertexFunction);
    pRenderQuadPipelineDesc->setFragmentFunction(pFragmentFunction);
    pRenderQuadPipelineDesc->colorAttachments()->object(0)->setPixelFormat(m_window->getFormat());
    assert(pRenderQuadPipelineDesc);

    /* RenderPipelineState */ 
    m_pPSO = m_pDevice->newRenderPipelineState(pRenderQuadPipelineDesc, &pError);
    if (!m_pPSO)
    {
        std::cout << pError->localizedDescription()->utf8String() << std::endl;
        throw std::runtime_error("Failed to create RenderQuad render pipeline state");
    }

    m_pRenderQuadLibrary = pLibrary;
    m_pRenderQuadPipelineDesc = pRenderQuadPipelineDesc;
}

void MtApplication::renderQuad(MTL::CommandBuffer* pCmb)
{
    return;
}

void MtApplication::release()
{
    m_renderFrame->release();
    m_pRenderQuadLibrary->release();
    m_queue->release();
    m_window->release();   
    m_pDevice->release();
}

void MtApplication::test() 
{
    simd::float3 triangleVertices[] = {
        {-0.5f, -0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { 0.0f,  0.5f, 0.0f}
    };
    

//     MTL::Buffer* triangleBuffer = m_pDevice->newBuffer(&triangleVertices, sizeof(triangleVertices), MTL::ResourceStorageModeShared);
//     MTL::AccelerationStructureTriangleGeometryDescriptor* accelerationStructureDescriptor = MTL::AccelerationStructureTriangleGeometryDescriptor::alloc()->init();
// //    accelerationStructureDescriptor->setIndexBuffer();
//     accelerationStructureDescriptor->setIndexType();
//     accelerationStructureDescriptor->setVertexBuffer(const MT);
//     accelerationStructureDescriptor->setVertexStride();
//     accelerationStructureDescriptor->setVertexBufferOffset()
}
