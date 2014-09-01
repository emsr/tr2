#include <iostream>
#include <typeinfo>
#include <complex>

int
main()
{
  using namespace std::literals::complex_literals;

  auto x = 3.1415F;
  std::cout << typeid(x).name() << '\n';
  auto z = 1.0 + 1.0i;
  std::cout << typeid(z).name() << '\n';
}
