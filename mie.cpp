#include <vector>
#include <complex>
#include <iostream>

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
  std::vector<double> dqv2{dqv};
  if (dqv.end() == std::find(dqv.begin(), dqv.end(), -1.0))
    dqv2.push_back(-1.0);
  std::vector<double> ph(dqv.size());
  xs1.resize(dqv.size(), std::vector<std::complex<double>>(dx.size()));
  xs2.resize(dqv.size(), std::vector<std::complex<double>>(dx.size()));
  dph.resize(dqv.size(), std::vector<double>(dx.size()));

  dqxt.resize(dx.size());
  dqsc.resize(dx.size());
  dqbk.resize(dx.size());
  dg.resize(dx.size());

  for (auto Size = 0L; Size < Dx.size(); ++Size)
  {
    if (dx[size] > imaxx)
      std::cerr << "\n  **  Error: Size Parameter Overflow in Mie\n";
    auto ir = 1.0 / cm;
    auto y =  dx[size] * cm;

    if (dx(size) < 0.02)
      nstop = 2;
    else if (dx(size) <= 8.0)
      nstop = dx(size) + 4.00 * std::pow(dx(size), 1.0/3.0) + 2.0;
    else if (dx(size) < 4200.0)
      nstop = dx(size) + 4.05 * std::pow(dx(size), 1.0/3.0) + 2.0;
    else
      nstop = dx(size) + 4.00 * std::pow(dx(size), 1.0/3.0) + 2.0;

    auto nmx = long(std::max([nstop, std::abs(y)]) + 15.0);
    d = dcomplexarr(nmx + 1);

    for (n = nmx - 1, 1, -1)
    {
      a1 = (n + 1) / y;
      d[n] = a1 - 1 / (a1 + d[n + 1]);
    }

    sm = dcomplexarr(dqv2.size())
    sp = dcomplexarr(dqv2.size())
    pi0 = dcomplexarr(dqv2.size())
    pi1 = dcomplex(replicate(1.0,dqv2.size()),replicate(0.0,dqv2.size()))

    auto psi0 =  std::cos(dx(size));
    auto psi1 =  std::sin(dx(size));
    auto chi0 = -std::sin(dx(size));
    auto chi1 =  std::cos(dx(size));
    std::complex<double> xi0{psi0, chi0};
    std::complex<double> xi1{psi1, chi1};

    tnp1 = 1.0;

    for (auto n = 1l; n <= nstop; ++n)
    {
      dn = double(n);
      tnp1 = tnp1 + 2.0;
      tnm1 = tnp1 - 2.0;
      a2 = tnp1 / (dn * (dn + 1.0));
      turbo = (dn + 1.0) / dn;
      rnx = dn / dx[size];
      psi = double(tnm1) * psi1 / dx[size] - psi0;
      chi = tnm1 * chi1 / dx[size] - chi0;
      std::complex<double> xi{psi, chi};
      a = ((d[n] * ir + rnx) * psi - psi1) / ((d[n] * ir + rnx) * xi - xi1);
      b = ((d[n] * cm + rnx) * psi - psi1) / ((d[n] * cm + rnx) * xi - xi1);
      dqxt[size] = tnp1 * double(a + b)                 + dqxt[size];
      dqsc[size] = tnp1 * double(a*conj(a) + b*conj(b)) + dqsc[size];
      if (n > 1)
        dg[size] = dg[size]
                 + (dn * dn - 1.0) * double(anm1 * conj(a) + bnm1 * conj(b)) / dn
                 + tnm1 * double(anm1 * conj(bnm1)) / (dn * dn - dn);
      anm1 = a;
      bnm1 = b;


      s = dqv2 * pi1;
      t = s - pi0;
      if (dph.size() > 0 )
      {
        taun = n * t - pi0;
        sp = (a2 * (a + b)) * (pi1 + taun) + sp;
        sm = (a2 * (a - b)) * (pi1 - taun) + sm;
      }
      pi0 = pi1;
      pi1 = s + t * turbo;

      psi0 = psi1;
      psi1 = psi;
      chi0 = chi1;
      chi1 = chi;
      xi1 = dcomplex{psi1, chi1};

    }

    if (dg[size] > 0.0)
      dg[size] = 2.0 * dg[size] / dqsc[size];

    //  the following lines are not needed unless dqv was set
    if (dph.size() > 0)
    {
      xs1(*,size) = ((sp[0:dqv.size()-1] + sm[0:dqv.size()-1]) / 2.0)
      xs2(*,size) = ((sp[0:dqv.size()-1] - sm[0:dqv.size()-1]) / 2.0)
      dph(*,size) = double(xs1(*,size) * conj(xs1(*,size))
                         + xs2(*,size) * conj(xs2(*,size)))
                          / dqsc[size];
    }

    dqbk[size] = (sp[dqv2.size() - 1] + sm[dqv2.size() - 1]) / 2.0 + dqbk[size];
  }

  dqsc =  2.0 * dqsc / (dx * dx);
  dqxt =  2.0 * dqxt / (dx * dx);
  dqbk =  double(dqbk * conj(dqbk)) / (dx * dx) / dpi;
}
