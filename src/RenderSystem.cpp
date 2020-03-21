#include "../include/RenderSystem.hpp"
#include "../include/Utils.hpp"
#include "../include/Mesh.hpp"
#include "../include/World.hpp"
#include "../include/components.hpp"
#include <chrono>

gravity::systems::RenderSystem::RenderSystem(World& w) : mtl_cpp::Metal_API(), System(w), _device(getDevice()), _shaderCode(readFile(SHADER_SOURCE)) {
    _sema = dispatch_semaphore_create(BUFFER_SIZE);
    using namespace gravity::components;
    w.registry().group<Position, CircleCollider, LocalToWorld>();
}

void gravity::systems::RenderSystem::onInitialize(void *view) {
    _lib = compileLibrary(_device, _shaderCode.data(), true);
    _queue = createCommandQueue(_device);
    _shader = createRenderState(view, _lib, _device, 4, "vertexShader", "fragmentShader");
    auto m = Mesh::makeCircle();
    _vertices = mallocBuffer(_device, m.vertices.data(), m.vertices.size()*sizeof(mathsimd::float2), mtl_cpp::Managed);
    _triangles = mallocBuffer(_device, m.triangles.data(), m.triangles.size()*sizeof(uint16_t), mtl_cpp::Managed);
}

void gravity::systems::RenderSystem::onDraw(void *view) {
    if (!_instanceCount) return;
    using namespace std::chrono;
    const double target = getTime() + 0.016667;
    static auto handler = [this]() {
        dispatch_semaphore_signal(_sema);
    };

    dispatch_semaphore_wait(_sema, DISPATCH_TIME_FOREVER);
    auto buffer = getCommandBuffer(_queue);
    addCompletionHandler(buffer, handler);

    /* Update dynamic buffers */
    localToWorlds_[_buffer_idx].flush();
    /* End */
    auto encoder = getRenderCommandEncoder(view, buffer, _sampling);
    setState(encoder, _shader);

    setVertexBuffer(encoder, _vertices, 0, 0);
    setVertexBuffer(encoder, localToWorlds_[_buffer_idx], 0, 1);
    setVertexBuffer(encoder, _camera[_buffer_idx], 0, 2);
    drawMesh(encoder, mtl_cpp::Triangle, _triangles.length<uint16_t>(),
            mtl_cpp::UINT16, _triangles, 0, _instanceCount);

    endEncoding(encoder);
    presentDrawable(view, buffer, target);
    commitCommandBuffer(buffer);
    _buffer_idx = (_buffer_idx + 1) % BUFFER_SIZE;
}

void gravity::systems::RenderSystem::onSizeChange(void *view, unsigned long const size[2]) {
    world().interrupt();
    _sampling = createMultiSamplingTexture(view, _device, size[0], size[1], 4);
    world().mainCamera().set_aspect_ratio(static_cast<float>(size[0]), static_cast<float>(size[1]));
}

void gravity::systems::RenderSystem::update(float delta) {
    using namespace gravity::components;
    auto tmp = camera_t(world().mainCamera());
    if (_camera[_buffer_idx].isEmpty()) {
        _camera[_buffer_idx] = mallocBuffer(_device, &tmp, sizeof(camera_t), mtl_cpp::Shared);
    } else {
        _camera[_buffer_idx].copy(&tmp, sizeof(camera_t));
    }

    auto group = world().registry().group<const Position, const CircleCollider, LocalToWorld>();
    group.each([](Position const&pos, CircleCollider const& col, LocalToWorld& ltw) {
        ltw = components::LocalToWorld::fromPositionAndRadius(pos.val, col.radius);
    });

    // update ltw buffer
    auto &buffer = localToWorlds_[_buffer_idx];
    if (buffer.isEmpty() || buffer.length<LocalToWorld>() != group.size<LocalToWorld>()) {
        buffer = RenderSystem::mallocBuffer(_device, group.raw<LocalToWorld>(), group.size() * sizeof(LocalToWorld), mtl_cpp::Managed);
    } else {
        buffer.copy(group.raw<LocalToWorld>(), group.size() * sizeof(components::LocalToWorld));
    }
    _instanceCount = group.size();
}
