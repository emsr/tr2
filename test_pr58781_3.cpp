// /home/ed/bin/bin/g++ -std=c++11 -o test_pr58781_3 test_pr58781_3.cpp

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
}

int
main()
{
}
