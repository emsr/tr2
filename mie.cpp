#include <iostream>
#include <cmath>
#include <algorithm>

#include "mie.h"

void
mie(std::vector<double> dx,
    std::complex<double> N,
    std::vector<double> cos_theta,
    std::vector<double> & eff_extinct,
    std::vector<double> & eff_scatter,
    std::vector<std::complex<double>> & eff_backscatt,
    std::vector<double> & asymmetry,
    std::vector<std::vector<std::complex<double>>> & amp_perp,
    std::vector<std::vector<std::complex<double>>> & amp_para,
    std::vector<std::vector<double>> & phase)
{
  auto imaxx = 12000L;

  constexpr double dpi = 3.1415926535897932385L;
  std::vector<double> cos_theta2{cos_theta};
  std::sort(cos_theta2.begin(), cos_theta2.end(), std::greater<>());
  if (cos_theta2.end() == std::find(cos_theta2.begin(), cos_theta2.end(), -1.0))
    cos_theta2.push_back(-1.0);
//for (auto && v2 : cos_theta2) std::cout << ' ' << v2;
//std::cout << '\n';
  std::vector<double> ph(cos_theta.size());
  amp_perp.resize(dx.size(), std::vector<std::complex<double>>(cos_theta.size()));
  amp_para.resize(dx.size(), std::vector<std::complex<double>>(cos_theta.size()));
  phase.resize(dx.size(), std::vector<double>(cos_theta.size()));

  eff_extinct.resize(dx.size());
  eff_scatter.resize(dx.size());
  eff_backscatt.resize(dx.size());
  asymmetry.resize(dx.size());

  for (auto size = 0L; size < dx.size(); ++size)
  {
    if (dx[size] > imaxx)
      std::cerr << "\n  **  Error: Size Parameter Overflow in Mie\n";
    auto invN = 1.0 / N;
    auto y =  dx[size] * N;

    long nstop;
    if (dx[size] < 0.02)
      nstop = 2;
    else if (dx[size] <= 8.0)
      nstop = dx[size] + 4.00 * std::pow(dx[size], 1.0/3.0) + 2.0;
    else if (dx[size] < 4200.0)
      nstop = dx[size] + 4.05 * std::pow(dx[size], 1.0/3.0) + 2.0;
    else
      nstop = dx[size] + 4.00 * std::pow(dx[size], 1.0/3.0) + 2.0;

    auto nmx = long(std::max(nstop, long(std::abs(y))) + 15.0);
    std::vector<std::complex<double>> d(nmx + 1);

    for (auto n = nmx - 1; n >= 1; --n)
    {
      auto a1 = double(n + 1) / y;
      d[n] = a1 - 1.0 / (a1 + d[n + 1]);
    }

    std::vector<std::complex<double>> sm(cos_theta2.size());
    std::vector<std::complex<double>> sp(cos_theta2.size());
    std::vector<std::complex<double>> pi0(cos_theta2.size());
    std::vector<std::complex<double>> pi1(cos_theta2.size(), 1.0);

    auto psi0 =  std::cos(dx[size]);
    auto psi1 =  std::sin(dx[size]);
    auto chi0 = -std::sin(dx[size]);
    auto chi1 =  std::cos(dx[size]);
    std::complex<double> xi0{psi0, chi0};
    std::complex<double> xi1{psi1, chi1};

    auto tnp1 = 1.0;

    std::complex<double> anm1, bnm1;
    for (auto n = 1L; n <= nstop; ++n)
    {
      const auto dn = double(n);
      tnp1 += 2.0;
      auto tnm1 = tnp1 - 2.0;
      auto a2 = tnp1 / (dn * (dn + 1.0));
      auto turbo = (dn + 1.0) / dn;
      auto rnx = dn / dx[size];
      auto psi = tnm1 * psi1 / dx[size] - psi0;
      auto chi = tnm1 * chi1 / dx[size] - chi0;
      std::complex<double> xi{psi, chi};
      auto a = ((d[n] * invN + rnx) * psi - psi1) / ((d[n] * invN + rnx) * xi - xi1);
      auto b = ((d[n] * N + rnx) * psi - psi1) / ((d[n] * N + rnx) * xi - xi1);
      eff_extinct[size] += tnp1 * std::real(a + b);
      eff_scatter[size] += tnp1 * (std::norm(a) + std::norm(b));
      if (n > 1L)
        asymmetry[size] += (dn * dn - 1.0) * std::real(anm1 * std::conj(a) + bnm1 * std::conj(b)) / dn
                         + tnm1 * std::real(anm1 * std::conj(bnm1)) / (dn * dn - dn);
      anm1 = a;
      bnm1 = b;

      for (auto k = 0; k < cos_theta2.size(); ++k)
      {
	auto s = cos_theta2[k] * pi1[k];
	auto t = s - pi0[k];
        auto taun = dn * t - pi0[k];
        sp[k] += (a2 * (a + b)) * (pi1[k] + taun);
        sm[k] += (a2 * (a - b)) * (pi1[k] - taun);
	pi0[k] = pi1[k];
	pi1[k] = s + t * turbo;
      }

      psi0 = psi1;
      psi1 = psi;
      chi0 = chi1;
      chi1 = chi;
      xi1 = std::complex<double>{psi1, chi1};
    }

    if (asymmetry[size] > 0.0)
      asymmetry[size] = 2.0 * asymmetry[size] / eff_scatter[size];

    for (auto k = 0; k < cos_theta.size(); ++k)
    {
      amp_perp[size][k] = (sp[k] + sm[k]) / 2.0;
      amp_para[size][k] = (sp[k] - sm[k]) / 2.0;
      phase[size][k] = (std::norm(amp_perp[size][k]) + std::norm(amp_para[size][k]))
                     / eff_scatter[size];
    }

    // Assumes last slot has theta = 180 or cos(theta) == -1 point.
    eff_backscatt[size] += (sp.back() + sm.back()) / 2.0;

    auto dx2 = dx[size] * dx[size];
    eff_scatter[size] *= 2.0 / dx2;
    eff_extinct[size] *= 2.0 / dx2;
std::cout << eff_backscatt[size] << '\n';
    eff_backscatt[size] = std::norm(eff_backscatt[size]) / dx2 / dpi;
  }
}
