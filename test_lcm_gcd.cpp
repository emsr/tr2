
// $HOME/bin/bin/g++ -std=c++14 -o test_lcm_gcd test_lcm_gcd.cpp

#include <iostream>

#include "numeric"

int
main()
{
  using namespace std::experimental;

  std::cout << "gcd(234,123) = " << gcd(234ULL, 123ULL) << '\n';
  std::cout << "gcd(112,84) = " << gcd(112ULL, 84ULL) << '\n';
  std::cout << "gcd(12457375,1234166) = " << gcd(12457375ULL,1234166ULL) << '\n';

  std::cout << "lcm(234,123) = " << lcm(234ULL, 123ULL) << '\n';
  std::cout << "lcm(112,84) = " << lcm(112ULL, 84ULL) << '\n';
  std::cout << "lcm(12457375,1234166) = " << lcm(12457375ULL, 1234166ULL) << '\n';
}
