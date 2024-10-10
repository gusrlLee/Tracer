#include <metal_stdlib>
using namespace metal;

constant float2 quadVertices[] = {
    float2(-1, -1),
    float2(-1,  1),
    float2( 1,  1),
    float2(-1, -1),
    float2( 1,  1),
    float2( 1, -1)
};

struct CopyVertexOut {
    float4 position [[position]];
    float2 uv;
};

vertex 
CopyVertexOut copyVertex(unsigned short vid [[vertex_id]]) {
    float2 position = quadVertices[vid];
    CopyVertexOut out;
    out.position = float4(position, 0, 1);
    out.uv = position * 0.5f + 0.5f;
    return out;
}

// Simple fragment shader that copies a texture and applies a simple tonemapping function.
fragment 
float4 copyFragment(CopyVertexOut in [[stage_in]],
                             texture2d<float> tex)
{
    constexpr sampler sam(min_filter::nearest, mag_filter::nearest, mip_filter::none);

    //float3 color = tex.sample(sam, in.uv).xyz;
    //color = color / (1.0f + color);
    float3 color = float3(0.0f, 0.0f, 1.0f);
    return float4(color, 1.0f);
}
