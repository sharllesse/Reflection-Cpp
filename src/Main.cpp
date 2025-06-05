#include "Pch.h"
#include "Reflect.h"

struct Foo
{
  int myInt;
  int otherInt;
};

REFLECT_BEGIN(Foo)
	REFLECT_FIELD(myInt),
	REFLECT_FIELD(otherInt)
REFLECT_END()

int main(int argc, char* argv[])
{
  

  Foo foo{ 10, 2 };

	auto fields{ Reflect<Foo>::GetFields() };
  auto& field = std::get<0>(fields);

  field.Set(foo, 20); //Work

  std::cout << field.GetName() << '\n'; //Work

  std::apply([&foo](const auto& ...args_)
    {
      ((std::cout << args_.Get(foo) << '\n'), ...); //Vietnam mais work
    }, Reflect<Foo>::GetFields());

  return 0;
}