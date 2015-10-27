// $HOME/bin/bin/g++ -std=c++14 -o test_hankel test_hankel.cpp > /dev/null

#include "hankel.tcc"

int
main()
{
  using namespace std::literals::complex_literals;

  std::complex<double> h1, h2, h1p, h2p;
  std::complex<double> z, nu;

  nu = 5.0 - 0.1i;
  z = 1.0 - 3.0i;

  uniform_hankel(h1, h2, h1p, h2p, z, nu);
  std::cout << "H1(z) = " << h1 << '\n';
  std::cout << "H1'(z) = " << h1 << '\n';
  std::cout << "H2(z) = " << h1 << '\n';
  std::cout << "H2'(z) = " << h1 << '\n';
}
