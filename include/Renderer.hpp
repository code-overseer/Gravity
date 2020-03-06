#ifndef GRAVITY_RENDERER_HPP
#define GRAVITY_RENDERER_HPP
#include <Metal_API.h>
#include <mathsimd.hpp>
#include "Camera.hpp"
#include <cstdint>
#include <dispatch/dispatch.h>
namespace gravity {
    struct Renderer : mtl_cpp::Metal_API {
        struct camera_t {
            mathsimd::float4x4 view, projection;
            camera_t(Camera const& cam) : view(cam.view()), projection(cam.projection()) {} // NOLINT(google-explicit-constructor)
        };
    private:
        static constexpr uint8_t BUFFER_SIZE = 3;
        dispatch_semaphore_t _sema;
        mtl_cpp::Buffer _triangles;
        mtl_cpp::Buffer _vertices;
        mtl_cpp::Buffer _localToWorlds[BUFFER_SIZE];
        mtl_cpp::Buffer _camera[BUFFER_SIZE];
        uint8_t _buffer_idx = 0;
        uint32_t _instanceCount = 0;

        mtl_cpp::Device _device;
        mtl_cpp::Library _lib;
        mtl_cpp::CommandQueue _queue;
        mtl_cpp::Texture _sampling;
        mtl_cpp::RenderPipelineState _shader;
        std::string _shaderCode;

    public:
        Renderer();

        void onInitialize(void* view) override;

        void onDraw(void* view) override;

        void onSizeChange(void* view, unsigned long const size[2]) override;

    };
}


#endif //GRAVITY_RENDERER_HPP
