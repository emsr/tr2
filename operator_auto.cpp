// /home/ed/bin/bin/g++ -std=c++14 -o operator_auto operator_auto.cpp

#include <cassert>

struct A
{
  operator auto();
};

template<typename Tp>
  struct B
  {
    Tp t;

    operator auto()
    { return t + 1; }
  };

A::operator auto()
{ return 42; }

int
main()
{
  auto a = A{};
  assert(a == 42);

  int b = B<int>{42};
  assert(b == 43);

  return (a != 42) | (b != 43);
}
