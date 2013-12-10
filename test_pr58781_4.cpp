// /home/ed/bin/bin/g++ -std=c++11 -o test_pr58781_4 test_pr58781_4.cpp

//#include <cstddef>

int
operator""_s(const char32_t *a, size_t b)
{
  return 0;
}

int
f()
{
  using b = decltype(U"\x8111"_s);
}
