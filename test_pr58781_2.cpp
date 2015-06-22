// $HOME/bin/bin/g++ -std=c++11 -o test_pr58781_2 test_pr58781_2.cpp > test_pr58781_2.out 2>&1

#include <cstddef>

int
operator""_s(const char32_t *a, size_t b)
{
  return 0;
}

int
f()
{
  using a = decltype(U"\x1181"_s);
  using b = decltype(U"\x8111"_s);
  using c = decltype(U" \x1181"_s);
}

int
main()
{
  f();
}
