// /home/ed/bin/bin/g++ -std=c++0x -o test_gamma_distribution test_gamma_distribution.cpp

#include <iostream>
#include <functional>
#include <random>
#include "histogram.h"

void
gammaplot(double alpha, double beta)
{
  std::default_random_engine re; // the default engine
  std::gamma_distribution<> gd(alpha, beta);
  std::cout << gd << std::endl;

  auto gen = std::bind(gd, re);

  histogram<double> bin(100, 0.0, 10.0);

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
  std::cout << "Gamma(1, 1)" << std::endl;
  gammaplot(/* alpha */ 1.0, /* beta */ 1.0);

  std::cout << "Gamma(2, 1/4)" << std::endl;
  gammaplot(/* alpha */ 2.0, /* beta */ 0.25);

  std::cout << "Gamma(15, 1/15)" << std::endl;
  gammaplot(/* alpha */ 15.0, /* beta */ 1.0 / 15.0);
}
