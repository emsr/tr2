// $HOME/bin/bin/g++ -std=c++11 -o test_pr58781_x test_pr58781_x.cpp

#include <cstddef>

int
operator""_s(const char32_t *a, size_t b)
{
  return 0;
}

int
f()
{
  using sa = decltype(U"\x8111");
  using oa = decltype(U"\x8111"_s);
}
