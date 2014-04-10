#ifndef NAKAGAMIFADINGMODEL
#define NAKAGAMIFADINGMODEL 1

#include <memory>

///
///  @brief  This class models Nakagami fading channel statistics.
///
///  The Nakagami fading model is a stochastic interference model which
///  considers multipath effects as arising from several sets if signals,
///  possibly arising from several transmitters or a single transmitter
///  with several strong reflection signals which are subsequently scattered
///  in higher-order reflections and diffractions.
///
///  The Nakagami fading model is parameterized by the shape parameter
///  or fading figure, m, which is dimensionless, and by the scale parameter,
///  omega, in a power unit such as mW.  The returned instantaneous signal
///  strength will be in this same unit.
///
///  If the Nakagami shape parameter m is set to 1 the Rayleigh fading
///  model is recovered.
///
///  The formula for the Nakagami model probability density function is
///  @f[
///  	 p(x;m,\omega) = \frac{2m^m}{\Gamma(m)\omega^m}
///                      x^{2m-1}e^{-m x / \omega}
///  @f]
///  where @f$\Gamma(z)@f$ is the gamma function and @f$m >= 0.5@f$
///  and @f$\omega > 0@f$.  Here x is the amplitude so @f$x^2@f$ is the power.
///
class NakagamiFadingModel
{

public:

  ///  @brief  Create a Nakagami fading model.
  ///  @param  m  The Nakagami shape parameter or fading figure.
  ///  @param  omega  The Nakagami scale parameter in dBp where 'p' is some power unit.
  NakagamiFadingModel(double m, double omega);
  ///  Nakagami fading model destructor.
  ~NakagamiFadingModel();

  ///  Return the Nakagami shape parameter or fading figure.
  double m() const;
  ///  Return the Nakagami scale parameter in dBp where 'p' is some power unit.
  double omega() const;

  ///  Return an instantaneous total signal power in dBp where 'p' is the same power unit as omega.
  double operator()();

private:

  class Impl;
  std::unique_ptr<Impl> _M_impl;
};

#endif // NAKAGAMIFADINGMODEL
