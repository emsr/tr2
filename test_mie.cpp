
//  /home/ed/bin/bin/g++ -std=c++14 -o test_mie test_mie.cpp

#include <iostream>
#include <iomanip>

#include "mie.h"

int RunMieScatteringModel();

int
main()
{
  std::string answer;
  do
  {
    RunMieScatteringModel();

    std::cout << "Do you wish to run MieScatteringModel again (y/n)? ";
    std ::cin >> answer;
  }
  while ( answer[0] == 'y' || answer[0] == 'Y' );

  return 0;
}

int
RunMieScatteringModel()
{
  constexpr double pi = 3.1415926535897932385L;
  constexpr double k = 2.0L * pi;

  double ka;
  double a;
  std::cout << "\nEnter particle radii in units of wavelength.";
  std::cout << "\nParticle radius: ";
  std::cin >> a;
  ka = k * a;

  std::complex<double> N;
  std::cout << "Enter particle complex index of refraction: ";
  std::cin >> N;

  std::vector<double> cos_theta;
  for (auto k = 50; k >= -50; --k)
    cos_theta.push_back(0.02 * k);

  double eff_extinct;
  double eff_scatter;
  std::complex<double> eff_backscatt;
  double asymmetry;
  std::vector<std::complex<double>> amp_perp;
  std::vector<std::complex<double>> amp_para;
  std::vector<double> phase;
  mie(ka, N, cos_theta,
      eff_extinct, eff_scatter, eff_backscatt,
      asymmetry, amp_perp, amp_para, phase);

  std::cout << "eff_extinct: " << std::setw(12) << eff_extinct << '\n';

  std::cout << "eff_scatter: " << std::setw(12) << eff_scatter << '\n';

  std::cout << "eff_backscatt: " << std::setw(12) << eff_backscatt << '\n';

  std::cout << "asymmetry: " << std::setw(12) << asymmetry  << '\n';

  std::cout << "amp_perp\n";
  for (auto&& perp : amp_perp)
    std::cout << '\t' << std::setw(12) << perp;
  std::cout << '\n';
  std::cout << '\n';

  std::cout << "amp_para\n";
  for (auto&& para : amp_para)
    std::cout << '\t' << std::setw(12) << para;
  std::cout << '\n';
  std::cout << '\n';

  std::cout << "phase\n";
  for (auto&& ph : phase)
    std::cout << '\t' << std::setw(12) << ph;
  std::cout << '\n';
  std::cout << '\n';
}
