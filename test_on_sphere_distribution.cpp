// ../bin/bin/g++ -std=c++1z -o test_on_sphere_distribution test_on_sphere_distribution.cpp

#include <ext/random>
#include <iostream>
#include <functional>
#include "histogram.h"

const char* header =
R"(
#Inventor V2.1 ascii

Separator {

Coordinate3 {
point [
)";

const char* footer =
R"(
]
}

BaseColor {
  rgb 1.0 0.0 0.0
}

DrawStyle {
  style  FILLED
  pointSize  2.0
  lineWidth  2.00000
  linePattern  0xffff
}

PointSet
{
  startIndex 0
  numPoints 10000
}

}
)";

template<std::size_t Dim>
  void
  onsphplot()
  {
    std::default_random_engine re; // the default engine
    __gnu_cxx::uniform_on_sphere_distribution<Dim> osd;

    auto gen = std::bind(osd, re);

    auto precision = std::cout.precision();
    std::cout.precision(std::numeric_limits<double>::max_digits10);

    std::cout << header;

    const std::size_t per = 10000;
    for (std::size_t i = 0; i < per; ++i)
    {
      auto pt = gen();
      for (coord : pt)
        std::cout << ' ' << coord;
      std::cout << ',' << '\n';
    }

    std::cout << footer;

    std::cout.precision(precision);
  }

int
main()
{
  __gnu_cxx::uniform_on_sphere_distribution<3> onsph;
  std::cout << "default onsph = " << onsph << '\n';

  onsphplot<3>();
}

