
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
    std::vector<double> & eff_extinct,
    std::vector<double> & eff_scatter,
    std::vector<std::complex<double>> & eff_backscatt,
    std::vector<double> & asymmetry,
    std::vector<std::vector<std::complex<double>>> & amp_perp,
    std::vector<std::vector<std::complex<double>>> & amp_para,
    std::vector<std::vector<double>> & phase);

int
main()
{
  constexpr double pi = 3.1415926535897932385L;
  constexpr double k = 2.0L * pi;

  std::vector<double> dx;
  int na = 0;
  double a;
  std::cout << "\nEnter particle radii in units of wavelength.";
  std::cout << "\nParticle radius " << ++na << ": ";
  while ((std::cin >> a) && a > 0.0)
  {
    dx.push_back(k * a);
    std::cout << "\nParticle radius " << ++na << ": ";
  }

  std::complex<double> cm;
  std::cout << "Enter particle complex index of refraction: ";
  std::cin >> cm;

  std::vector<double> dqv;
  for (auto k = 50; k >= -50; --k)
    dqv.push_back(0.02 * k);

  std::vector<double> eff_extinct;
  std::vector<double> eff_scatter;
  std::vector<std::complex<double>> eff_backscatt;
  std::vector<double> asymmetry;
  std::vector<std::vector<std::complex<double>>> amp_perp;
  std::vector<std::vector<std::complex<double>>> amp_para;
  std::vector<std::vector<double>> phase;
  mie(dx, cm, dqv,
      eff_extinct, eff_scatter, eff_backscatt,
      asymmetry, amp_perp, amp_para, phase);

  std::cout << "eff_extinct\n";
  for (auto&& extinct : eff_extinct)
    std::cout << '\t' << std::setw(12) << extinct;
  std::cout << '\n';

  std::cout << "eff_scatter\n";
  for (auto&& scatter : eff_scatter)
    std::cout << '\t' << std::setw(12) << scatter;
  std::cout << '\n';

  std::cout << "eff_backscatt\n";
  for (auto&& backscatt : eff_backscatt)
    std::cout << '\t' << std::setw(12) << backscatt;
  std::cout << '\n';

  std::cout << "asymmetry\n";
  for (auto&& asym : asymmetry)
    std::cout << '\t' << std::setw(12) << asym;
  std::cout << '\n';

  std::cout << "amp_perp\n";
  for (auto&& s1 : amp_perp)
  {
    for (auto&& perp : s1)
      std::cout << '\t' << std::setw(12) << perp;
    std::cout << '\n';
  }
  std::cout << '\n';

  std::cout << "amp_para\n";
  for (auto&& s2 : amp_para)
  {
    for (auto&& para : s2)
      std::cout << '\t' << std::setw(12) << para;
    std::cout << '\n';
  }
  std::cout << '\n';

  std::cout << "phase\n";
  for (auto&& ph : phase)
  {
    for (auto&& p : ph)
      std::cout << '\t' << std::setw(12) << p;
    std::cout << '\n';
  }
  std::cout << '\n';
}
