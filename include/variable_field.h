#ifndef REFLECTION_CPP_VARIABLE_FIELD_H
#define REFLECTION_CPP_VARIABLE_FIELD_H

#include "compile_string.h"

#include <string_view>

template<auto, compile_string>
class variable_field;

template<typename CTy_, typename VTy_, VTy_ CTy_::* field, compile_string field_name>
class variable_field<field, field_name>
{
    static_assert(field != nullptr, "the field cannot be nullptr.");

public:
    using class_type = CTy_;
    using variable_type = VTy_;
    using variable_field_type = VTy_ CTy_::*;

    constexpr static variable_field_type s_field{field};
    constexpr static std::string_view s_field_name{field_name};

    variable_field() = default;
    variable_field(const variable_field&) = default;
    variable_field(variable_field&&) noexcept = default;
    ~variable_field() = default;

    variable_field& operator=(const variable_field&) = default;
    variable_field& operator=(variable_field&&) noexcept = default;

    template<typename ClassType>
        requires(std::derived_from<std::remove_cvref_t<ClassType>, class_type>)
    constexpr const variable_type& get(ClassType&& _instance) const
    {
        return _instance.*s_field;
    }

    template<typename ClassType, typename VariableType>
        requires(std::derived_from<std::remove_cvref_t<ClassType>, class_type>)
    constexpr void set(ClassType&& _instance, VariableType&& _variable) const
    {
        _instance.*s_field = std::forward<VariableType>(_variable);
    }

    template<typename ClassType>
        requires(std::derived_from<std::remove_pointer_t<ClassType>, class_type>)
    constexpr const variable_type& get(ClassType* _instance) const
    {
        return _instance->*s_field;
    }

    template<typename ClassType, typename VariableType>
        requires(std::derived_from<std::remove_pointer_t<ClassType>, class_type>)
    constexpr void set(ClassType* _instance, VariableType&& _variable) const
    {
        _instance->*s_field = std::forward<VariableType>(_variable);
    }

    [[nodiscard]] constexpr std::string_view get_name() const
    {
        return s_field_name;
    }
};



#endif //REFLECTION_CPP_VARIABLE_FIELD_H
