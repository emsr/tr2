#ifndef MIE_H
#define MIE_H 1L

#include <vector>
#include <complex>

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
      std::vector<Num> & polarization);

#include "mie.tpp"

#endif
