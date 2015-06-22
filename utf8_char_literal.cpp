// $HOME/bin_var_template_2/bin/g++ -std=c++1z -o utf8_char_literal utf8_char_literal.cpp

#include <cassert>
#include <experimental/type_traits>

#include <iostream>
#include <typeinfo>

constexpr int
operator""_foo(char c)
{ return c * 100; }

int
main()
{
  auto cc = u8'8'_foo;

  auto c = 'c';
  auto u8c = u8'c';
  auto okc = std::experimental::is_same_v<decltype(u8c), decltype(c)>;
  assert(okc);

  auto u8s = u8"c";
  auto x = u8s[0];

  std::cout << typeid(u8c).name() << std::endl;
  std::cout << typeid(x).name() << std::endl;

  auto ok = std::experimental::is_same_v<decltype(u8c), decltype(x)>;
  assert(ok);
}
