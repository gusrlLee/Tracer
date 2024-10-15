#include "mt_path_tracer.h"
#include "mt_utils.h"

void MtPathTracer::prepare(MTL::Device* pDevice)
{
    std::string kernelShaderSrc = readShaderFile("../Shader/tracer.metal");
    
    NS::Error* pError = nullptr;
    MTL::Library* mComputeLibrary = pDevice->newLibrary(NS::String::string(kernelShaderSrc.c_str(), NS::ASCIIStringEncoding), nullptr, &pError);
    if (!mComputeLibrary)
    {
        std::cout << pError->localizedDescription()->utf8String() << std::endl;
        throw std::runtime_error("Failed to create shader library :-(");
    }
    
    MTL::Function* pPathTracingFunction = mComputeLibrary->newFunction(NS::String::string("pathTracer", NS::ASCIIStringEncoding));
    MTL::ComputePipelineState* mComputePipelineState = pDevice->newComputePipelineState(pPathTracingFunction, &pError);
    if (!mComputePipelineState)
    {
        std::cout << pError->localizedDescription()->utf8String() << std::endl;
        throw std::runtime_error("Failed to create compute pipeline state!");
    }
    pPathTracingFunction->release();
    mComputeLibrary->release();
    
    
}

void MtPathTracer::release()
{

}

void MtPathTracer::encoding(MTL::CommandBuffer pCmd)
{

}

