#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class A
{
    public:
        A() {}
        ~A() {}
        __attribute__((noinline)) void set(const uint64_t i) { m_i = i; }
        __attribute__((noinline)) uint64_t get() { return m_i; }

    private:
        volatile uint64_t m_i;
};

template<typename T>
constexpr void call_set(T& obj, const uint64_t i)
{
    constexpr void (T::*fn)(const uint64_t i) = &T::set;
    (obj.*fn)(i);
}


auto main() -> int
{
    // Direct call
    A a;
    a.set(42);

    // Member function pointer
    constexpr void (A::*fn)(const uint64_t i) = &A::set;
    (a.*fn)(42);

    // Function pointer via template
    call_set(a, 1);

    // Function pointer via std::function
    const std::function<void(A*, const uint64_t)> fn_fn = std::mem_fn(&A::set);
    fn_fn(&a, 2);

    // Via lambda wth std::bind
    const std::function<void(A*, const uint64_t)> fn_lambda = std::bind(&A::set, std::placeholders::_1, std::placeholders::_2);
    fn_lambda(&a, 3);

    // via lambda
    auto fn_lambda2 = [](A* _a, const uint64_t i) { _a->set(i); };
    fn_lambda2(&a, 4);

    return 0;
}
