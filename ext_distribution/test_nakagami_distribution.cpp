// $HOME/bin/bin/g++ -std=c++0x -o test_nakagami_distribution test_nakagami_distribution.cpp

#include <iostream>
#include <functional>
#include <ext/random>
#include "histogram.h"

void
nakagami(double mu, double omega)
{
  std::default_random_engine re; // the default engine
  __gnu_cxx::nakagami_distribution<> nd(mu, omega);

  auto gen = std::bind(nd, re);

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
  std::cout << "nakagami(0.5, 1.0)" << std::endl;
  nakagami(0.5 /* mu */, 1.0 /* omega */);

  std::cout << "nakagami(1.0, 1.0)" << std::endl;
  nakagami(1.0 /* mu */, 1.0 /* omega */);

  std::cout << "nakagami(1.0, 2.0)" << std::endl;
  nakagami(1.0 /* mu */, 2.0 /* omega */);

  std::cout << "nakagami(1.0, 3.0)" << std::endl;
  nakagami(1.0 /* mu */, 3.0 /* omega */);

  std::cout << "nakagami(1.0, 8.0)" << std::endl;
  nakagami(1.0 /* mu */, 8.0 /* omega */);

  std::cout << "nakagami(5.0, 1.0)" << std::endl;
  nakagami(5.0 /* mu */, 1.0 /* omega */);

  std::cout << "nakagami(2.0, 2.0)" << std::endl;
  nakagami(2.0 /* mu */, 2.0 /* omega */);
}
