
//  /home/ed/bin/bin/g++ -std=c++14 -o test_mie test_mie.cpp mie.cpp

//  

#include <vector>
#include <complex>
#include <iostream>
#include <iomanip>

void
mie(std::vector<double> dx,
    std::complex<double> cm,
    std::vector<double> dqv,
    std::vector<double> & dqxt,
    std::vector<double> & dqsc,
    std::vector<std::complex<double>> & dqbk,
    std::vector<double> & dg,
    std::vector<std::vector<std::complex<double>>> & xs1,
    std::vector<std::vector<std::complex<double>>> & xs2,
    std::vector<std::vector<double>> & dph);

int
main()
{
  std::vector<double> dx;
  int na = 0;
  double a;
  std::cout << "\nEnter particle radii in units of wavelength.";
  std::cout << "\nParticle radius " << ++na << ": ";
  while ((std::cin >> a) && a > 0.0)
  {
    dx.push_back(a);
    std::cout << "\nParticle radius " << ++na << ": ";
  }

  std::complex<double> cm;
  std::cout << "Enter particle complex index of refraction: ";
  std::cin >> cm;

  std::vector<double> dqv;
  for (auto k = 10; k >= -10; --k)
    dqv.push_back(0.1 * k);

  std::vector<double> dqxt;
  std::vector<double> dqsc;
  std::vector<std::complex<double>> dqbk;
  std::vector<double> dg;
  std::vector<std::vector<std::complex<double>>> xs1;
  std::vector<std::vector<std::complex<double>>> xs2;
  std::vector<std::vector<double>> dph;
  mie(dx, cm, dqv,
      dqxt, dqsc, dqbk, dg, xs1, xs2, dph);

  std::cout << "dqxt\n";
  for (auto&& qxt : dqxt)
    std::cout << '\t' << std::setw(12) << qxt;
  std::cout << '\n';

  std::cout << "dqsc\n";
  for (auto&& qsc : dqsc)
    std::cout << '\t' << std::setw(12) << qsc;
  std::cout << '\n';

  std::cout << "dqbk\n";
  for (auto&& qbk : dqbk)
    std::cout << '\t' << std::setw(12) << qbk;
  std::cout << '\n';

  std::cout << "dg\n";
  for (auto&& g : dg)
    std::cout << '\t' << std::setw(12) << g;
  std::cout << '\n';

  std::cout << "xs1\n";
  for (auto&& s1 : xs1)
  {
    for (auto&& x : s1)
      std::cout << '\t' << std::setw(12) << x;
    std::cout << '\n';
  }
  std::cout << '\n';

  std::cout << "xs2\n";
  for (auto&& s2 : xs2)
  {
    for (auto&& x : s2)
      std::cout << '\t' << std::setw(12) << x;
    std::cout << '\n';
  }
  std::cout << '\n';

  std::cout << "dph\n";
  for (auto&& ph : dph)
  {
    for (auto&& p : ph)
      std::cout << '\t' << std::setw(12) << p;
    std::cout << '\n';
  }
  std::cout << '\n';
}
