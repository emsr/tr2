// /home/ed/bin/bin/g++ -std=c++1y -o binary_literal binary_literal.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./binary_literal

#include <iostream>

constexpr unsigned long long
operator"" _lshift(unsigned long long n)
{ return n << 1; }

int
main()
{
  unsigned long long m = 0b01010101010101010101010101010101_lshift;
  std::cout << std::showbase << __gnu_cxx::__bin << 0b01010101010101010101010101010101 << '\n';
  std::cout << std::showbase << __gnu_cxx::__bin << m << '\n';
}
