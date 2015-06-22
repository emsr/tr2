// $HOME/bin/bin/g++ -std=c++11 -o test_pr58781_3 test_pr58781_3.cpp > test_pr58781_3.out 2>&1

#include <cstddef>
#include <type_traits>

struct A {};

A
operator""_s(const char32_t *a, size_t b)
{
  return A{};
}

int
f()
{
  using a = decltype(U"\x1181"_s);
  static_assert(std::is_same<a, A>::value, "a != A");
}

int
main()
{
  return f();
}
