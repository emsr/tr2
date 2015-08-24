#include <complex>
#include <iostream>
#include <tuple> // For ignore and get and tuple_size.
#include <functional> // For reference_wrapper and ref.
#include <experimental/array> // For make_array.

// $HOME/bin/bin/g++ -std=c++1y -o complex_tie complex_tie.cpp

namespace std
{

  template<typename _Tp>
    inline array<_Tp, 2>
    make_array(complex<_Tp> & __z)
    { return make_array(__z.real(), __z.imag()); }

  template<typename _Tp>
    inline complex<_Tp&>
    complex_tie(_Tp& __re, _Tp& __im) noexcept
    { return complex<std::reference_wrapper<_Tp>>(std::ref(__re), std::ref(__im)); }

  template<typename _Tp>
    inline complex<_Tp&>
    complex_tie2(_Tp& __re, _Tp& __im) noexcept
    {
      //_Tp& (&z)[2]{__re, __im}; Nope!
      return complex<_Tp&>(std::forward<_Tp&>(__re), std::forward<_Tp&>(__im));
    }
}

int
test1()
{
  std::complex<double> z(1.0, -1.0);
  double re = 0.0, im = 0.0;
  std::complex_tie(re, im) = z;
  std::cout << "z = " << z << std::endl;
  std::cout << "re = " << re << std::endl;
  std::cout << "im = " << im << std::endl;
  re = 5.0; // Not working - 'z.real()' stays the same!!!  Actually, re is just a double.
  std::cout << "z = " << z << std::endl;
  z.real(5.0);
  z.imag(3.0); // Not working - 'im' stays the same!!!
  std::cout << "z = " << z << std::endl;
  std::cout << "re = " << re << std::endl;
  std::cout << "im = " << im << std::endl;
}

int
test2()
{
  std::complex<double> z(1.0, -1.0);
  double re = 0.0, im = 0.0;
  std::complex_tie2(re, im) = z;
  std::cout << "z = " << z << std::endl;
  std::cout << "re = " << re << std::endl;
  std::cout << "im = " << im << std::endl;
  re = 5.0; // Not working - 'z.real()' stays the same!!!  Actually, re is just a double.
  std::cout << "z = " << z << std::endl;
  z.real(5.0);
  z.imag(3.0); // Not working - 'im' stays the same!!!
  std::cout << "z = " << z << std::endl;
  std::cout << "re = " << re << std::endl;
  std::cout << "im = " << im << std::endl;
}

int
main()
{
  std::cout << "-----test1-----" << std::endl;
  test1();
  std::cout << "-----test2-----" << std::endl;
  test2();
}
