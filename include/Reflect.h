#ifndef REFLECT_H
#define REFLECT_H

#include <string_view>
#include <tuple>
#include <optional>

struct Bar
{
  int myInt;
};

template<class ClassT>
struct Reflect;

//Besoin de check si le fieldPtr_ n'est pas nullptr. a la compilation.
//Changer tout les parametres par des template non-typer.

template<class ClassT, typename FieldT, FieldT ClassT::* FieldPtr>
struct Field
{
  static_assert(FieldPtr != nullptr, "FieldPtr cannot be nullptr.");

private:
  constexpr Field() = default;

public:
  constexpr Field(const Field&) = default;
  constexpr Field(Field&&) noexcept = default;

  constexpr explicit Field(std::string_view fieldName_)
    : m_fieldName(fieldName_)
  {
  }

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

  [[nodiscard]] constexpr std::string_view GetName() const { return m_fieldName; }
private:
  static constexpr auto s_fieldPtr{ FieldPtr };

  std::string_view m_fieldName;
};

template<>
struct Reflect<Bar>
{
private:
  using Type = Bar;

  constexpr static std::string_view s_reflectedClassName{ "Bar" };

  constexpr static auto s_fieldArray{ std::make_tuple(Field<Bar, decltype(Type::myInt), &Type::myInt>("myInt")) };
public:
  [[nodiscard]] consteval static std::string_view GetClassName() { return s_reflectedClassName; }
  [[nodiscard]] consteval static auto& GetFields() { return s_fieldArray; }
  [[nodiscard]] consteval static auto& GetFieldByName(std::string_view name_)
  {
    return GetFieldByName_impl<0>(name_);
  }

private:
  template<size_t Index>
  consteval static auto& GetFieldByName_impl(std::string_view name_)
  {
  	static_assert(std::tuple_size_v<decltype(s_fieldArray)> > Index, "The field has not been found.");

    constexpr auto& field = std::get<Index>(s_fieldArray);
    if constexpr (field.GetName() == name_)
    {
      return field;
    }

    return GetFieldByName_impl<Index + 1>(name_);
  }
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
Field<Type, decltype(Type::Variable), &Type::Variable>(#Variable) \

#define REFLECT_END() \
	) };		      \
};				      \

#endif