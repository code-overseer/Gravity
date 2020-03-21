#ifndef GRAVITY_RENDERSYSTEM_HPP
#define GRAVITY_RENDERSYSTEM_HPP
#include <Metal_API.h>
#include <mathsimd.hpp>
#include <cstdint>
#include <dispatch/dispatch.h>
#include <functional>
#include "Camera.hpp"
#include "System.hpp"

namespace gravity::systems {
    struct RenderSystem : System {
    private:
        struct camera_t {
            mathsimd::float4x4 view, projection;
            explicit camera_t(Camera const& cam) : view(cam.view()), projection(cam.projection()) {}
        };
        struct RenderAPI : mtl_cpp::Metal_API {
            static constexpr uint8_t BUFFER_SIZE = 3;
            dispatch_semaphore_t sema;
            mtl_cpp::Buffer triangles;
            mtl_cpp::Buffer vertices;
            mtl_cpp::Buffer localToWorlds[BUFFER_SIZE];
            mtl_cpp::Buffer camera[BUFFER_SIZE];
            uint8_t buffer_idx = 0;
            uint32_t instanceCount = 0;
            mtl_cpp::Device device;
            mtl_cpp::Library lib;
            mtl_cpp::CommandQueue queue;
            mtl_cpp::Texture sampling;
            mtl_cpp::RenderPipelineState shader;
            std::string shaderCode;
            std::vector<std::function<void(unsigned long const size[2])>> resizeListeners;

            RenderAPI();

            void onInitialize(void* view) override;

            void onDraw(void* view) override;

            void onSizeChange(void* view, unsigned long const size[2]) override;
        };
        RenderAPI _renderer;
        friend class gravity::World;
        RenderSystem(gravity::World &w);
    public:
        void update(float delta) override;

        void addResizeListener(std::function<void(unsigned long const size[2])>&& delegate);
    };
}


#endif //GRAVITY_RENDERSYSTEM_HPP
