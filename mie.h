#ifndef MIE_H
#define MIE_H 1L

#include <vector>
#include <complex>

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
    std::vector<std::vector<double>> & phase);

#endif
