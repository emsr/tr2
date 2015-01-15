
// /home/ed/bin/bin/g++ -std=c++1y -fdiagnostics-color=auto -o variable_template variable_template.cpp

#include <array>
#include <complex>
#include <iostream>

using namespace std::literals::complex_literals;

// Write pi once for every possible type
template<typename Tp>
  constexpr Tp
  pi = static_cast<Tp>(3.1415926535897932385L);

// Sample use
template<typename Tp>
  constexpr Tp
  circular_area(Tp r)
  {
    return pi<Tp> * r * r; // ICE
  }

template<typename Tp>
  struct pauli : std::array<std::array<std::complex<Tp>, 2>, 2>
  {
    constexpr pauli(std::array<std::array<std::complex<Tp>, 2>, 2> arr) 
    : std::array<std::array<std::complex<Tp>, 2>, 2>(arr) 
    {}
  };

template<typename T>
  constexpr pauli<T> sigma0{ {{ {{ 1.0L, 0.0L }}, {{ 0.0L, 1.0L }} }} };

template<typename T>
  constexpr pauli<T> sigma1{ {{ {{ 0.0L, 1.0L }}, {{ 1.0L, 0.0L }} }} };

template<typename T>
  constexpr pauli<T> sigma2{ {{ {{ 0.0L, -1.0il }}, {{ 1.0il, 0.0L }} }} };

template<typename T>
  constexpr pauli<T> sigma3{ {{ {{ 1.0L, 0.0L }}, {{ -1.0L, 0.0L }} }} };

//template<typename T>
//  T x;

//template<typename T>
//  std::complex<T> z;

int
main()
{
  std::cout << circular_area(2.0f) << '\n';

  //pauli<long double> herm22 = sigma2<long double>;
  pauli<long double> herm22 = sigma0<long double>;

  //x = 5.0;
}
