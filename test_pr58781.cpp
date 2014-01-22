// /home/ed/bin/bin/g++ -std=c++11 -o test_pr58781 test_pr58781.cpp

#include <cmath>
#include <cstddef>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

int
operator""_s(const char32_t *a, size_t b)
{
  std::cout << "len: " << b << '\n';
  std::cout << "str: " << a << '\n';
  for (int i = 0; i < b; ++i)
    std::cout << ' ' << a[i] << '\n';
  return 0;
}

int
operator""_c(char32_t c)
{
  std::cout << "chr: " << c << '\n';
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
  std::cout << std::boolalpha << std::showbase << std::hex;
  std::cout << "\nstring literals\n";
  auto i = U"\x1181"_s;
  auto j = U"\x8111"_s;
  auto k = U" \x1181"_s;
  auto l = U'\x1181'_c;
  auto m = U'\x8111'_c;

  std::cout << "\ndecltype string literals\n";
  using a = decltype(U"\x1181"_s);
  using b = decltype(U"\x8111"_s);
  using c = decltype(U" \x1181"_s);
  using d = decltype(U'\x1181'_c);
  using e = decltype(U'\x8111'_c);
  using sa = decltype(U"\x1181");
  using sb = decltype(U"\x8111");
  using sc = decltype(U" \x1181");
  using ca = decltype(U'\x1181');
  using cb = decltype(U'\x8111');
  std::cout << std::is_same<a, int>::value << '\n';
  std::cout << std::is_same<sa, const char32_t*>::value << '\n';
  std::cout << std::is_same<ca, char32_t>::value << '\n';
  int status;
  size_t length = 1000;
  char buffer[1000] = "";
  const char *realname = abi::__cxa_demangle(typeid(sa).name(), buffer, &length, &status);
  std::cout << "sa is: " << typeid(sa).name();
  if (status == 0)
    std::cout << " = \"" << realname << "\"\n";
  else
  {
    std::cout << '\n';
    std::cerr << "status = " << status << '\n';
  }

  std::cout << "\nfloat literals\n";
  auto q = 3.141592654_q;
  using p = decltype(3.141592654_q);
  std::cout << std::is_same<p, decltype(q)>::value << '\n';
  std::cout << std::is_same<p, int>::value << '\n';

  std::cout << "\nexplicit operator calls\n";
  auto el = operator""_s(U"\x1181", sizeof(U"\x1181")/sizeof(U'\x1181') - 1);
  auto em = operator""_s(U"\x8111", sizeof(U"\x8111")/sizeof(U'\x8111') - 1);
  auto en = operator""_s(U" \x1181", sizeof(U" \x1181")/sizeof(U' ') - 1);

  //  These work!!!
  std::cout << "\ndecltype explicit operator calls\n";
  using x = decltype(operator""_s(U"\x1181", sizeof(U"\x1181")/sizeof(U'\x1181') - 1));
  using y = decltype(operator""_s(U"\x8111", sizeof(U"\x8111")/sizeof(U'\x8111') - 1));
  using z = decltype(operator""_s(U" \x1181", sizeof(U" \x1181")/sizeof(U' ') - 1));
  std::cout << std::is_same<x, decltype(l)>::value << '\n';
  std::cout << std::is_same<x, int>::value << '\n';
}

int
main()
{
  f();
}
