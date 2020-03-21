#ifndef GRAVITY_CAMERA_HPP
#define GRAVITY_CAMERA_HPP
#include <mathsimd.hpp>

namespace gravity {
    struct Camera {
    private:
        float zoom_ = 4500.f;
        float rcp_aspect_ = 0.666666667f; // height/width
        mathsimd::float2 position_{0,0};
        // far = 8.1, near = 0.1
    public:
        [[nodiscard]] mathsimd::float4x4 view() const {
            return mathsimd::float4x4(mathsimd::float4(1, 0, 0, 0),
                               mathsimd::float4(0, 1, 0, 0),
                               mathsimd::float4(0, 0, 1, 0),
                               mathsimd::float4(-position_, 1, 1));
        }
        [[nodiscard]] mathsimd::float4x4 projection() const {
            return mathsimd::float4x4(mathsimd::float4(2 * rcp_aspect_/zoom_, 0,0,0),
                                      mathsimd::float4(0, 2/zoom_, 0, 0),
                                      mathsimd::float4(0, 0,0,0),
                                      mathsimd::float4(0, 0,0,1));
        }
        [[nodiscard]] mathsimd::float4x4 inv_view() const {
            return mathsimd::float4x4(mathsimd::float4(1, 0, 0, 0),
                                      mathsimd::float4(0, 1, 0, 0),
                                      mathsimd::float4(0, 0, 1, 0),
                                      mathsimd::float4(position_, -1, 1));
        }
        [[nodiscard]] mathsimd::float4x4 inv_projection() const {
            return mathsimd::float4x4(mathsimd::float4(0.5f * zoom_/rcp_aspect_, 0,0,0),
                                      mathsimd::float4(0, 0.5f * zoom_, 0, 0),
                                      mathsimd::float4(0, 0,-4,0),
                                      mathsimd::float4(0, 0,-4.1f,1));
        }
        [[nodiscard]] float zoom() const { return zoom_; }
        [[nodiscard]] float rcp_aspect() const { return rcp_aspect_; }
        [[nodiscard]] mathsimd::float2 position() const { return position_; }
        void zoom(float z) { zoom_ = z; }
        void set_aspect_ratio(float w, float h) { rcp_aspect_ = h / w; }
        void position(mathsimd::float2 const &p) { position_ = p; }
    };
}


#endif //GRAVITY_CAMERA_HPP
