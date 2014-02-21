
//  /home/ed/bin/bin/g++ -std=c++1y -o complex_ref complex_ref.cpp

#include <complex>
#include <functional>
#include <iostream>

int
main()
{
  double x{}, y{1};
  std::complex<std::reference_wrapper<double>> z{std::ref(x), std::ref(y)};
  std::cout << z << std::endl;
  x = 5;
  y = 4;
  std::cout << z << std::endl;
  std::complex<double> w{2.0, 3.0};
  w = z; // Can't go the other way.
  std::cout << w << std::endl;
  //z.real(15.0);
  //z.imag(30.0);
  //std::cout << "z = " << z << std::endl;
  //std::cout << "x = " << x << std::endl;
  //std::cout << "y = " << y << std::endl;
}
