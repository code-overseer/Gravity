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
        operator float() { return radius; }
    };
    struct Mass {
        float val;
        explicit Mass(float m) : val(m) {}
        operator float() { return val; }
    };

}




#endif //GRAVITY_COMPONENTS_HPP
