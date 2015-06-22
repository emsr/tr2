// $HOME/bin/bin/g++ -std=c++0x -o test_pareto_distribution test_pareto_distribution.cpp

#include <iostream>
#include <functional>
#include <ext/random>
#include "histogram.h"

void
paretoplot(double alpha, double mu)
{
  std::default_random_engine re; // the default engine
  __gnu_cxx::pareto_distribution<> pd(alpha, mu);
  std::cout << pd << std::endl;

  auto gen = std::bind(pd, re);

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
  std::cout << "pareto(1.0, 1.0)" << std::endl;
  paretoplot(1.0, 1.0);

  std::cout << "pareto(3.0, 2.0)" << std::endl;
  paretoplot(3.0, 2.0);
}
