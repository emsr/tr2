
// /home/ed/bin_var_template_2/bin/g++ -std=c++1y -fdiagnostics-color=auto -o variable_template variable_template.cpp

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

template<typename T>
  struct pauli
  {
    std::array<std::array<std::complex<T>, 2>, 2> mat;
  };

template<typename T>
  constexpr pauli<T> sigma0 = { { 1.0l, 0.0l }, { 0.0l, 1.0l } };

template<typename T>
  constexpr pauli<T> sigma1 = { { 0.0l, 1.0l }, { 1.0l, 0.0l } };

template<typename T>
  constexpr pauli<T> sigma2 = { { 0.0l, -1.0il }, { 1.0il, 0.0l } };

template<typename T>
  constexpr pauli<T> sigma3 = { { 1.0l, 0.0l }, { -1.0l, 0.0l } };

template<typename T>
  T x;

template<typename T>
  std::complex<T> z;

int
main()
{
  std::cout << circular_area(2.0f) << '\n';

  pauli<long double> herm22 = sigma2<long double>;

  x = 5.0;
}

// ./bin/bin/g++ -std=c++14 -fdiagnostics-color=auto -o variable_template variable_template.cpp
