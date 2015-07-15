// $HOME/bin_var_template_2/bin/g++ -std=c++1z -o utf8_char_literal utf8_char_literal.cpp

// $HOME/bin_var_template_2/bin/g++ -std=c++14 -o utf8_char_literal utf8_char_literal.cpp

// $HOME/bin_var_template_2/bin/g++ -std=c++11 -o utf8_char_literal utf8_char_literal.cpp

// $HOME/bin_var_template_2/bin/g++ -std=gnu++11 -o utf8_char_literal utf8_char_literal.cpp

#include <cassert>
#include <type_traits>

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
  auto okc = std::is_same<decltype(u8c), decltype(c)>::value;
  assert(okc);

  auto u8s = u8"c";
  auto x = u8s[0];

  std::cout << typeid(u8c).name() << std::endl;
  std::cout << typeid(x).name() << std::endl;

  auto ok = std::is_same<decltype(u8c), decltype(x)>::value;
  assert(ok);
}
