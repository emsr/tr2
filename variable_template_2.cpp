
// /home/ed/bin_var_template_2/bin/g++ -std=c++1y -fdiagnostics-color=auto -o variable_template_2 variable_template_2.cpp

#include <iostream>

// Write pi once for every possible type
template<typename Tp>
  constexpr Tp pi = Tp(3.1415926535897932385L);

// Write pi once for every possible type
namespace xxx
{
template<typename Tp>
  constexpr Tp eee = Tp(2.81828L);
}

// Sample use
template<typename Tp>
  Tp
  circular_area(Tp r)
  {
    return pi<Tp> * r * r;
  }

// Write n pi once for every possible type
template<typename Tp, std::size_t N>
  constexpr Tp npi = N * Tp(3.1415926535897932385L);

// Partial specialization for int type.
//template<std::size_t N>
//  constexpr double npi<int, N> = N * double(3.1415926535897932385L);

int
main()
{
  //std::cout << "pi<float> = " << pi<float> << std::endl;
  std::cout << "npi<double, 2> = " << npi<double, 2> << std::endl;
  std::cout << "circular_area(2.0) = " << circular_area(2.0) << std::endl;
  std::cout << "eee = " << xxx::eee<double> << std::endl;
}

// ./bin/bin/g++ -std=c++1y -fdiagnostics-color=auto -o variable_template_2 variable_template_2.cpp
