// ../bin/bin/g++ -std=c++1z -o test_on_sphere_distribution test_on_sphere_distribution.cpp

#include <ext/random>
#include <iostream>
#include <functional>
#include "histogram.h"

template<std::size_t Dim>
  void
  onsphplot()
  {
    std::default_random_engine re; // the default engine
    __gnu_cxx::uniform_on_sphere_distribution<Dim> osd;

    auto gen = std::bind(osd, re);

    const std::size_t per = 1000;
    for (std::size_t i = 0; i < per; ++i)
    {
      auto pt = gen();
      for (coord : pt)
        std::cout << ' ' << coord;
      std::cout << '\n';
    }
  }

int
main()
{
  __gnu_cxx::uniform_on_sphere_distribution<3> onsph;
  std::cout << "default hyper = " << onsph << '\n';

  onsphplot<3>();
}

