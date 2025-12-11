#ifndef REFLECTION_C_FUNCTION_FIELD_H
#define REFLECTION_C_FUNCTION_FIELD_H

#include "compile_string.h"

#include <string_view>
#include <tuple>

template<auto, compile_string>
class function_field;

template<typename RTy_, typename CTy_, typename ...ArgsTy_, RTy_ (CTy_::* field)(ArgsTy_...), compile_string field_name>
class function_field<field, field_name>
{
    static_assert(field != nullptr, "the field cannot be nullptr.");

public:
    using return_type = RTy_;
    using class_type = CTy_;
    using args_type = std::tuple<ArgsTy_...>;
    using function_field_type = RTy_ (CTy_::*)(ArgsTy_...);

    constexpr static function_field_type s_field{field};
    constexpr static std::string_view s_field_name{field_name};

    function_field() = default;
    function_field(const function_field&) = default;
    function_field(function_field&&) noexcept = default;
    ~function_field() = default;

    function_field& operator=(const function_field&) = default;
    function_field& operator=(function_field&&) noexcept = default;

    template<typename ClassType, typename ...Args>
        requires(std::derived_from<std::remove_cvref_t<ClassType>, class_type>)
    constexpr return_type invoke(ClassType&& _instance, Args&& ..._args)
    {
        return (_instance.*s_field)(std::forward<Args>(_args)...);
    }

    template<typename ClassType, typename ...Args>
        requires(std::derived_from<std::remove_pointer_t<ClassType>, class_type>)
    constexpr return_type invoke(ClassType* _instance, Args&& ..._args)
    {
        return (_instance->*s_field)(std::forward<Args>(_args)...);
    }

    [[nodiscard]] constexpr std::string_view get_name() const
    {
        return s_field_name;
    }
};

#endif //REFLECTION_C_FUNCTION_FIELD_H
