#ifndef REFLECTION_CPP_REFLECT_H
#define REFLECTION_CPP_REFLECT_H

#include "function_field.h"
#include "variable_field.h"

template<typename Cty_>
class reflect;

#define REFLECT_PRIVATE_ACCESS(ClassType)\
    friend reflect<ClassType>;           \

#define REFLECT_BEGIN(ClassType)                               \
template<>                                                     \
class reflect<ClassType>                                       \
{                                                              \
public:                                                        \
    using class_type = ClassType;                              \
    static constexpr std::string_view s_class_name{#ClassType};\
    using fields_type = std::tuple<                            \

#define REFLECT_VARIABLE(Variable)                      \
        variable_field<&class_type::Variable, #Variable>\

#define REFLECT_FUNCTION(Function)                      \
        function_field<&class_type::Function, #Function>\

#define REFLECT_END()                                                                                                        \
    >;                                                                                                                       \
    static constexpr fields_type s_fields;                                                                                   \
                                                                                                                             \
    template<compile_string name>                                                                                            \
    [[nodiscard]] static consteval auto get_field_by_name()                                                                  \
    {                                                                                                                        \
        return get_field_by_name_impl<name, 0>();                                                                            \
    }                                                                                                                        \
                                                                                                                             \
    template<typename VisitorType>                                                                                           \
    static constexpr void for_each_field(VisitorType&& _visitor)                                                             \
    {                                                                                                                        \
        for_each_field_impl(std::forward<VisitorType>(_visitor), std::make_index_sequence<std::tuple_size_v<fields_type>>{});\
    }                                                                                                                        \
private:                                                                                                                     \
    template<compile_string name, size_t index>                                                                              \
    static consteval auto get_field_by_name_impl()                                                                           \
    {                                                                                                                        \
        static_assert(std::tuple_size_v<fields_type> > index, "The field has not been found.");                              \
                                                                                                                             \
        constexpr auto& field = std::get<index>(s_fields);                                                                   \
        if constexpr (field.get_name() == static_cast<const char*>(name))                                                    \
        {                                                                                                                    \
            return field;                                                                                                    \
        }                                                                                                                    \
        else                                                                                                                 \
        {                                                                                                                    \
            return get_field_by_name_impl<name, index + 1>();                                                                \
        }                                                                                                                    \
    }                                                                                                                        \
                                                                                                                             \
    template<typename Visitor, size_t ...index>                                                                              \
    static constexpr void for_each_field_impl(Visitor&& _visitor, std::index_sequence<index...>)                             \
    {                                                                                                                        \
        (_visitor(std::get<index>(s_fields)), ...);                                                                          \
    }                                                                                                                        \
};                                                                                                                           \

#endif //REFLECTION_CPP_REFLECT_H
