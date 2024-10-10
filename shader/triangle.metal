#include <metal_stdlib>

using namespace metal;

vertex float4 
vertexShader(uint vertexID [[vertex_id]], constant simd::float3* vertexPosition)
{

}

fragment float4 
fragmentShader(float4 vertexOutPosition [[stage_in]])
{
    return float4(0.0f);
}