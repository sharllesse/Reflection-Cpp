#include "reflect.h"

#include <iostream>

//The reflection system doesn't work with const function and I think it is because of the const propagation.
//And I did not test it with static, but I'm sure it does not work since static member are not called in the same way as a normal one.

class foo_bar
{
    REFLECT_PRIVATE_ACCESS(foo_bar)

public:
    foo_bar() = default;
    ~foo_bar() = default;

    void print()
    {
        std::cout << "foo is equal to " << m_foo << " and bar to " << m_bar << '\n';
    }

    int* get_foo()
    {
        return &m_foo;
    }

    int& get_bar()
    {
        return m_bar;
    }

    const int& get_other()
    {
        return other;
    }

    int other{0};
private:
    int m_foo{10};
    int m_bar{20};
};

REFLECT_BEGIN(foo_bar)
    REFLECT_FUNCTION(print),
    REFLECT_FUNCTION(get_foo),
    REFLECT_FUNCTION(get_bar),
    REFLECT_FUNCTION(get_other),
    REFLECT_VARIABLE(other),
    REFLECT_VARIABLE(m_foo),
    REFLECT_VARIABLE(m_bar)
REFLECT_END()

int main(int argc, char* argv[])
{
    reflect<foo_bar>::for_each_field([](const auto& field)
    {
       std::cout << field.get_name() << '\n';
    });

    foo_bar instance;
    foo_bar* instance_ptr{&instance};

    auto print_function{reflect<foo_bar>::get_field_by_name<"print">()};
    print_function.invoke(instance);

    auto get_foo{reflect<foo_bar>::get_field_by_name<"get_foo">()};
    int* foo_ref{get_foo.invoke(instance)};
    *foo_ref *= 100;

    print_function.invoke(instance_ptr);

    return 0;
}