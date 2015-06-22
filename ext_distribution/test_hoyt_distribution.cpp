// $HOME/bin/bin/g++ -std=c++0x -o test_hoyt_distribution test_hoyt_distribution.cpp

#include "hoyt_distribution.h"
#include <iostream>
#include <functional>
#include <random>
#include "histogram.h"

void
hoytplot(double q, double omega)
{
  std::default_random_engine re; // the default engine
  __gnu_cxx::hoyt_distribution<> hd(q, omega);
  std::cout << hd << std::endl;

  auto gen = std::bind(hd, re);

  histogram<double> bin(100, 0.0, 3.0);

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
  std::cout << "hoyt(0.05, 2.0)" << std::endl;
  hoytplot(0.05, 2.0);

  std::cout << "hoyt(0.3, 2.0)" << std::endl;
  hoytplot(0.3, 2.0);

  std::cout << "hoyt(0.9, 2.0)" << std::endl;
  hoytplot(0.9, 2.0);

  std::cout << "hoyt(0.2, 0.5)" << std::endl;
  hoytplot(0.2, 0.5);

  std::cout << "hoyt(0.2, 2.0)" << std::endl;
  hoytplot(0.2, 2.0);

  std::cout << "hoyt(0.2, 5.0)" << std::endl;
  hoytplot(0.2, 5.0);
}
