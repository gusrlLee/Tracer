#pragma once 
#include "mt_pch.h"

class MtPathTracer {
public:
    void prepare(MTL::Device* pDevice);
    void release();
    void encoding(MTL::CommandBuffer pCmd);
    
private:
    MTL::Device* m_pDevice;
    MTL::Buffer* m_buffer;
    MTL::Texture* m_renderScene;
};
