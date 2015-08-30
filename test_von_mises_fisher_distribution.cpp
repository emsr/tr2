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

template<typename Tp, size_t Dim>
  std::array<Tp, Dim>&
  operator+=(std::array<Tp, Dim>& a, const std::array<Tp, Dim>& b)
  {
    for (int i = 0; i < Dim; ++i)
      a[i] += b[i];
    return a;
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

template<typename Tp, size_t Dim, typename Up>
  std::array<Tp, Dim>&
  operator/=(std::array<Tp, Dim>& a, Up b)
  {
    for (int i = 0; i < Dim; ++i)
      a[i] /= b;
    return a;
  }

template<typename Tp, size_t Dim>
  Tp
  norm(const std::array<Tp, Dim>& a)
  {
    Tp n{};
    for (int i = 0; i < Dim; ++i)
      n += a[i] * a[i];
    return std::sqrt(n);
  }

template<typename Tp, size_t Dim>
  std::array<Tp, Dim>
  normal(const std::array<Tp, Dim>& a)
  {
    std::array<Tp, Dim> c(a);
    auto n = norm(c);
    for (int i = 0; i < Dim; ++i)
      c[i] /= n;
    return c;
  }

template<typename Tp, size_t Dim>
  std::array<Tp, Dim>&
  normalize(std::array<Tp, Dim>& a)
  {
    auto n = norm(a);
    for (int i = 0; i < Dim; ++i)
      a[i] /= n;
    return a;
  }

int
main()
{
  std::mt19937 re;

  double phi0 = __gnu_cxx::__math_constants<double>::__pi_quarter;
  std::array<double, 2> mu2 = std::experimental::make_array(std::cos(phi0),
							    std::sin(phi0));
  __gnu_cxx::von_mises_fisher_distribution<2, double> vmd2(mu2, 100.0);
  std::array<double, 2> mean2;
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir2 = vmd2(re);
      mean2 += dir2;
      std::cout << "  (" << std::setw(12) << dir2[0]
		 << ", " << std::setw(12) << dir2[1]
                << ")  " << norm(dir2) << "\n";
    }
  mean2 /= 1000;
  std::cout << "  mean2 = (" << std::setw(12) << mean2[0]
	     << ", " << std::setw(12) << mean2[1] << ")\n";
  mean2 = normal(mean2);
  std::cout << "  mean2 = (" << std::setw(12) << mean2[0]
	     << ", " << std::setw(12) << mean2[1] << ")\n";
  std::cout << "  vmd2 = " << vmd2 << '\n';


  phi0 = __gnu_cxx::__math_constants<double>::__pi_quarter;
  double theta0 = __gnu_cxx::__math_constants<double>::__pi_third;
  std::array<double, 3> mu3 = std::experimental::make_array(std::cos(phi0) * std::sin(theta0),
							    std::sin(phi0) * std::sin(theta0),
							    std::cos(theta0));
  __gnu_cxx::von_mises_fisher_distribution<3, double> vmd3(mu3, 100.0);
  std::array<double, 3> mean3;
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir3 = vmd3(re);
      mean3 += dir3;
      std::cout << "  (" << std::setw(12) << dir3[0]
		 << ", " << std::setw(12) << dir3[1]
		 << ", " << std::setw(12) << dir3[2]
                << ")  " << norm(dir3) << "\n";
    }
  mean3 /= 1000;
  std::cout << "  mean3 = ("
                    << std::setw(12) << mean3[0]
	    << ", " << std::setw(12) << mean3[1]
	    << ", " << std::setw(12) << mean3[2] << ")\n";
  mean3 = normal(mean3);
  std::cout << "  mean3 = ("
                    << std::setw(12) << mean3[0]
	    << ", " << std::setw(12) << mean3[1]
	    << ", " << std::setw(12) << mean3[2] << ")\n";
  std::cout << "  vmd3 = " << vmd3 << '\n';


  auto mu4 = std::experimental::make_array(0.1, 0.3, 0.4, std::sqrt(1.0 - 0.01 - 0.09 - 0.16));
  __gnu_cxx::von_mises_fisher_distribution<4, double> vmd4(mu4, 100.0);
  std::array<double, 4> mean4;
  for (auto i = 0; i < 1000; ++i)
    {
      auto dir4 = vmd4(re);
      mean4 += dir4;
      std::cout << "  (" << std::setw(12) << dir4[0]
		 << ", " << std::setw(12) << dir4[1]
		 << ", " << std::setw(12) << dir4[2]
		 << ", " << std::setw(12) << dir4[3]
                << ")  " << norm(dir4) << "\n";
    }
  mean4 /= 1000;
  std::cout << "  mean4 = ("
                    << std::setw(12) << mean4[0]
	    << ", " << std::setw(12) << mean4[1]
	    << ", " << std::setw(12) << mean4[2]
	    << ", " << std::setw(12) << mean4[3] << ")\n";
  mean4 = normal(mean4);
  std::cout << "  mean4 = ("
                    << std::setw(12) << mean4[0]
	    << ", " << std::setw(12) << mean4[1]
	    << ", " << std::setw(12) << mean4[2]
	    << ", " << std::setw(12) << mean4[3] << ")\n";
  std::cout << "  vmd4 = " << vmd4 << '\n';
}
