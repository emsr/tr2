#ifndef RICEFADINGMODEL
#define RICEFADINGMODEL 1

#include <memory>

///
///  @brief  This class models Rice fading channel statistics.
///
///  The Rice fading model is a stochastic interference model which
///  considers multipath effects as arising from one dominant signal,
///  usually a line-of-sight path, interfering with other signals
///  representing scattered propagation paths.
///
///  The Rayleigh fading model is a special case of the Rice fading model
///  where there is no dominant signal.  The Rayleigh fading model is constructed
///  from the Rice fading model by setting the signal to noise ratio,
///  or Rice parameter K, to zero.
///
///  The Rice fading model is parameterized by K, the ratio of the dominant signal
///  strength to the mean of the strength of the other signals - the signal to
///  interference ratio - in dB, and by A the power in the combined dominant
///  and scattered signals in a decibel power unit such as dBm.  The returned
///  instantaneous signal strength will be in this same unit.
///
///  The formula for the Rice probability density function is
///  @f[
///      p(x;K,A) = \frac{2(1+K)x}{A}e^{-K}
///                        \exp\left(-\frac{1+K}{A}x^2\right)
///                        I_0\left(2\sqrt{\frac{K(1+K)}{A}}x\right)
///  @f]
///  where @f$I_0(z)@f$ is the modified Bessel function of the first kind
///  of order 0 and @f$K >= 0@f$ and @f$A > 0@f$.  Here x is the amplitude
///  so @f$x^2@f$ is the power.
///
class RiceFadingModel
{

public:

  ///  @brief  Create a Rice fading model.
  ///  @param  K  The Rice fading parameter in dB.
  ///  @param  A  The mean total power in dBp where 'p' is some power unit.
  RiceFadingModel(double K, double A);
  ///  Rice fading model destructor.
  ~RiceFadingModel();

  ///  Return the Rice fading parameter in dB.
  double K() const;
  ///  Return the mean total signal power in dBp where 'p' is some power unit.
  double A() const;

  ///  Return an instantaneous total power in dBp where 'p' is the same power unit as A.
  double operator()();

private:

  class Impl;
  std::unique_ptr<Impl> _M_impl;
};

#endif // RICEFADINGMODEL
