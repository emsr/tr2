// /home/ed/bin/bin/g++ -std=c++0x -o test_arcsine_distribution test_arcsine_distribution.cpp

#include <iostream>
#include <functional>
#include <ext/random>
#include "histogram.h"

void
arcsineplot(double a, double b)
{
  std::default_random_engine re; // the default engine
  __gnu_cxx::arcsine_distribution<> ad(a, b);
  std::cout << ad << std::endl;

  auto gen = std::bind(ad, re);

  histogram<double> bin(100, a, b);

  const std::size_t per = 1000;

  for (std::size_t i = 0; i < 1000 * per; ++i)
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
  std::cout << "arcsine(-1.0, +1.0)" << std::endl;
  arcsineplot(-1.0, +1.0);

  std::cout << "arcsine(-1.0, +3.0)" << std::endl;
  arcsineplot(-1.0, +3.0);
}
