#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "mie.h"


template<typename Num>
  std::complex<Num>
  cont_frac(long nmx, std::complex<Num> y);


///
/// @see Improved Mie scattering algorithms
///      W. J. Wiscombe
///      Applied Optics, Vol. 19, No. 9, pp 1505-1509, 1 May 1980
///
template<typename Num>
  void
  mie(Num ka,
      std::complex<Num> N,
      std::vector<Num> cos_theta,
      Num & eff_extinct,
      Num & eff_scatter,
      std::complex<Num> & eff_backscatt,
      Num & asymmetry,
      std::vector<std::complex<Num>> & amp_perp,
      std::vector<std::complex<Num>> & amp_para,
      std::vector<Num> & phase,
      std::vector<Num> & polarization)
  {
    constexpr auto imaxx = 12000L;

    constexpr Num pi = Num(3.1415926535897932385L);
    std::vector<Num> cos_theta2{cos_theta};
    std::sort(cos_theta2.begin(), cos_theta2.end(), std::greater<>());
    if (cos_theta2.end() == std::find(cos_theta2.begin(), cos_theta2.end(), -1.0))
      cos_theta2.push_back(-1.0);
    std::vector<Num> ph(cos_theta.size());
    amp_perp.resize(cos_theta.size());
    amp_para.resize(cos_theta.size());
    phase.resize(cos_theta.size());
    polarization.resize(cos_theta.size());

    eff_extinct = Num(0);
    eff_scatter = Num(0);
    eff_backscatt = Num(0);
    asymmetry = Num(0);

    if (ka > imaxx)
      std::cerr << "\n  **  Error: Size Parameter Overflow in Mie\n";
    auto invN = Num(1) / N;
    auto y =  ka * N;

    //  Estimate number of terms in Mie series.
    long nstop;
    if (ka < Num(0.02L))
      nstop = 2;
    else if (ka <= Num(8))
      nstop = ka + Num(4) * std::pow(ka, Num(1.0L/3.0L)) + Num(1);
    else if (ka < Num(4200))
      nstop = ka + Num(4.05L) * std::pow(ka, Num(1.0L/3.0L)) + Num(2);
    else
      nstop = ka + Num(4) * std::pow(ka, Num(1.0L/3.0L)) + Num(2);

    auto nmx = long(std::max(nstop, long(std::abs(y))) + Num(15));
    std::vector<std::complex<Num>> d(nmx + 1);

    //  Get starting value from continued fraction.
    //  This may be unnecessary since we back up and do Millers algorithm.
    //  Other codes check input values and either do forward recursion or
    //  backwards starting right at the required max.
    //d[nmx] = cont_frac(nmx, y);

    //  Downward recurrence for Bessel function logarithmic derivative.
    for (auto n = nmx - 1; n >= 1; --n)
    {
      auto a1 = Num(n + 1) / y;
      d[n] = a1 - Num(1) / (a1 + d[n + 1]);
    }

    std::vector<std::complex<Num>> Sm(cos_theta2.size());
    std::vector<std::complex<Num>> Sp(cos_theta2.size());
    std::vector<std::complex<Num>> pi0(cos_theta2.size());
    std::vector<std::complex<Num>> pi1(cos_theta2.size(), Num(1));

    //  Initialize Ricatti-Bessel function recursion.
    auto psi0 =  std::cos(ka);
    auto psi1 =  std::sin(ka);
    auto chi0 = -std::sin(ka);
    auto chi1 =  std::cos(ka);
    std::complex<Num> xi0{psi0, chi0};
    std::complex<Num> xi1{psi1, chi1};

    auto tnp1 = Num(1);

    std::complex<Num> anm1, bnm1;
    for (auto n = 1L; n <= nstop; ++n)
    {
      const auto dn = Num(n);
      tnp1 += Num(2);
      auto tnm1 = tnp1 - Num(2);
      auto turbo = (dn + Num(1)) / dn;
      auto rnx = dn / ka;
      auto psi = tnm1 * psi1 / ka - psi0;
      auto chi = tnm1 * chi1 / ka - chi0;
      std::complex<Num> xi{psi, chi};
      auto a = ((d[n] * invN + rnx) * psi - psi1) / ((d[n] * invN + rnx) * xi - xi1);
      auto b = ((d[n] * N + rnx) * psi - psi1) / ((d[n] * N + rnx) * xi - xi1);
      eff_extinct += tnp1 * std::real(a + b);
      eff_scatter += tnp1 * (std::norm(a) + std::norm(b));
      if (n > 1L)
	asymmetry += (dn * dn - Num(1)) * std::real(anm1 * std::conj(a) + bnm1 * std::conj(b)) / dn
                   + tnm1 * std::real(anm1 * std::conj(bnm1)) / (dn * dn - dn);
      anm1 = a;
      bnm1 = b;

      auto a2 = tnp1 / (dn * (dn + Num(1)));
      auto Cp = a2 * (a + b);
      auto Cm = a2 * (a - b);

      //  Increment Mie sums for S+, S- while upward recursing angular function pi.
      for (auto k = 0; k < cos_theta2.size(); ++k)
      {
	auto s = cos_theta2[k] * pi1[k];
	auto t = s - pi0[k];
	auto taun = dn * t - pi0[k];
	Sp[k] += Cp * (pi1[k] + taun);
	Sm[k] += Cm * (pi1[k] - taun);
	pi0[k] = pi1[k];
	pi1[k] = s + t * turbo;
      }

      //  Upward recurrence of Ricatti-Bessel functions.
      psi0 = psi1;
      psi1 = psi;
      chi0 = chi1;
      chi1 = chi;
      xi1 = std::complex<Num>{psi1, chi1};
    }

    if (asymmetry > Num(0))
      asymmetry = Num(2) * asymmetry / eff_scatter;

    for (auto k = 0; k < cos_theta.size(); ++k)
    {
      amp_perp[k] = (Sp[k] + Sm[k]) / Num(2);
      amp_para[k] = (Sp[k] - Sm[k]) / Num(2);
      phase[k] = (std::norm(amp_perp[k]) + std::norm(amp_para[k]))
               / eff_scatter;
      polarization[k] = (std::norm(amp_perp[k]) - std::norm(amp_para[k]))
                      / (std::norm(amp_perp[k]) + std::norm(amp_para[k]));
    }

    // Assumes last slot has theta = 180 or cos(theta) == -1 point.
    eff_backscatt += (Sp.back() + Sm.back()) / Num(2);

    auto ka2 = ka * ka;
    eff_scatter *= Num(2) / ka2;
    eff_extinct *= Num(2) / ka2;
    eff_backscatt = std::norm(eff_backscatt) / ka2 / pi;
  }


