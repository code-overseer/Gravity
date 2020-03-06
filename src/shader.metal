#include <metal_stdlib>

using namespace metal;

struct Uniform {
    float4x4 view;
    float4x4 projection;
};

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

constant constexpr static const float4 TRI[3]
{
{-0.5f, -0.5f, 0.f, 1.f},
{ 0.5f, -0.5f, 0.f, 1.f},
{  0.f, 0.5f, 0.f, 1.f}
};

constant constexpr static const float4 COL[3]
{
{1.f, 0.f, 0.f, 1.f},
{ 0.f, 1.f, 0.f, 1.f},
{  0.f, 0.f, 1.f, 1.f}
};

vertex VertexOut helloTriangle(const uint vid [[vertex_id]]) {
    VertexOut output;
    output.position = TRI[vid];
    output.color = COL[vid];
    return output;
}

vertex VertexOut vertexShader(constant const float2* verts [[buffer(0)]],
                                constant const float4x4* ltw [[buffer(1)]],
                                constant const Uniform& cam [[buffer(2)]],
                                const uint vid [[vertex_id]],
                                const uint iid [[instance_id]]) {
    float4x4 pv = cam.projection * cam.view;
    float4 vert = float4(verts[vid], 0, 1);
    VertexOut output;
    output.position = pv * ltw[iid] * vert;
    output.color = float4(1,0,0,1);

    return output;
}

fragment float4 fragmentShader(VertexOut in [[stage_in]]) {
    return in.color;
}