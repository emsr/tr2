// $HOME/bin/bin/g++ -std=c++11 -o trigint_error trigint_error.cpp

#include <iostream>
#include <iomanip>
#include "cmath"

int
main()
{
  double x = 2.2;
  double c = cosint(x);
  double s = sinint(x);

  std::cout << std::endl;
  for (int i = 180; i <= 300; ++i)
    {
      double x = 0.01 * i;
      std::cout << std::setw(10) << x;
      double ciser, siser;
      __detail::__csint_series(x, ciser, siser);
      std::cout << ' ' << std::setw(10) << ciser << ' ' << std::setw(10) << siser;
      double cicfr, sicfr;
      __detail::__csint_cont_frac(x, cicfr, sicfr);
      std::cout << ' ' << std::setw(10) << cicfr << ' ' << std::setw(10) << sicfr;
      std::cout << ' ' << std::setw(10) << cicfr - ciser << ' ' << std::setw(10) << sicfr - siser;
      //std::cout << ' ' << std::setw(10) <<  << ' ' << std::setw(10) << ;
      //std::cout << ' ' << std::setw(10) <<  << ' ' << std::setw(10) << ;
      std::cout << std::endl;
    }
  std::cout << std::endl;

  std::cout << std::endl;
  for (int i = 50; i <= 2500; ++i)
    {
      double x = 1.0 * i;
      std::cout << std::setw(10) << x;
      double ciasm, siasm;
      __detail::__csint_asymp(x, ciasm, siasm);
      std::cout << ' ' << std::setw(10) << ciasm << ' ' << std::setw(10) << siasm;
      double cicfr, sicfr;
      __detail::__csint_cont_frac(x, cicfr, sicfr);
      std::cout << ' ' << std::setw(10) << cicfr << ' ' << std::setw(10) << sicfr;
      std::cout << ' ' << std::setw(10) << cicfr - ciasm << ' ' << std::setw(10) << sicfr - siasm;
      //std::cout << ' ' << std::setw(10) <<  << ' ' << std::setw(10) << ;
      //std::cout << ' ' << std::setw(10) <<  << ' ' << std::setw(10) << ;
      std::cout << std::endl;
    }
  std::cout << std::endl;

}
