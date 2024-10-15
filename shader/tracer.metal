#include <metal_stdlib>
using namespace metal;

kernel 
void pathTrace (
    texture2d<float, access::write > image [[texture(0)]] ,  // Render Scene 
    uint2 coord [[thread_position_in_grid]], // thread index 
    uint2 size [[threads_per_grid]] // grid size 
) {
    float2 uv = float2(coord) / float2(size - 1);
    image.write(float4(uv, 0.0, 1.0), coord);
}
