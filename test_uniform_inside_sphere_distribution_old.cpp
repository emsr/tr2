// /home/ed/bin/bin/g++ -std=c++1z -o test_uniform_inside_sphere_distribution_old test_uniform_inside_sphere_distribution_old.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_uniform_inside_sphere_distribution_old > sphold.iv

//#include <ext/random>
#include "uniform_inside_sphere_distribution.h"
#include <iostream>
#include <functional>
#include <regex>

const char* header =
R"(
#Inventor V2.1 ascii

Separator {

Coordinate3
{
point [
)";

const char* footer =
R"(
]
}

BaseColor
{
  rgb 1.0 0.0 0.0
}

DrawStyle
{
  style FILLED
  pointSize 2.0
  lineWidth 2.00000
  linePattern 0xffff
}

PointSet
{
  startIndex 0
  numPoints 10000
}

Material
{
  transparency 0.8
}

Complexity
{
  value 1.0
}

Sphere
{
  radius RADIUS
}

}
)";

template<std::size_t Dim>
  void
  insphplot(double radius = 1.0)
  {
    std::default_random_engine re; // the default engine
    __gnu_cxx::uniform_inside_sphere_distribution<Dim> isd(radius);

    auto gen = std::bind(isd, re);

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

    std::regex rgx{R"(RADIUS)"};
    std::cout << std::regex_replace (footer, rgx, std::to_string(radius));
          //const charT* s,
          //const basic_regex<charT,traits>& rgx,
          //const charT* fmt);

    std::cout.precision(precision);
  }

int
main()
{
  __gnu_cxx::uniform_inside_sphere_distribution<3> insph;
  std::cout << "default insph = " << insph << '\n';

  __gnu_cxx::uniform_inside_sphere_distribution<2> incirc;
  std::cout << "default incirc = " << incirc << '\n';

  insphplot<3>(5.0);
  std::cout << '\f';
  insphplot<2>(4000.0);
}

