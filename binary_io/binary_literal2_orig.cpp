// $HOME/bin/bin/g++ -std=c++1y -o binary_literal2_orig binary_literal2_orig.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./binary_literal2_orig

#include <iostream>

constexpr unsigned long long
operator"" _lshift(unsigned long long n)
{ return n << 1; }

int
main()
{
  unsigned long long m = 0b01010101010101010101010101010101_lshift;
  std::cout << std::showbase << std::bin << 0b01010101010101010101010101010101 << '\n';
  std::cout << std::showbase << std::bin << m << '\n';
}
