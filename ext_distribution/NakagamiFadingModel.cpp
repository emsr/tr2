
#include <chrono>
#include <functional> // For bind.
#include <ext/random>

#include "NakagamiFadingModel.h"

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
///  @brief  Implementation class for the Nakagami fading model.
///
class NakagamiFadingModel::Impl
{

public:

  ///  @brief  Create a Nakagami fading model implementation.
  ///  @param  m  The Nakagami shape parameter or fading figure.
  ///  @param  omega  The Nakagami shape parameter in dBp where 'p' is some power unit.
  Impl(double m, double omega)
  : _M_mu{m},
    _M_omega{std::pow(10.0, omega / 10.0)},
    _M_re{system_now()},
    _M_nakd{_M_mu, _M_omega},
    _M_gen{std::bind(_M_nakd, _M_re)}
  { }

  ///  Return the Nakagami shape parameter or fading figure.
  double
  mu() const
  { return _M_mu; }

  ///  Return the Nakagami scale parameter in dBp where 'p' is some power unit.
  double
  omega() const
  { return 10.0 * std::log10(_M_omega); }

  ///  Return an instantaneous total signal power in dBp where 'p' is the same power unit as omega.
  double
  operator()()
  { return 10.0 * std::log10(_M_gen()); }

private:

  ///  The Nakagami shape parameter or fading figure.
  double _M_mu;
  ///  The Nakagami scale parameter in power units.
  double _M_omega;
  ///  The pseudo-random number engine - the library default.
  std::default_random_engine _M_re;
  ///  The underlying Nakagami distribution.
  __gnu_cxx::nakagami_distribution<double> _M_nakd;
  ///  The variate generator built from the pseudo-random number engine and the Nakagami distribution.
  decltype(std::bind(_M_nakd, _M_re)) _M_gen;
};

///  @brief  Create a Nakagami fading model.
///  @param  m  The Nakagami shape parameter or fading figure.
///  @param  omega  The Nakagami scale parameter in dBp where 'p' is some power unit.
NakagamiFadingModel::NakagamiFadingModel(double m, double omega)
: _M_impl{new NakagamiFadingModel::Impl{m, omega}}
{ }

///  Default destructor.
NakagamiFadingModel::~NakagamiFadingModel() = default;

///  Return the Nakagami shape parameter or fading figure.
double
NakagamiFadingModel::m() const
{ return _M_impl.get()->mu(); }

///  Return the Nakagami scale parameter in dBp where 'p' is some power unit.
double
NakagamiFadingModel::omega() const
{ return _M_impl.get()->omega(); }

///  Return an instantaneous total signal power in dBp where 'p' is the same power unit as omega.
double
NakagamiFadingModel::operator()()
{ return _M_impl.get()->operator()(); }
