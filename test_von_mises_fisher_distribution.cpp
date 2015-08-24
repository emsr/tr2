// /home/ed/bin/bin/g++ -std=gnu++17 -o test_von_mises_fisher_distribution test_von_mises_fisher_distribution.cpp

#include <iostream>
#include <iomanip>
#include <experimental/array>

#include "von_mises_fisher_distribution.h"

template<typename Tp, size_t Dim>
  std::array<Tp, Dim>
  operator+(const std::array<Tp, Dim>& a, const std::array<Tp, Dim>& b)
  {
    std::array<Tp, Dim> c(a);
    for (int i = 0; i < Dim; ++i)
      c[i] += b[i];
    return c;
  }

template<typename Tp, size_t Dim, typename Up>
  std::array<Tp, Dim>
  operator/(const std::array<Tp, Dim>& a, Up b)
  {
    std::array<Tp, Dim> c(a);
    for (int i = 0; i < Dim; ++i)
      c[i] /= b;
    return c;
  }

int
main()
{
  std::mt19937 re;

  double phi0 = __gnu_cxx::__math_constants<double>::__pi_quarter;
  std::array<double, 2> mu2 = std::experimental::make_array(std::cos(phi0),
							    std::sin(phi0));
  __gnu_cxx::von_mises_fisher_distribution<2, double> vmd2(mu2, 10.0);
  std::array<double, 2> mean2;
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir2 = vmd2(re);
      mean2 = mean2 + dir2;
      std::cout << "  (" << std::setw(12) << dir2[0]
		 << ", " << std::setw(12) << dir2[1] << ")\n";
    }
  mean2 = mean2 / 1000;
  std::cout << "  mean2 = (" << std::setw(12) << mean2[0]
	     << ", " << std::setw(12) << mean2[1] << ")\n";
  std::cout << "  vmd2 = " << vmd2 << '\n';


  phi0 = __gnu_cxx::__math_constants<double>::__pi_quarter;
  double theta0 = __gnu_cxx::__math_constants<double>::__pi_third;
  std::array<double, 3> mu3 = std::experimental::make_array(std::cos(phi0) * std::sin(theta0),
							    std::sin(phi0) * std::sin(theta0),
							    std::cos(theta0));
  __gnu_cxx::von_mises_fisher_distribution<3, double> vmd3(mu3, 10.0);
  std::array<double, 3> mean3;
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir3 = vmd3(re);
      mean3 = mean3 + dir3;
      std::cout << "  (" << std::setw(12) << dir3[0]
		 << ", " << std::setw(12) << dir3[1]
		 << ", " << std::setw(12) << dir3[2] << ")\n";
    }
  mean3 = mean3 / 1000;
  std::cout << "  mean3 = ("
                    << std::setw(12) << mean3[0]
	    << ", " << std::setw(12) << mean3[1]
	    << ", " << std::setw(12) << mean3[2] << ")\n";
  std::cout << "  vmd3 = " << vmd3 << '\n';


  auto mu4 = std::experimental::make_array(0.0, 0.0, 0.0, 1.0);
  __gnu_cxx::von_mises_fisher_distribution<4, double> vmd4(mu4, 10.0);
  std::array<double, 4> mean4;
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir4 = vmd4(re);
      mean4 = mean4 + dir4;
      std::cout << "  (" << std::setw(12) << dir4[0]
		 << ", " << std::setw(12) << dir4[1]
		 << ", " << std::setw(12) << dir4[2]
		 << ", " << std::setw(12) << dir4[3] << ")\n";
    }
  mean4 = mean4 / 1000;
  std::cout << "  mean4 = ("
                    << std::setw(12) << mean4[0]
	    << ", " << std::setw(12) << mean4[1]
	    << ", " << std::setw(12) << mean4[2]
	    << ", " << std::setw(12) << mean4[3] << ")\n";
  std::cout << "  vmd4 = " << vmd4 << '\n';
}
