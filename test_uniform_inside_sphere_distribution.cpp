// /home/ed/bin/bin/g++ -std=c++1z -o test_uniform_inside_sphere_distribution test_uniform_inside_sphere_distribution.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_uniform_inside_sphere_distribution

//#include <ext/random>
#include "uniform_inside_sphere_distribution.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <regex>

const char* header =
R"(#Inventor V2.1 ascii

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
  insphplot(std::ostream & iv, double radius = 1.0)
  {
    std::default_random_engine re; // the default engine
    __gnu_cxx::uniform_inside_sphere_distribution<Dim> isd(radius);

    auto gen = std::bind(isd, re);

    auto precision = iv.precision();
    iv.precision(std::numeric_limits<double>::max_digits10);

    iv << header;

    const std::size_t per = 10000;
    for (std::size_t i = 0; i < per; ++i)
    {
      auto pt = gen();
      for (auto && coord : pt)
      {
        iv << ' ' << coord;
      }
      if (Dim == 2)
        iv << " 0.0,\n";
      else
        iv << ',' << '\n';
    }

    std::regex rgx{R"(RADIUS)"};
    iv << std::regex_replace (footer, rgx, std::to_string(radius));
          //const charT* s,
          //const basic_regex<charT,traits>& rgx,
          //const charT* fmt);

    iv.precision(precision);
  }

int
main()
{
  __gnu_cxx::uniform_inside_sphere_distribution<3> insph;
  std::cout << "default insph = " << insph << '\n';

  __gnu_cxx::uniform_inside_sphere_distribution<2> incirc;
  std::cout << "default incirc = " << incirc << '\n';

  std::ofstream sph("sph.iv");
  insphplot<3>(sph, 5.0);

  std::ofstream circ("circ.iv");
  insphplot<2>(circ, 4000.0);
}

