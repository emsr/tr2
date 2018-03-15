
#include <cmath>
#include <chrono>
#include <functional> // For bind.
#include <ext/random>

#include "RiceFadingModel.h"

namespace
{

///
///  @brief  Return the system time in microseconds.
///          This is used as a seed for a random number engine.
///
unsigned long
system_now()
{
  std::chrono::time_point<std::chrono::system_clock> now{std::chrono::system_clock::now()};
  std::chrono::system_clock::duration epoch{now.time_since_epoch()};
  return static_cast<unsigned long>(now.time_since_epoch().count());
}

}

///
///  @brief  Implementation class for the Rice fading model.
///
class RiceFadingModel::Impl
{

public:

  ///  @brief  Create a Rice fading model implementation.
  ///  @param  K  The Rice fading parameter in dB.
  ///  @param  A  Return the mean total power in dBp where 'p' is some power unit.
  Impl(double K, double A)
  : _M_K{std::pow(10.0, K / 10.0)},
    _M_A{std::pow(10.0, A / 10.0)},
    _M_nu{std::sqrt(_M_K * _M_A / (_M_K + 1.0))},
    _M_sigma{std::sqrt(_M_A / (2.0 * (_M_K + 1.0)))},
    _M_re{system_now()},
    _M_rd{_M_nu, _M_sigma},
    _M_gen{std::bind(_M_rd, _M_re)}
  { }

  ///  Return the Rice fading parameter in dB.
  double
  K() const
  { return 20.0 * (std::log10(_M_nu) - std::log10(_M_sigma)) - 10.0 * log10(2.0); }

  ///  Return the mean total signal power in dBp where 'p' is some power uni.
  double
  A() const
  { return 10.0 * std::log10(_M_nu * _M_nu + 2.0 * _M_sigma * _M_sigma); }

  ///  Return an instantaneous total signal power in dBp where 'p' is the same power unit as A.
  double
  operator()()
  { return 10.0 * std::log10(_M_gen()); }

private:

  ///  The dimensionless Rice fading parameter.
  double _M_K;
  ///  The mean total signal power in power units.
  double _M_A;
  ///  The shape parameter of the underlying Rice distribution.
  double _M_nu;
  ///  The scale parameter of the underlying Rice distribution.
  double _M_sigma;
  ///  The pseudo-random number engine - the library default.
  std::default_random_engine _M_re;
  ///  The underlying Rice distribution.
  __gnu_cxx::rice_distribution<double> _M_rd;
  ///  The variate generator built from the pseudo-random number engine and the Rice distribution.
  decltype(std::bind(_M_rd, _M_re)) _M_gen;
};

///  @brief  Create a Rice fading model.
///  @param  K  The Rice fading parameter in dB.
///  @param  A  The mean total power in dBp where 'p' is some power unit.
RiceFadingModel::RiceFadingModel(double K, double A)
: _M_impl{new RiceFadingModel::Impl{K, A}}
{ }

///  Default destructor.
RiceFadingModel::~RiceFadingModel() = default;

///  Return the Rice fading parameter in dB.
double
RiceFadingModel::K() const
{ return _M_impl.get()->K(); }

///  Return the mean total power in dBp where 'p' is some power unit.
double
RiceFadingModel::A() const
{ return _M_impl.get()->A(); }

///  Return an instantaneous total power in dBp where 'p' is the same power unit as A.
double
RiceFadingModel::operator()()
{ return _M_impl.get()->operator()(); }
