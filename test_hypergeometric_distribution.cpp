// ../bin/bin/g++ -std=c++11 -o test_hypergeometric_distribution test_hypergeometric_distribution.cpp

//#include "hypergeometric"
#include <iostream>
#include <functional>
#include <ext/random>
#include "histogram.h"

void
hyperplot(unsigned int N, unsigned int K, unsigned int n)
{
  std::default_random_engine re; // the default engine
  __gnu_cxx::hypergeometric_distribution<> hd(N, K, n);

  auto gen = std::bind(hd, re);

  auto num = hd.max() - hd.min();
  histogram<double> bin(num, hd.min(), hd.max() + 1);

  const std::size_t per = 1000;

  for (std::size_t i = 0; i < 1000 * per; ++i)
    std::cout << gen() << '\n';

  for (std::size_t i = 0; i < 200 * per; ++i)
    bin << gen() + 0.5;

  for (std::size_t i = 1; i <= bin.size(); ++i)
  {
    std::cout << bin.value(i) << '\t';
    for (std::size_t j = 0; j < bin[i]; j += per)
      std::cout << '*';
    std::cout << '\n';
  }
}

int
main()
{
  __gnu_cxx::hypergeometric_distribution<> hyper{};
  std::cout << "default hyper = " << hyper << '\n';

  __gnu_cxx::hypergeometric_distribution<> hyper2(52, 13, 5);
  std::cout << "hyper(52, 13, 5) = " << hyper2 << '\n';

  std::cout << "hyper(52, 13, 5)\n";
  hyperplot(52, 13, 5);

  std::cout << "hyper(20, 10, 12)\n";
  hyperplot(20, 10, 12);

  std::cout << "hyper(10, 9, 8)\n";
  hyperplot(10, 9, 8);

  std::cout << "hyper(500, 20, 6)\n";
  hyperplot(500, 20, 6);

  std::cout << "hyper(500, 200, 40)\n";
  hyperplot(500, 200, 40);
}
