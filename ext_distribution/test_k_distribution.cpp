// /home/ed/bin/bin/g++ -std=c++0x -o test_k_distribution test_k_distribution.cpp

#include <iostream>
#include <functional>
#include <ext/random>
#include "histogram.h"

void
kplot(double lambda, double mu, double nu)
{
  std::default_random_engine re; // the default engine
  __gnu_cxx::k_distribution<> kd(lambda, mu, nu);
  std::cout << kd << std::endl;

  auto gen = std::bind(kd, re);

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
  std::cout << "k(1.0, 1.0, 1.0)" << std::endl;
  kplot(1.0 /* lambda */, 1.0 /* mu */, 1.0 /* nu */);

  std::cout << "k(1.0, 1.0, 2.0)" << std::endl;
  kplot(1.0 /* lambda */, 1.0 /* mu */, 2.0 /* nu */);

  std::cout << "k(1.0, 0.5, 2.0)" << std::endl;
  kplot(1.0 /* lambda */, 0.5 /* mu */, 2.0 /* nu */);

  std::cout << "k(1.0, 2.0, 2.0)" << std::endl;
  kplot(1.0 /* lambda */, 2.0 /* mu */, 2.0 /* nu */);

  std::cout << "k(1.0, 5.0, 2.0)" << std::endl;
  kplot(1.0 /* lambda */, 5.0 /* mu */, 2.0 /* nu */);

  std::cout << "k(1.0, 1.0, 0.5)" << std::endl;
  kplot(1.0 /* lambda */, 1.0 /* mu */, 0.5 /* nu */);

  std::cout << "k(1.0, 1.0, 1.0)" << std::endl;
  kplot(1.0 /* lambda */, 1.0 /* mu */, 1.0 /* nu */);

  std::cout << "k(1.0, 1.0, 15.0)" << std::endl;
  kplot(1.0 /* lambda */, 1.0 /* mu */, 15.0 /* nu */);
}
