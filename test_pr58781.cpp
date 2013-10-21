// /home/ed/bin/bin/g++ -std=c++11 -o test_pr58781 test_pr58781.cpp

#include <cmath>
#include <cstddef>
#include <iostream>

int
operator""_s(const char32_t *a, size_t b)
{
  std::cout << b << '\n';
  std::cout << a << '\n';
  for (int i = 0; i < b; ++i)
    std::cout << ' ' << std::showbase << std::hex << a[i] << '\n';
  return 0;
}

int
operator""_q(long double x)
{
  double dx = static_cast<double>(x), delta = 0.001;
  double q = std::copysign(1.0, dx)
           * delta * std::floor(std::abs(dx) / delta + 0.5);
  std::cout << q << '\n';
  return q;
}

int
f()
{
  std::cout << '\n';
  auto i = U"\x1181"_s;
  auto j = U"\x8111"_s;
  auto k = U" \x1181"_s;

  using a = decltype(U"\x1181"_s);
  //using b = decltype(U"\x8111"_s);
  //using c = decltype(U" \x1181"_s);
  std::cout << '\n';
  std::cout << std::boolalpha << std::is_same<a, int>::value << '\n';

  std::cout << '\n';
  auto q = 3.141592654_q;
  using p = decltype(3.141592654_q);
  std::cout << std::boolalpha << std::is_same<p, decltype(q)>::value << '\n';
  std::cout << std::boolalpha << std::is_same<p, int>::value << '\n';

  std::cout << '\n';
  auto l = operator""_s(U"\x1181", sizeof(U"\x1181")/sizeof(U'\x1181') - 1);
  auto m = operator""_s(U"\x8111", sizeof(U"\x8111")/sizeof(U'\x8111') - 1);
  auto n = operator""_s(U" \x1181", sizeof(U" \x1181")/sizeof(U' ') - 1);

  //  These work!!!
  using x = decltype(operator""_s(U"\x1181", sizeof(U"\x1181")/sizeof(U'\x1181') - 1));
  using y = decltype(operator""_s(U"\x8111", sizeof(U"\x8111")/sizeof(U'\x8111') - 1));
  using z = decltype(operator""_s(U" \x1181", sizeof(U" \x1181")/sizeof(U' ') - 1));

  std::cout << '\n';
  std::cout << std::boolalpha << std::is_same<x, decltype(l)>::value << '\n';
  std::cout << std::boolalpha << std::is_same<x, int>::value << '\n';
}

int
main()
{
  f();
}
