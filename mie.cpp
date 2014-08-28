#include <vector>
#include <complex>
#include <iostream>
#include <cmath>
#include <algorithm>

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
}


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
    std::vector<std::vector<double>> & dph)
{
  auto imaxx = 12000L;

  constexpr double dpi = 3.1415926535897932385L;
  std::vector<double> dqv2{dqv};
  if (dqv.end() == std::find(dqv.begin(), dqv.end(), -1.0))
    dqv2.push_back(-1.0);
  std::vector<double> ph(dqv.size());
  xs1.resize(dx.size(), std::vector<std::complex<double>>(dqv.size()));
  xs2.resize(dx.size(), std::vector<std::complex<double>>(dqv.size()));
  dph.resize(dx.size(), std::vector<double>(dqv.size()));

  dqxt.resize(dx.size());
  dqsc.resize(dx.size());
  dqbk.resize(dx.size());
  dg.resize(dx.size());

  for (auto size = 0l; size < dx.size(); ++size)
  {
    if (dx[size] > imaxx)
      std::cerr << "\n  **  Error: Size Parameter Overflow in Mie\n";
    auto ir = 1.0 / cm;
    auto y =  dx[size] * cm;

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

    std::vector<std::complex<double>> sm(dqv2.size());
    std::vector<std::complex<double>> sp(dqv2.size());
    std::vector<std::complex<double>> pi0(dqv2.size());
    std::vector<std::complex<double>> pi1(dqv2.size(), 1.0);

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
      auto psi = double(tnm1) * psi1 / dx[size] - psi0;
      auto chi = tnm1 * chi1 / dx[size] - chi0;
      std::complex<double> xi{psi, chi};
      auto a = ((d[n] * ir + rnx) * psi - psi1) / ((d[n] * ir + rnx) * xi - xi1);
      auto b = ((d[n] * cm + rnx) * psi - psi1) / ((d[n] * cm + rnx) * xi - xi1);
      dqxt[size] += tnp1 * std::real(a + b);
      dqsc[size] += tnp1 * std::real(a * std::conj(a) + b * std::conj(b));
      if (n > 1)
        dg[size] += (dn * dn - 1.0) * std::real(anm1 * std::conj(a) + bnm1 * std::conj(b)) / dn
                  + tnm1 * std::real(anm1 * std::conj(bnm1)) / (dn * dn - dn);
      anm1 = a;
      bnm1 = b;

      for (auto k = 0; k < dqv2.size(); ++k)
      {
	auto s = dqv2[k] * pi1[k];
	auto t = s - pi0[k];
	if (dph.size() > 0)
	{
          auto taun = dn * t - pi0[k];
          sp[k] = (a2 * (a + b)) * (pi1[k] + taun) + sp[k];
          sm[k] = (a2 * (a - b)) * (pi1[k] - taun) + sm[k];
	}
	pi0[k] = pi1[k];
	pi1[k] = s + t * turbo;
      }

      psi0 = psi1;
      psi1 = psi;
      chi0 = chi1;
      chi1 = chi;
      xi1 = std::complex<double>{psi1, chi1};
    }

    if (dg[size] > 0.0)
      dg[size] = 2.0 * dg[size] / dqsc[size];

    if (dph.size() > 0)
    {
      for (auto k = 0; k < dqv.size(); ++k)
      {
	xs1[size][k] = (sp[k] + sm[k]) / 2.0;
	xs2[size][k] = (sp[k] - sm[k]) / 2.0;
	dph[size][k] = std::real(xs1[size][k] * std::conj(xs1[size][k])
                               + xs2[size][k] * std::conj(xs2[size][k]))
                               / dqsc[size];
      }
    }

    // Assumes last slot has theta = 180 or cos(theta) = -1 point.
    dqbk[size] += (sp[dqv2.size() - 1] + sm[dqv2.size() - 1]) / 2.0;

    auto dx2 = dx[size] * dx[size];
    dqsc[size] =  2.0 * dqsc[size] / dx2;
    dqxt[size] =  2.0 * dqxt[size] / dx2;
    dqbk[size] =  std::real(dqbk[size] * std::conj(dqbk[size])) / dx2 / dpi;
  }
}
