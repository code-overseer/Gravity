#ifndef GRAVITY_COMPONENTS_HPP
#define GRAVITY_COMPONENTS_HPP
#include <mathsimd.hpp>
namespace gravity::components {
    enum Tag { Grid0, Grid1, Grid2, Grid3, Grid4, Grid5, Grid6, Grid7 };
    struct Position {
        mathsimd::float2 val{0,0};
        Position(mathsimd::float2 const &x) : val(x) {};
        Position(float const &x, float const &y) : val(x,y) {};
        operator mathsimd::float2() { return val; }
    };
    struct Velocity {
        mathsimd::float2 val{0,0};
        Velocity(mathsimd::float2 const &x) : val(x) {};
        Velocity(float const &x, float const &y) : val(x,y) {};
        operator mathsimd::float2() { return val; }
    };
    struct CircleCollider {
        float radius;
        explicit CircleCollider(float r) : radius(r) {}
        AABB toAABB(mathsimd::float2 const&pos) { return AABB{pos - radius, pos + radius};}
        operator float() { return radius; }
    };
    struct Mass {
        float val;
        explicit Mass(float m) : val(m) {}
        operator float() { return val; }
    };
    struct Restitution {
        float val;
        explicit Restitution(float e) : val(e) {}
        operator float() { return val; }
    };
    struct LocalToWorld {
        mathsimd::float4x4 val = mathsimd::float4x4::identity();
        LocalToWorld(mathsimd::float4x4 const &translation, mathsimd::float4x4 const &rotation, mathsimd::float4x4 const &scale) {
            val = mathsimd::matmul(mathsimd::matmul(translation, rotation), scale);
        }
        explicit LocalToWorld(mathsimd::float4x4 const &ltw) {
            val = ltw;
        }
        explicit LocalToWorld(mathsimd::float2 const &position) {
            using namespace mathsimd;
            auto tmp = float4(position.x(), position.y(), 0, 1);
            val = float4x4(float4::right(), float4::up(), float4::forward(), tmp);
        }
        static LocalToWorld fromPositionAndRadius(mathsimd::float2 const &pos, float radius) {
            using namespace mathsimd;
            auto tmp = LocalToWorld(pos);
            mathsimd::float4x4 tmp1 = {radius*float4::right(),radius*float4::up(),radius*float4::forward(),float4::in()};
            tmp.val = matmul(tmp.val, tmp1);
            return tmp;
        }
    };

}

#endif //GRAVITY_COMPONENTS_HPP
