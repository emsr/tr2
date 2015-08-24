// /home/ed/bin/bin/g++ -std=gnu++17 -o test_von_mises_fisher_distribution test_von_mises_fisher_distribution.cpp

#include <iostream>
#include <iomanip>
#include <experimental/array>

#include "von_mises_fisher_distribution.h"

int
main()
{
  std::mt19937 re;

  double phi0 = __gnu_cxx::__math_constants<double>::__pi_quarter;
  std::array<double, 2> mu2 = std::experimental::make_array(std::cos(phi0),
							    std::sin(phi0));
  __gnu_cxx::von_mises_fisher_distribution<2, double> vmd2(mu2, 10.0);
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir2 = vmd2(re);
      std::cout << "  (" << std::setw(12) << dir2[0]
		 << ", " << std::setw(12) << dir2[1] << ")\n";
    }

  phi0 = __gnu_cxx::__math_constants<double>::__pi_quarter;
  double theta0 = __gnu_cxx::__math_constants<double>::__pi_half;
  std::array<double, 3> mu3 = std::experimental::make_array(std::cos(phi0) * std::sin(theta0),
							    std::sin(phi0) * std::sin(theta0),
							    std::cos(theta0));
  __gnu_cxx::von_mises_fisher_distribution<3, double> vmd3(mu3, 10.0);
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir3 = vmd3(re);
      std::cout << "  (" << std::setw(12) << dir3[0]
		 << ", " << std::setw(12) << dir3[1]
		 << ", " << std::setw(12) << dir3[2] << ")\n";
    }

  auto mu4 = std::experimental::make_array(0.0, 0.0, 0.0, 1.0);
  __gnu_cxx::von_mises_fisher_distribution<4, double> vmd4(mu4, 10.0);
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir4 = vmd4(re);
      std::cout << "  (" << std::setw(12) << dir4[0]
		 << ", " << std::setw(12) << dir4[1]
		 << ", " << std::setw(12) << dir4[2]
		 << ", " << std::setw(12) << dir4[3] << ")\n";
    }
}
