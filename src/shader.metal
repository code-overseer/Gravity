#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct Uniform {
    simd_float4x4 view;
    simd_float4x4 projection;
};

struct VertexOut {
    simd_float4 position [[position]];
};

vertex VertexOut vertexShader(constant const simd_float2* vertex [[buffer(0)]],
                                constant const simd_float4x4* ltw [[buffer(1)]],
                                constant const Uniform& cam [[buffer(2)]],
                                const uint vid [[vertex_id]],
                                const uint iid [[instance_id]]) {
    simd_float4x4 vp = cam.view * cam.projection;
    simd_float4 vert = simd_float4(vertex[vid], 0, 1);
    VertexOut output;
    output.position = vp * ltw[iid] * vert;
    return output;
}

fragment float4 fragmentShader(VertexOut in [[stage_in]]) {
    return float4(1,0,0,1);
}