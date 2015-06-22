
// $HOME/bin/bin/g++ -std=c++14 -o test_mie test_mie.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./test_mie

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>

#include "mie.h"

template<typename Num>
  void RunMieScatteringModel(int n = 0);

using microsecs = std::chrono::duration<long, std::micro>;
template<typename _T>
  long
  timeElapsed(const std::chrono::time_point<_T>& curr,
              const std::chrono::time_point<_T>& prev)
  {
    long tm = std::chrono::duration_cast<microsecs>(curr - prev).count();
    return tm;
  }


int
main()
{
  std::string answer;
  int n = 1;
  do
  {
    auto prev_time = std::chrono::system_clock::now();
    RunMieScatteringModel<double>(n);
    auto curr_time = std::chrono::system_clock::now();
    std::cout << "\nTime elapsed: " << 1.0 * timeElapsed(curr_time, prev_time) << " us\n";
    std::cout << "\nDo you wish to run MieScatteringModel again (y/n)? ";
    std ::cin >> answer;
    ++n;
  }
  while ( answer[0] == 'y' || answer[0] == 'Y' );

  std::cout << '\n';

  return 0;
}


template<typename Num>
  void
  RunMieScatteringModel(int n)
  {
    constexpr Num pi = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;
    constexpr Num k = 2.0L * pi;

    Num ka;
    Num a;
    std::cout << "\nEnter particle radius in units of wavelength: ";
    std::cin >> a;
    ka = k * a;

    std::complex<Num> N;
    std::cout << "\nEnter particle complex index of refraction  : ";
    std::cin >> N;

    bool even_angles = true;
    std::vector<Num> cos_theta;
    if (even_angles)
    {
      //  Even angles.
      for (auto k = 0; k <= 180; ++k)
        cos_theta.push_back(std::cos(k * pi / 180));
    }
    else
    {
      //  Even cosines.
      for (auto k = 50; k >= -50; --k)
        cos_theta.push_back(0.02L * k);
    }
    //for (auto k = 0L; k < cos_theta.size(); ++k)
    //  std::cout << cos_theta[k] << '\n';

    Num eff_extinct;
    Num eff_scatter;
    std::complex<Num> eff_backscatt;
    Num asymmetry;
    std::vector<std::complex<Num>> amp_perp;
    std::vector<std::complex<Num>> amp_para;
    std::vector<Num> phase;
    std::vector<Num> polarization;
    mie(ka, N, cos_theta,
        eff_extinct, eff_scatter, eff_backscatt,
        asymmetry, amp_perp, amp_para, phase, polarization);

    std::cout << "Particle radius        : " << a << '\n';
    std::cout << "Particle size parameter: " << ka << '\n';
    std::cout << "Index of refraction    : " << N << '\n';
    std::cout << '\n';
    std::cout << "Extinction Efficiency  : " << std::setw(12) << eff_extinct << '\n';
    std::cout << "Scattering Efficiency  : " << std::setw(12) << eff_scatter << '\n';
    std::cout << "Backscatter Efficiency : " << std::setw(12) << eff_backscatt << '\n';
    std::cout << "Asymmetry              : " << std::setw(12) << asymmetry  << '\n';

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

    std::ostringstream plot_file;
    plot_file << "plot" << n << ".dat";
    std::ofstream plot(plot_file.str());
    plot << "# Particle radius    : " << a << '\n';
    plot << "# Index of refraction: " << N << '\n';
    plot << "#\n";
    plot << "# gnuplot> plot \"" << plot_file.str() << "\" using 1:2 title \"A_perp\"\n";
    plot << "# gnuplot> plot \"" << plot_file.str() << "\" using 1:3 title \"A_para\"\n";
    plot << "#\n";
    plot << "#     theta       A_perp       A_para\n";
    for (auto k = 0L; k < cos_theta.size(); ++k)
    {
      plot << ' ' << std::setw(12) << 180.0 * std::acos(cos_theta[k]) / pi
           << ' ' << std::setw(12) << std::abs(amp_perp[k])
           << ' ' << std::setw(12) << std::abs(amp_para[k])
           << ' ' << std::setw(12) << phase[k]
           << ' ' << std::setw(12) << polarization[k]
           << '\n';
    }
    std::cout << "\nPlot data file written to \"" << plot_file.str() << "\"\n";
  }
