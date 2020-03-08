#include "../include/Renderer.hpp"
#include "../include/Utils.hpp"
#include "../include/Mesh.hpp"
#include <chrono>

gravity::Renderer::Renderer() : mtl_cpp::Metal_API(), _device(getDevice()), _shaderCode(readFile(SHADER_SOURCE)) {
    _sema = dispatch_semaphore_create(BUFFER_SIZE);
}

void gravity::Renderer::onInitialize(void *view) {
    _lib = compileLibrary(_device, _shaderCode.data(), true);
    _queue = createCommandQueue(_device);
    _shader = createRenderState(view, _lib, _device, 4, "vertexShader", "fragmentShader");
    auto m = Mesh::makeCircle();
    _vertices = mallocBuffer(_device, m.vertices.data(), m.vertices.size()*sizeof(mathsimd::float2), mtl_cpp::Managed);
    _triangles = mallocBuffer(_device, m.triangles.data(), m.triangles.size()*sizeof(uint16_t), mtl_cpp::Managed);
}

void gravity::Renderer::onDraw(void *view) {
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

void gravity::Renderer::onSizeChange(void *view, unsigned long const size[2]) {
    _sampling = createMultiSamplingTexture(view, _device, size[0], size[1], 4);
}

void gravity::Renderer::updateCamera(gravity::Camera const& cam) {
    auto tmp = camera_t(cam);
    if (_camera[_buffer_idx].isEmpty()) {
        _camera[_buffer_idx] = mallocBuffer(_device, &tmp, sizeof(camera_t), mtl_cpp::Shared);
    } else {
        _camera[_buffer_idx].copy(&tmp, sizeof(camera_t));
    }
}
