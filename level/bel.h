#ifndef _LEVEL_H
#define _LEVEL_H 1


#include <experimental/ratio>
#include <limits>
#include <type_traits>
#include <cmath>
#include <tuple>
#include <algorithm> // minmax


#define CXX14CONSTEXPR


  template<typename _Tp, typename _Unit = std::ratio<1>>
    class level
    {
      static_assert(std::experimental::ratio_not_equal_v<_Unit, std::ratio<0>>
                 && (_Unit::num == 1 && (_Unit::den == 1 || _Unit::den % 10 == 0)
                  || _Unit::num % 10 == 0 && _Unit::den == 1), "Expected power of 10.");
    public:

      using value_type = _Tp;

      static constexpr value_type max_level = std::numeric_limits<_Tp>::max_exponent10 - 1;
      static constexpr value_type factor = value_type(_Unit::den) / value_type(_Unit::num);
      static constexpr value_type max_value = factor * max_level;

      explicit constexpr
      operator value_type() const
      { return this->_M_val; }

      constexpr explicit
      level()
      : _M_val{-max_value}
      { }

      constexpr explicit
      level(value_type x)
      : _M_val{x <= max_value ? x : max_value}
      { }

      constexpr
      level(const level &) = default;

      constexpr
      level(level &&) = default;

      template<typename _Up, typename _Vnit>
        constexpr
        level(level<_Up, _Vnit> b)
        {
          using _UVinv = std::ratio_divide<_Unit, _Vnit>;
          this->_M_val = _UVinv::den * static_cast<value_type>(_Up(b)) / _UVinv::num;
        }

      constexpr level &
      operator=(const level &) = default;

      constexpr level &
      operator=(level &&) = default;

      template<typename _Up, typename _Vnit>
        CXX14CONSTEXPR level &
        operator+=(level<_Up, _Vnit> b) noexcept
        {
          value_type ma{}, mb{};
          std::tie(ma, mb) = std::minmax(this->_M_val, value_type(level(b)));

          this->_M_val = mb;
          if (mb - ma < max_value)
            this->_M_val += factor * std::log10(value_type{1} + std::pow(value_type{10}, (ma - mb) / factor));

          return *this;
        }

      template<typename _Up, typename _Vnit>
        CXX14CONSTEXPR level &
        operator-=(level<_Up, _Vnit> b) noexcept
        {
          value_type ma{}, mb{};
          std::tie(ma, mb) = std::minmax(this->_M_val, value_type(level(b)));

          this->_M_val = mb;
          if (mb - ma < max_value)
            this->_M_val += factor * std::log10(value_type{1} - std::pow(value_type{10}, (ma - mb) / factor));

          return *this;
        }

      constexpr value_type
      root_power() const noexcept
      { return std::pow(value_type(10), this->_M_val / (2 * factor)); }

      constexpr value_type
      power() const noexcept
      { return std::pow(value_type(10), this->_M_val / factor); }

    private:

      value_type _M_val;
    };

  template<typename _Tp>
    using bel = level<_Tp, std::ratio<1L,1L>>;

  template<typename _Tp>
    using decibel = level<_Tp, std::deci>;

  template<typename _Tp>
    using centibel = level<_Tp, std::centi>;

  template<typename _Tp>
    using millibel = level<_Tp, std::milli>;

  template<typename _Tp>
    constexpr decibel<_Tp>
    make_decibel(_Tp x) noexcept
    { return _Tp{10} * std::log10(x); }

  template<typename _Tp>
    constexpr centibel<_Tp>
    make_centibel(_Tp x) noexcept
    { return _Tp{100} * std::log10(x); }

  template<typename _Tp>
    constexpr millibel<_Tp>
    make_millibel(_Tp x) noexcept
    { return _Tp{1000} * std::log10(x); }

  template<typename _Tp, typename _Unit,
           typename _Up, typename _Vnit>
    constexpr bool
    operator==(level<_Tp, _Unit> a, level<_Up, _Vnit> b) noexcept
    { return _Tp(a) == _Up(b); }

  template<typename _Tp, typename _Unit,
           typename _Up, typename _Vnit>
    constexpr bool
    operator!=(level<_Tp, _Unit> a, level<_Up, _Vnit> b) noexcept
    { return !(a == b); }

  template<typename _Tp, typename _Unit,
           typename _Up, typename _Vnit>
    constexpr bool
    operator<(level<_Tp, _Unit> a, level<_Up, _Vnit> b) noexcept
    { return _Tp(a) < _Up(b); }

  template<typename _Tp, typename _Unit,
           typename _Up, typename _Vnit>
    constexpr bool
    operator>=(level<_Tp, _Unit> a, level<_Up, _Vnit> b) noexcept
    { return !(a < b); }

  template<typename _Tp, typename _Unit,
           typename _Up, typename _Vnit>
    constexpr bool
    operator>(level<_Tp, _Unit> a, level<_Up, _Vnit> b) noexcept
    { return b < a; }

  template<typename _Tp, typename _Unit,
           typename _Up, typename _Vnit>
    constexpr bool
    operator<=(level<_Tp, _Unit> a, level<_Up, _Vnit> b) noexcept
    { return !(b < a); }

  template<typename _Tp, typename _Unit>
    constexpr level<_Tp, _Unit>
    operator-(level<_Tp, _Unit> a)
    { return level<_Tp, _Unit>{-_Tp(a)}; }

  template<typename _Tp, typename _Unit>
    constexpr level<_Tp, _Unit>
    operator+(level<_Tp, _Unit> a) noexcept
    { return a; }

  template<typename _Unit, typename _Vnit,
           bool _Uless, bool _Vless>
    struct __common_unit_helper;

  template<typename _Unit, typename _Vnit>
    struct __common_unit_helper<_Unit, _Vnit, false, false> : public _Unit
    { };

  template<typename _Unit, typename _Vnit>
    struct __common_unit_helper<_Unit, _Vnit, true, false> : public _Unit
    { };

  template<typename _Unit, typename _Vnit>
    struct __common_unit_helper<_Unit, _Vnit, false, true> : public _Vnit
    { };

  template<typename _Unit, typename _Vnit>
    struct __common_unit : public __common_unit_helper<_Unit, _Vnit,
                                      std::experimental::ratio_less_v<_Unit, _Vnit>,
                                      std::experimental::ratio_less_v<_Vnit, _Unit>>
    { };

  template<typename _Tp, typename _Unit,
           typename _Up, typename _Vnit>
    CXX14CONSTEXPR level<std::common_type_t<_Tp, _Up>, __common_unit<_Unit, _Vnit>>
    operator+(level<_Tp, _Unit> a, level<_Up, _Vnit> b) noexcept
    {
      using _Vp = std::common_type_t<_Tp, _Up>;
      using _Wnit = __common_unit<_Unit, _Vnit>;
      using _NLevel = level<_Vp, _Wnit>;

      return _NLevel(a) += _NLevel(b);
    }

  template<typename _Tp, typename _Unit, typename _Up, typename _Vnit>
    CXX14CONSTEXPR level<std::common_type_t<_Tp, _Up>, __common_unit<_Unit, _Vnit>>
    operator-(level<_Tp, _Unit> a, level<_Up, _Vnit> b) noexcept
    {
      using _Vp = std::common_type_t<_Tp, _Up>;
      using _Wnit = __common_unit<_Unit, _Vnit>;
      using _NLevel = level<_Vp, _Wnit>;

      return _NLevel(a) -= _NLevel(b);
    }

  template<typename _Tp, typename _Unit,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits> &
    operator<<(std::basic_ostream<_CharT, _Traits> &,
	       const level<_Tp, _Unit> &);

  template<typename _Tp, typename _Unit,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits> &
    operator>>(std::basic_istream<_CharT, _Traits> &,
	       level<_Tp, _Unit> &);

  namespace decibel_literals
  {
    constexpr decibel<float>
    operator""_dBf(unsigned long long n) noexcept
    { return decibel<float>(static_cast<float>(n)); }

    constexpr decibel<double>
    operator""_dB(unsigned long long n) noexcept
    { return decibel<double>(static_cast<double>(n)); }

    constexpr decibel<long double>
    operator""_dBl(unsigned long long n) noexcept
    { return decibel<long double>(static_cast<long double>(n)); }

    constexpr decibel<float>
    operator""_dBf(long double x) noexcept
    { return decibel<float>(static_cast<float>(x)); }

    constexpr decibel<double>
    operator""_dB(long double x) noexcept
    { return decibel<double>(static_cast<double>(x)); }

    constexpr decibel<long double>
    operator""_dBl(long double x) noexcept
    { return decibel<long double>(static_cast<long double>(x)); }
  }


  template<typename _Tp>
    class neper
    {
    public:
      using value_type = _Tp;

      static constexpr value_type max_neper = std::log(value_type{10})
                                            * std::numeric_limits<_Tp>::max_exponent10 - 1;
      static constexpr value_type factor = std::exp(value_type{1});
      static constexpr value_type max_value = factor * max_neper;

      explicit constexpr
      operator value_type() const
      { return this->_M_val; }

      constexpr explicit
      neper()
      : _M_val{-max_value}
      { }

      constexpr explicit
      neper(value_type x)
      : _M_val{x <= max_value ? x : max_value}
      { }

      constexpr
      neper(const neper &) = default;

      constexpr
      neper(neper &&) = default;

      template<typename _Up, typename _Vnit>
        constexpr explicit
        neper(level<_Up, _Vnit> b)
        {
          this->_M_val = std::log(value_type{10})
                       * static_cast<value_type>(_Up(b) / (2 * (level<_Up, _Vnit>::factor)));
        }

      constexpr neper &
      operator=(const neper &) = default;

      constexpr neper &
      operator=(neper &&) = default;

      template<typename _Up>
        CXX14CONSTEXPR neper &
        operator+=(neper<_Up> b) noexcept
        {
          value_type ma{}, mb{};
          std::tie(ma, mb) = std::minmax(this->_M_val, value_type(neper(b)));

          if (mb - ma < max_value)
            this->_M_val = mb + std::log1p(std::exp(ma - mb));
          else
            this->_M_val = +max_value;

          return *this;
        }

      template<typename _Up>
        CXX14CONSTEXPR neper &
        operator-=(neper<_Up> b) noexcept
        {
          value_type ma{}, mb{};
          std::tie(ma, mb) = std::minmax(this->_M_val, value_type(neper(b)));

          if (mb - ma < std::numeric_limits<value_type>::epsilon())
            this->_M_val = -max_value;
          else if (mb - ma < max_value)
            this->_M_val = mb + std::log1p(-std::exp(ma - mb));

          return *this;
        }

      constexpr value_type
      root_power() const noexcept
      { return std::exp(this->_M_val); }

      constexpr value_type
      power() const noexcept
      { return std::exp(2 * this->_M_val); }

    private:

      value_type _M_val;
    };

  template<typename _Tp, typename _Up>
    CXX14CONSTEXPR neper<std::common_type_t<_Tp, _Up>>
    operator+(neper<_Tp> a, neper<_Up> b) noexcept
    {
      using _Vp = std::common_type_t<_Tp, _Up>;
      using _Nep = neper<_Vp>;

      return _Nep(a) += _Nep(b);
    }

  template<typename _Tp, typename _Up>
    CXX14CONSTEXPR neper<std::common_type_t<_Tp, _Up>>
    operator-(neper<_Tp> a, neper<_Up> b) noexcept
    {
      using _Vp = std::common_type_t<_Tp, _Up>;
      using _Nep = neper<_Vp>;

      return _Nep(a) -= _Nep(b);
    }

  template<typename _Tp, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits> &
    operator<<(std::basic_ostream<_CharT, _Traits> &,
	       const neper<_Tp> &);

  template<typename _Tp, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits> &
    operator>>(std::basic_istream<_CharT, _Traits> &,
	       neper<_Tp> &);

  namespace neper_literals
  {
    constexpr neper<float>
    operator""_Npf(unsigned long long n) noexcept
    { return neper<float>(static_cast<float>(n)); }

    constexpr neper<double>
    operator""_Np(unsigned long long n) noexcept
    { return neper<double>(static_cast<double>(n)); }

    constexpr neper<long double>
    operator""_Npl(unsigned long long n) noexcept
    { return neper<long double>(static_cast<long double>(n)); }

    constexpr neper<float>
    operator""_Npf(long double x) noexcept
    { return neper<float>(static_cast<float>(x)); }

    constexpr neper<double>
    operator""_Np(long double x) noexcept
    { return neper<double>(static_cast<double>(x)); }

    constexpr neper<long double>
    operator""_Npl(long double x) noexcept
    { return neper<long double>(static_cast<long double>(x)); }
  }


#include "bel.tcc"


#endif // _LEVEL_H
