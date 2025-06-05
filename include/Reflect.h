#ifndef REFLECT_H
#define REFLECT_H

#include <string_view>
#include <tuple>

struct Bar
{
  int myInt;
};

template<class ClassT>
struct Reflect
{
private:
  Reflect() = default;

public:
  constexpr static std::string_view GetClassName();
  constexpr static auto& GetFields();

  ~Reflect() = default;
};

//template<class ClassT>
//struct BaseField
//{
//public:
//  constexpr BaseField(std::string_view fieldName_)
//	  : m_fieldName(fieldName_)
//  {}
//
//protected:
//  std::string_view m_fieldName;
//};

  //Besoin de check si le fieldPtr_ n'est pas nullptr. a la compilation.
//Changer tout les parametres par des template non-typer.

template<class ClassT, typename FieldT>
struct Field
{
private:
  constexpr Field() = default;

public:
  constexpr Field(const Field&) = default;
  constexpr Field(Field&&) noexcept = default;

  constexpr Field(FieldT ClassT::* fieldPtr_, std::string_view fieldName_)
    : m_fieldPtr(fieldPtr_), m_fieldName(fieldName_)
  {
    //static_assert(std::is_null_pointer_v<decltype(m_fieldPtr)>, "Field pointer cannot be nullptr.");
  }

  constexpr ~Field() = default;

  Field& operator=(const Field&) = default;
  Field& operator=(Field&&) = default;

  /// @param class_ This is the class value that contain the value that will be returned.
  /// @return The value contained my the field.
  [[nodiscard]] constexpr const FieldT& GetValue(const ClassT& class_) const { return class_.*m_fieldPtr; }

  /// @tparam ValueT The type of the new value, it needs to be same as the field.
  /// @param class_ This is the class value that will be modified.
  /// @param value_ The new value of the field.
  template<typename ValueT>
  constexpr void SetValue(ClassT& class_, ValueT&& value_) { class_.*m_fieldPtr = std::forward<ValueT>(value_); }

  [[nodiscard]] constexpr std::string_view GetName() const { return m_fieldName; }
private:
  FieldT ClassT::* m_fieldPtr{ nullptr };

  std::string_view m_fieldName;
};

//template<>
//struct Reflect<Bar>
//{
//private:
//  constexpr static std::string_view s_reflectedClassName{ "Bar" };
//
//public:
//  constexpr static std::string_view GetClassName() { return s_reflectedClassName; }
//
//private:
//  constexpr static auto s_fieldArray{ std::make_tuple(Field<Bar, int>(&Bar::myInt)) };
//};

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
  [[nodiscard]] constexpr static std::string_view GetClassName() { return s_reflectedClassName; } \
  [[nodiscard]] constexpr static auto& GetFields() { return s_fieldArray; }						  \
																								  \
private:																						  \
  constexpr static auto s_fieldArray{ std::make_tuple(											  \

#define REFLECT_FIELD(Variable)									  \
Field<Type, decltype(Type::Variable)>(&Type::Variable, #Variable) \

#define REFLECT_END() \
	) };		      \
};				      \

#endif