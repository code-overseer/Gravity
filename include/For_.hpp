

#ifndef GRAVITY_FOR__HPP
#define GRAVITY_FOR__HPP
namespace gravity {
    template<class F, int... Is>
    void for_(F func, std::integer_sequence<int, Is...>) {
        using expander = int[];
        (void) expander{0, ((void) func(std::integral_constant<decltype(Is), Is>{}), 0)...};
    }

    template<int N, typename F>
    void for_(F func) {
        for_(func, std::make_integer_sequence<decltype(N), N>());
    }
}
#endif //GRAVITY_FOR__HPP
