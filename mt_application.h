#pragma once
#include "mt_pch.h"
#include "mt_window.h"

class MtApplication {
public:
    void prepare();
    void run();
    void release();
    void test();
    
private:
    void buildBasicRenderQuadShader();
    void buildRenderFrame();
    void buildPathTracePipeline();
    void renderQuad(MTL::CommandBuffer* pCmb);
    
    MTL::Device* m_pDevice;
    ref<MtWindow> m_window;
    MTL::CommandQueue* m_queue;
    MTL::Library* m_pRenderQuadLibrary;

    // For render frame     
    MTL::RenderPipelineDescriptor* m_pRenderQuadPipelineDesc;
    MTL::RenderPipelineState* m_pPSO;
    MTL::Texture* m_renderFrame;
    
    // For path tracer 
    MTL::ComputePipelineState* m_pPathTracePipelineState;
    MTL::Texture* m_renderPathTracingSceneData;
};