template<typename Num>
  std::complex<Num>
  cont_frac(long n, std::complex<Num> y)
  {
    constexpr Num eps1 = Num(0.01L);
    constexpr Num eps2 = Num(1.0e-8L);
    constexpr long maxit = 10000;

    auto zinv = Num(1) / y;
    auto confra = Num(n + 1L) * zinv;
    auto mm = -1L;
    auto kk = 2L * n + 3L;
    auto ak = Num(mm * kk) * zinv;
    auto denom = ak;
    auto numer = denom + Num(1) / confra;
    auto kount = 1L;

    while (true)
    {
      ++kount;

      if (kount > maxit)
        throw std::logic_error("cont_frac: iteration failed to converge");

      //  Ref. 2, Eq. 25b
      mm = -mm;
      kk += 2;
      ak = Num(mm * kk) * zinv;
      //  Ref. 2, Eq. 32
      if (std::abs(numer / ak) <= eps1
       || std::abs(denom / ak) <= eps1)
      {
        //  Ill-conditioned case -- stride two terms instead of one.

        //  Ref. 2, Eqs. 34
        auto ntn = ak * numer + Num(1);
        auto dtd = ak * denom + Num(1);
        confra *= ntn / dtd;
        //  Ref. 2, Eq. 25b
        mm = -mm;
        kk += 2;
        ak = Num(mm * kk) * zinv;
        //  Ref. 2, Eqs. 35
        numer = ak + numer / ntn;
        denom = ak + denom / dtd;
        ++kount;
        continue;
      }
      else
      {
        //  Well-conditioned case...

        //  Ref. 2, Eqs. 26, 27
        auto apt = numer / denom;
        confra *= apt;
        //  Check for convergence (Ref. 2, Eq. 31)
        if (std::abs(std::real(apt) - Num(1)) >= eps2
         || std::abs(std::imag(apt)) >= eps2)
        {
          //  Ref. 2, Eqs. 30A-B
          numer = ak + Num(1) / numer;
          denom = ak + Num(1) / denom;
          continue;
        }

        break;
      }
    }

    return confra;
  }
