// $HOME/bin/bin/g++ -std=c++14 -o test_hankel test_hankel.cpp > /dev/null

#include "hankel.tcc"

int
main()
{
  using namespace std::literals::complex_literals;

  std::complex<double> h1, h2, h1p, h2p;
  std::complex<double> z, nu;

  nu = 5.0;
  z = 1.0 - 3.0i;

  uniform_hankel(nu, z, h1, h2, h1p, h2p);

  std::cout.precision(std::numeric_limits<double>::digits10);
  std::cout << "nu     = " << nu << '\n'
            << "z      = " << z << '\n';
  std::cout << "H1(z)  = " << h1 << '\n';
  std::cout << "H1'(z) = " << h1p << '\n';
  std::cout << "H2(z)  = " << h2 << '\n';
  std::cout << "H2'(z) = " << h2p << '\n';
  std::cout << "J(z)   = " << (h1 + h2) / 2.0 << '\n';
  std::cout << "Y(z)   = " << (h1 - h2) / 2.0 << '\n';
}
