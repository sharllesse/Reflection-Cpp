#ifndef REFLECT_H
#define REFLECT_H

#include <string_view>
#include <tuple>
#include <optional>
#include <algorithm>

struct Bar
{
  int myInt;
  float myFloat;
};

template <size_t Size>
struct CompileString
{
  static constexpr size_t s_size{ Size };

  char m_buffer[Size]{};

  consteval CompileString(const char(&str_)[Size]) noexcept {
    std::copy_n(str_, Size, m_buffer);
  }

  consteval operator const char* () const noexcept {
    return m_buffer;
  }

  template <size_t OtherSize>
  consteval bool
    operator==(const CompileString<OtherSize>& other) const noexcept {
    if constexpr (Size != OtherSize) {
      return false;
    }

    for (size_t i = 0; i < Size; ++i) {
      if (m_buffer[i] != other.m_buffer[i]) {
        return false;
      }
    }

    return true;
  }
};

template <class ClassT>
struct Reflect;

template<class ClassT, typename FieldT, FieldT ClassT::* FieldPtr, CompileString FieldName>
struct Field
{
  static_assert(FieldPtr != nullptr, "FieldPtr cannot be nullptr.");
  static_assert(FieldName != "", "FieldName cannot be empty.");

  constexpr Field() = default;
  constexpr Field(const Field&) = default;
  constexpr Field(Field&&) noexcept = default;

  constexpr ~Field() = default;

  Field& operator=(const Field&) = default;
  Field& operator=(Field&&) = default;

  /// @param class_ This is the class value that contain the value that will be returned.
  /// @return The value contained my the field.
  [[nodiscard]] constexpr const FieldT& Get(const ClassT& class_) const { return class_.*s_fieldPtr; }

  /// @tparam ValueT The type of the new value, it needs to be same as the field.
  /// @param class_ This is the class value that will be modified.
  /// @param value_ The new value of the field.
  template<typename ValueT>
  constexpr void Set(ClassT& class_, ValueT&& value_) { class_.*s_fieldPtr = std::forward<ValueT>(value_); }

  [[nodiscard]] constexpr std::string_view GetName() const { return std::string_view{ s_fieldName.m_buffer }; }

  static consteval auto GetStaticName() { return s_fieldName; }
private:
  static constexpr auto s_fieldPtr{ FieldPtr };
  static constexpr auto s_fieldName{ FieldName };
};

template<>
struct Reflect<Bar>
{
  using Type = Bar;

  [[nodiscard]] static consteval std::string_view GetClassName() { return s_reflectedClassName; }
  [[nodiscard]] static consteval auto GetFields() { return s_fieldArray; }

  template<CompileString Name>
  [[nodiscard]] static consteval auto GetFieldByName()
  {
    return GetFieldByName_impl<Name, 0>();
  }

  template<typename Visitor>
  static constexpr void ForEachFields(Visitor&& visitor_)
  {
    ForEachFields_impl(std::forward<Visitor>(visitor_), std::make_index_sequence<std::tuple_size_v<decltype(s_fieldArray)>>{});
  }

private:
  template<CompileString Name, size_t Index>
  static consteval auto GetFieldByName_impl()
  {
    static_assert(std::tuple_size_v<decltype(s_fieldArray)> > Index, "The field has not been found.");
    static_assert(Index >= 0, "The field has not been found.");

    constexpr auto field = std::get<Index>(s_fieldArray);
    if constexpr (std::decay_t<decltype(field)>::GetStaticName() == Name)
    {
      return field;
    }
    else 
    {
      return GetFieldByName_impl<Name, Index + 1>();
    }
  }

  template<typename Visitor, size_t ...Index>
  static constexpr void ForEachFields_impl(Visitor&& visitor_, std::index_sequence<Index...>)
  {
    (visitor_(std::get<Index>(s_fieldArray)), ...);
  }

  static constexpr std::string_view s_reflectedClassName{ "Bar" };

  static constexpr auto s_fieldArray{ std::make_tuple(Field<Bar, decltype(Type::myFloat), &Type::myFloat, "myFloat">(), Field<Bar, decltype(Type::myInt), &Type::myInt, "myInt">()) };
};

#define REFLECT_BEGIN(ClassType)																  \
template<>																						  \
struct Reflect<ClassType>																		  \
{																								  \
private:																						  \
  using Type = ClassType;																		  \
																								  \
  constexpr static std::string_view s_reflectedClassName{ #ClassType };							  \
																								  \
public:																							  \
  [[nodiscard]] consteval static std::string_view GetClassName() { return s_reflectedClassName; } \
  [[nodiscard]] consteval static auto& GetFields() { return s_fieldArray; }						  \
																								  \
private:																						  \
  constexpr static auto s_fieldArray{ std::make_tuple(											  \

#define REFLECT_FIELD(Variable)									  \
Field<Type, decltype(Type::Variable), &Type::Variable, #Variable>() \

#define REFLECT_END() \
	) };		      \
};				      \

#endif