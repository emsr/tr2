#ifndef _DECIBEL_H
#define _DECIBEL_H 1


#include <limits>
#include <type_traits>
#include <cmath>
#include <tuple>
#include <algorithm> // minmax


#define CXX14CONSTEXPR


  template<typename _Tp>
    class decibel
    {
    public:

      using value_type = _Tp;

      static constexpr value_type max_bel = std::numeric_limits<_Tp>::max_exponent10 - 1;
      static constexpr value_type max_decibel = value_type{10} * max_bel;

      explicit constexpr
      operator value_type() const
      { return this->_M_val; }

      constexpr explicit
      decibel()
      : _M_val{-max_decibel}
      { }

      constexpr explicit
      decibel(value_type x)
      : _M_val{x}
      { }

      template<typename _Up>
        CXX14CONSTEXPR decibel &
        operator+=(decibel<_Up> b) noexcept
        {
          value_type ma{}, mb{};
          std::tie(ma, mb) = std::minmax(this->_M_val, value_type(decibel(b)));
          value_type factor = value_type(10);

          this->_M_val = mb;
          if (mb - ma < factor * max_bel)
            this->_M_val += factor * std::log10(value_type{1} + std::pow(value_type{10}, (ma - mb) / factor));

          return *this;
        }

      template<typename _Up>
        CXX14CONSTEXPR decibel &
        operator-=(decibel<_Up> b) noexcept
        {
          value_type ma{}, mb{};
          std::tie(ma, mb) = std::minmax(this->_M_val, value_type(decibel(b)));
          value_type factor = value_type(10);

          this->_M_val = mb;
          if (mb - ma < factor * max_bel)
            this->_M_val += factor * std::log10(value_type{1} - std::pow(value_type{10}, (ma - mb) / factor));

          return *this;
        }

      constexpr value_type
      power() noexcept
      { return std::pow(value_type(10), this->_M_val / value_type(10)); }

    private:

      value_type _M_val;
    };

  template<typename _Tp>
    constexpr decibel<_Tp>
    make_decibel(_Tp x) noexcept
    { return _Tp{10} * std::log10(x); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator==(decibel<_Tp> a, decibel<_Up> b) noexcept
    { return _Tp(a) == _Up(b); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator!=(decibel<_Tp> a, decibel<_Up> b) noexcept
    { return !(a == b); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator<(decibel<_Tp> a, decibel<_Up> b) noexcept
    { return _Tp(a) < _Up(b); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator>=(decibel<_Tp> a, decibel<_Up> b) noexcept
    { return !(a < b); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator>(decibel<_Tp> a, decibel<_Up> b) noexcept
    { return b < a; }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator<=(decibel<_Tp> a, decibel<_Up> b) noexcept
    { return !(b < a); }

  template<typename _Tp>
    constexpr decibel<_Tp>
    operator-(decibel<_Tp> a)
    { return decibel<_Tp>{-_Tp(a)}; }

  template<typename _Tp>
    constexpr decibel<_Tp>
    operator+(decibel<_Tp> a) noexcept
    { return a; }

  template<typename _Tp, typename _Up>
    CXX14CONSTEXPR decibel<std::common_type_t<_Tp, _Up>>
    operator+(decibel<_Tp> a, decibel<_Up> b) noexcept
    {
      using _Vp = std::common_type_t<_Tp, _Up>;

      _Vp ma{}, mb{};
      std::tie(ma, mb) = std::minmax(_Tp(a), _Up(b));

      if (mb - ma < _Vp{10} * decibel<_Vp>::max_bel)
        return decibel<_Vp>(mb + _Vp{10} * std::log10(_Vp{1} + std::pow(_Vp{10}, (ma - mb) / _Vp{10})));
      else
        return decibel<_Vp>(mb);
    }

  template<typename _Tp, typename _Up>
    CXX14CONSTEXPR decibel<std::common_type_t<_Tp, _Up>>
    operator-(decibel<_Tp> a, decibel<_Up> b) noexcept
    {
      using _Vp = std::common_type_t<_Tp, _Up>;

      _Vp ma{}, mb{};
      std::tie(ma, mb) = std::minmax(_Tp(a), _Up(b));

      if (mb - ma < _Vp{10} * decibel<_Vp>::max_bel)
        return decibel<_Vp>(mb + _Vp{10} * std::log10(_Vp{1} - std::pow(_Vp{10}, (ma - mb) / _Vp{10})));
      else
        return decibel<_Vp>(mb);
    }

  template<typename _Tp,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>&,
	       const decibel<_Tp>&);

  template<typename _Tp,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>&,
	       decibel<_Tp>&);

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


#include "decibel.tcc"


#endif // _DECIBEL_H
