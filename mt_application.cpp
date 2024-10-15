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
    // Build path tracer
    buildPathTracePipeline();
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
        
        MTL::ComputeCommandEncoder* pComputeEncoder = pCmb->computeCommandEncoder();
        pComputeEncoder->setComputePipelineState(m_pPathTracePipelineState);
        pComputeEncoder->setTexture(m_renderFrame, 0);
        
        MTL::Size gridSize = MTL::Size(m_window->getWidth(), m_window->getHeight(), 1);
        NS::UInteger threadGroupSize = m_pPathTracePipelineState->maxTotalThreadsPerThreadgroup();
        MTL::Size threadgroupSize( threadGroupSize, 1, 1 );
        pComputeEncoder->dispatchThreads(gridSize, threadgroupSize);
        pComputeEncoder->endEncoding();
    
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

    MTL::TextureDescriptor* pPathTraceTextureDesc = MTL::TextureDescriptor::alloc()->init();
    pPathTraceTextureDesc->setWidth(m_window->getWidth());
    pPathTraceTextureDesc->setHeight(m_window->getHeight());
    pPathTraceTextureDesc->setPixelFormat(m_window->getFormat());

    MTL::Texture *pPathTraceTexture = m_pDevice->newTexture( pPathTraceTextureDesc );
    m_renderPathTracingSceneData = pPathTraceTexture;
    pPathTraceTextureDesc->release();
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
    m_renderPathTracingSceneData->release();
    m_renderFrame->release();
    m_pRenderQuadLibrary->release();
    m_queue->release();
    m_window->release();   
    m_pDevice->release();
}

void MtApplication::buildPathTracePipeline()
{   
    std::string kernelShaderSrc = readShaderFile("../Shader/tracer.metal");
    
    NS::Error* pError = nullptr;
    MTL::Library* pComputeLibrary = m_pDevice->newLibrary(NS::String::string(kernelShaderSrc.c_str(), NS::ASCIIStringEncoding), nullptr, &pError);
    if (!pComputeLibrary)
    {
        std::cout << pError->localizedDescription()->utf8String() << std::endl;
        throw std::runtime_error("Failed to create shader library :-(");
    }

    MTL::Function* pPathTracingFunction = pComputeLibrary->newFunction(NS::String::string("pathTrace", NS::ASCIIStringEncoding));
    m_pPathTracePipelineState = m_pDevice->newComputePipelineState(pPathTracingFunction, &pError);
    if (!m_pPathTracePipelineState)
    {
        std::cout << pError->localizedDescription()->utf8String() << std::endl;
        throw std::runtime_error("Failed to create compute pipeline state!");
    }
    
    pPathTracingFunction->release();
    pComputeLibrary->release();
}

void MtApplication::test() 
{
//    std::vector<simd::float3> triangleVertices;
//    triangleVertices.push_back(simd::make_float3(-0.5f, -0.5f, 0.0f ));
//    triangleVertices.push_back(simd::make_float3( 0.5f, -0.5f, 0.0f ));
//    triangleVertices.push_back(simd::make_float3( 0.0f,  0.5f, 0.0f ));
//
//    std::vector<uint32_t> indices = { 0, 1, 2 };
//
//    // Buffer define
//    MTL::Buffer* triangleBuffer = m_pDevice->newBuffer(&triangleVertices, triangleVertices.size() * sizeof(simd::float3), MTL::ResourceStorageModeShared);
//    MTL::Buffer* triangleIndicesBuffer = m_pDevice->newBuffer(&indices, indices.size() * sizeof(uint32_t), MTL::ResourceStorageModeShared);
//    
//    // Define Acceleration structure triangle geometry descriptor
//    MTL::AccelerationStructureTriangleGeometryDescriptor* triangleDesc = MTL::AccelerationStructureTriangleGeometryDescriptor::alloc()->init();
//    
//    triangleDesc->setIndexBuffer(triangleIndicesBuffer);
//    triangleDesc->setIndexType(MTL::IndexTypeUInt32);
//    triangleDesc->setVertexBuffer(triangleBuffer);
//    triangleDesc->setVertexStride(sizeof(simd::float3));
//    NS::UInteger triangleCount = (indices.size() / 3);
//    triangleDesc->setTriangleCount(triangleCount);
//    NS::UInteger tableOffset = 0;
//    triangleDesc->setIntersectionFunctionTableOffset(tableOffset);
//    
//    NS::Array* descArr = NS::Array::array(triangleDesc, 1);
//    
//    // Define Acceleration structure descriptor (different!!)
//    MTL::PrimitiveAccelerationStructureDescriptor* accelDesc = MTL::PrimitiveAccelerationStructureDescriptor::alloc()->init();
//    accelDesc->setGeometryDescriptors(descArr);
//
//    // Create Acceleration Structure!
//    MTL::AccelerationStructureSizes sizes = m_pDevice->accelerationStructureSizes(accelDesc);
//    
//    MTL::AccelerationStructure* accelerationStructure = m_pDevice->newAccelerationStructure(sizes.accelerationStructureSize);
//    MTL::Buffer* scratchBuffer = m_pDevice->newBuffer(sizes.buildScratchBufferSize, MTL::ResourceStorageModeShared);
//    
//    MTL::CommandBuffer* accelCmdbuf = m_queue->commandBuffer();
//    MTL::AccelerationStructureCommandEncoder* accelCmdEncoder = accelCmdbuf->accelerationStructureCommandEncoder();
//    
//    MTL::Buffer* compactSizeBuffer = m_pDevice->newBuffer(sizeof(uint32_t), MTL::ResourceStorageModeShared);
//    
//    accelCmdEncoder->buildAccelerationStructure(accelerationStructure,
//                                                accelDesc,
//                                                scratchBuffer,
//                                                0);
//    
//    accelCmdEncoder->writeCompactedAccelerationStructureSize(accelerationStructure,
//                                                             compactSizeBuffer,
//                                                             0);
//    
//    accelCmdEncoder->endEncoding();
//    accelCmdbuf->commit();
//    accelCmdbuf->waitUntilCompleted();
//    
//    uint32_t compactedSize = *(uint32_t*)compactSizeBuffer->contents();
//    
//    MTL::AccelerationStructure* compactedAccelerationStructure = m_pDevice->newAccelerationStructure(compactedSize);
//    
//    MTL::CommandBuffer* compactedAccelCmdbuf = m_queue->commandBuffer();
//    MTL::AccelerationStructureCommandEncoder* compactedAccelCmdEncoder = compactedAccelCmdbuf->accelerationStructureCommandEncoder();
//    compactedAccelCmdEncoder->copyAndCompactAccelerationStructure(accelerationStructure,
//                                                                  compactedAccelerationStructure);
//    compactedAccelCmdEncoder->endEncoding();
//    compactedAccelCmdbuf->commit();
//    std::cout << compactedAccelerationStructure->size() << std::endl;
    
}
