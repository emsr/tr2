
// $HOME/bin_git/bin/g++ -std=c++1z -pedantic -DVOMIT -o fold_expression fold_expression.cpp

#include <iostream>

template<typename... Args>
  auto
  sum(Args... args)
  { return (... + args); }

template<typename... Args>
  auto
  product(Args... args)
  { return (args * ...); }

template<typename... Args>
  bool
  some(Args... args)
  { return (... || args); }

template<typename... Args>
  bool
  all(Args... args)
  { return (args && ...); }

template<typename... Args>
  bool
  f(Args... args)
  { 
    return (true + ... + args); // OK
  }

bool b = all(true, true, true, false);

bool a = some(true, true, true, false);

auto c = sum(1, 2, 3, 4, 5);

auto d = sum(1, 2, 3, 4, 5.0);

auto p = product(1, 2, 3, 4, 5.0);

int
main()
{
  std::cout << "a = " << std::boolalpha << a << '\n';
  std::cout << "b = " << std::boolalpha << b << '\n';
  std::cout << "c = " << c << '\n';
  std::cout << "d = " << d << '\n';
  std::cout << "p = " << p << '\n';
}
