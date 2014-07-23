
// /home/ed/bin_var_template_2/bin/g++ -std=c++1y -fdiagnostics-color=auto -o variable_template_2 variable_template_2.cpp

#include <iostream>

// Write pi once for every possible type
template<typename Tp>
  constexpr Tp pi = Tp(3.1415926535897932385L);

// Sample use
template<typename Tp>
  Tp
  circular_area(Tp r)
  {
    return pi<Tp> * r * r;
  }

int
main()
{
  //std::cout << "pi<float> = " << pi<float> << std::endl;
  std::cout << "circular_area(2.0) = " << circular_area(2.0) << std::endl;
}

// ./bin/bin/g++ -std=c++1y -fdiagnostics-color=auto -o variable_template_2 variable_template_2.cpp
