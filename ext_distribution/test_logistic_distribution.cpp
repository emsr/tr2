// $HOME/bin/bin/g++ -std=c++11 -o test_logistic_distribution test_logistic_distribution.cpp

#include "logistic_distribution.h"
#include <iostream>
#include <functional>
#include <ext/random>
#include "histogram.h"

void
logisticplot(double a, double b)
{
  std::default_random_engine re; // the default engine
  std::logistic_distribution<> rd(a, b);

  auto gen = std::bind(rd, re);

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
  std::cout << "logistic(4.0, 1.0)\n";
  logisticplot(4.0 /* mean */, 1.0 /* sigma */);

  std::cout << "logistic(2.0, 1.0)\n";
  logisticplot(2.0 /* mean */, 1.0 /* sigma */);

  std::cout << "logistic(0.0, 1.0)\n";
  logisticplot(0.0 /* mean */, 1.0 /* sigma */);

  std::cout << "logistic(0.0, 2.0)\n";
  logisticplot(0.0 /* mean */, 2.0 /* sigma */);

  std::cout << "logistic(2.0, 2.0)\n";
  logisticplot(2.0 /* mean */, 2.0 /* sigma */);

  std::cout << "logistic(2.0, 4.0)\n";
  logisticplot(2.0 /* mean */, 4.0 /* sigma */);

  std::cout << "logistic(5.0, 0.2)\n";
  logisticplot(5.0 /* mean */, 0.2 /* sigma */);
}
