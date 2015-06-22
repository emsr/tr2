// $HOME/bin/bin/g++ -std=c++14 -o test_variant test_variant.cpp

#include "variant"
#include <string>
#include <complex>
#include <iostream>

int
main()
{
  auto s = std::max({sizeof(std::size_t),
                     sizeof(std::string),
                     sizeof(std::complex<double>)});
  std::cout << "max sizeof: " << s << '\n';
  std::cout << "sizeof(std::size_t): " << sizeof(std::size_t) << '\n';
  std::cout << "sizeof(std::string): " << sizeof(std::string) << '\n';
  std::cout << "sizeof(std::complex<double>): " << sizeof(std::complex<double>) << '\n';

  variant<std::size_t, std::string, std::complex<double>> v;
  std::cout << "max sizeof: " << sizeof(v) << '\n';
}
