// /home/ed/bin/bin/g++ -std=c++0x -o test_rice_distribution_multi test_rice_distribution_multi.cpp

#include "rice_distribution_multi.h"
#include <iostream>
#include <functional>
#include <ext/random>
#include "histogram.h"

void
riceplot(double mean, double sigma)
{
  std::default_random_engine re; // the default engine
  __gnu_cxx::rice_distribution<> rd(mean, sigma);

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

void
multiriceplot(const std::vector<double> mean, double sigma)
{
  using mrdist = rice_distribution_multi<>;
  using mrparam = mrdist::param_type;

  std::default_random_engine re; // the default engine
  mrdist mrd{mrparam{std::begin(mean), std::end(mean), sigma}};

  auto gen = std::bind(mrd, re);

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
  const std::vector<double> mean{1.0, 3.0, 4.5, 4.75, 5.5, 6.0};
  const double sigma{6.0};
  const double m = mean.size() / 2.0;
  double s = 0.0;
  for (std::size_t k = 0; k < mean.size(); ++k)
    s += mean[k] * mean[k];
  s = std::sqrt(s);
  const double a = s / sigma;

  rice_density<> rice(mean, sigma);
  std::cout << "rice = " << rice << std::endl;

  const double del = 0.1;
  const unsigned k = 100;
  double cpd = 0.0;
  for (int i = 0; i < 1000 * k; ++i)
    {
      double x = del * i / k;
      double y = 0.5 * (rice(x) + rice(x + del / k));
      cpd += del * y / k;
      if (i % k == 0)
        {
	  std::cout << "rice(" << x << ") = " << y;
	  std::cout << "    cpd(" << x << ") = " << cpd;
	  double b = x / sigma;
	  std::cout << "    1 - Q_5("") = " << 1.0 - marcum_q(m, a, b);
	  std::cout << std::endl;
        }
    }

  std::cout << "rice(4.0, 1.0)" << std::endl;
  riceplot(4.0 /* mean */, 1.0 /* sigma */);

  std::cout << "rice(2.0, 1.0)" << std::endl;
  riceplot(2.0 /* mean */, 1.0 /* sigma */);

  std::cout << "rice(0.0, 1.0)" << std::endl;
  riceplot(0.0 /* mean */, 1.0 /* sigma */);

  std::cout << "rice(0.0, 2.0)" << std::endl;
  riceplot(0.0 /* mean */, 2.0 /* sigma */);

  std::cout << "rice(2.0, 2.0)" << std::endl;
  riceplot(2.0 /* mean */, 2.0 /* sigma */);

  std::cout << "rice(2.0, 4.0)" << std::endl;
  riceplot(2.0 /* mean */, 4.0 /* sigma */);

  using mrdist = rice_distribution_multi<>;
  using mrparam = mrdist::param_type;

  mrdist mrice{mrparam{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 1.0}};

  multiriceplot({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 1.0);

  multiriceplot({0.0, 0.0}, 1.0);
}
