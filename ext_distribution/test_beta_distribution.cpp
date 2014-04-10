// /home/ed/bin/bin/g++ -std=c++0x -o test_beta_distribution test_beta_distribution.cpp

#include <cassert>
#include <ext/random>
#include <sstream>
#include <iostream>
#include <functional>
#include "histogram.h"

#define VERIFY assert

void
beta(double alpha, double beta)
{
  std::default_random_engine re; // the default engine
  __gnu_cxx::beta_distribution<> bd(alpha, beta);

  auto gen = std::bind(bd, re);

  histogram<double> bin(100, 0.0, 1.0);

  const std::size_t per = 1000;

  for (std::size_t i = 0; i < 5000 * per; ++i)
    bin << gen();

  std::cout << "Mean: " << bin.mean() << std::endl;

  for (std::size_t i = 1; i <= bin.size(); ++i)
  {
    std::cout << bin.value(i) << '\t';
    for (std::size_t j = 0; j < bin[i]; j += per)
      std::cout << '*';
    std::cout << std::endl;
  }
}

int
main()
{
  __gnu_cxx::beta_distribution<> u;
  VERIFY( u.alpha() == 1.0 );
  VERIFY( u.beta() == 1.0 );
  VERIFY( u.min() == 0.0 );
  VERIFY( u.max() == 1.0 );
  std::cout << u << std::endl;

  __gnu_cxx::beta_distribution<> v(1.5, 3.0);
  VERIFY( v.alpha() == 1.5 );
  VERIFY( v.beta() == 3.0 );
  VERIFY( v.min() == 0.0 );
  VERIFY( v.max() == 1.0 );
  std::cout << v << std::endl;

  std::stringstream str;
  __gnu_cxx::beta_distribution<double> x(1.5, 3.0), y;
  std::minstd_rand0 rng;
  std::cout << x << std::endl;
  std::cout << y << std::endl;

  x(rng); // advance
  str << x;

  str >> y;
  std::cout << x << std::endl;
  std::cout << y << std::endl;

  typedef __gnu_cxx::beta_distribution<double> test_type;

  typedef test_type::result_type result_type;

  beta(0.5, 0.5);

  beta(2.0, 2.0);

  beta(1.5, 3.0);

  return 0;
}
