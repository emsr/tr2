

#include <limits>
#include <type_traits>
#include <cmath>
#include <tuple>
#include <algorithm> // minmax


  template<typename _Tp>
    class decibel
    {
    public:

      using value_type = _Tp;

      static constexpr std::size_t max_bel = std::numeric_limits<_Tp>::max_exponent10 - 1;

      explicit constexpr operator value_type() const { return m_db; }

      constexpr explicit decibel()
      : m_db{-value_type{10} * max_bel}
      { }

      constexpr explicit decibel(value_type x)
      : m_db{x}
      { }

    private:

      value_type m_db;
    };

  template<typename _Tp, typename _Up>
    bool
    operator==(decibel<_Tp> a, decibel<_Up> b)
    { return a == b; }

  template<typename _Tp, typename _Up>
    bool
    operator!=(decibel<_Tp> a, decibel<_Up> b)
    { return !(a == b); }

  template<typename _Tp, typename _Up>
    bool
    operator<(decibel<_Tp> a, decibel<_Up> b)
    { return a < b; }

  template<typename _Tp, typename _Up>
    bool
    operator>=(decibel<_Tp> a, decibel<_Up> b)
    { return !(a < b); }

  template<typename _Tp, typename _Up>
    bool
    operator>(decibel<_Tp> a, decibel<_Up> b)
    { return b < a; }

  template<typename _Tp, typename _Up>
    bool
    operator<=(decibel<_Tp> a, decibel<_Up> b)
    { return !(b < a); }

  template<typename _Tp, typename _Up>
    decibel<std::common_type_t<_Tp, _Up>>
    operator+(decibel<_Tp> a, decibel<_Up> b)
    {
      std::tie(a, b) = std::minmax(a, b);

      using _Vp = std::common_type_t<_Tp, _Up>;
      if (b - a < _Vp{10} * decibel<_Vp>::max_bel)
        return b + _Vp{10} * std::log10(1.0 + std::pow(_Vp{10}, ((a - b) / _Vp{10})));
      else
        return b;
    }

  template<typename _Tp, typename _Up>
    decibel<std::common_type_t<_Tp, _Up>>
    operator-(decibel<_Tp> a, decibel<_Up> b)
    {
      std::tie(a, b) = std::minmax(a, b);

      using _Vp = std::common_type_t<_Tp, _Up>;
      if (b - a < _Vp{10} * decibel<_Vp>::max_bel)
        return b + _Vp{10} * std::log10(1.0 - std::pow(_Vp{10}, ((a - b) / _Vp{10})));
      else
        return b;
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
    operator""_dBf(unsigned long long n)
    { return decibel<float>(static_cast<float>(n)); }

    constexpr decibel<double>
    operator""_dB(unsigned long long n)
    { return decibel<double>(static_cast<double>(n)); }

    constexpr decibel<long double>
    operator""_dBl(unsigned long long n)
    { return decibel<long double>(static_cast<long double>(n)); }

    constexpr decibel<float>
    operator""_dBf(long double x)
    { return decibel<float>(static_cast<float>(x)); }

    constexpr decibel<double>
    operator""_dB(long double x)
    { return decibel<double>(static_cast<double>(x)); }

    constexpr decibel<long double>
    operator""_dBl(long double x)
    { return decibel<long double>(static_cast<long double>(x)); }
  }

#include "decibel.tcc"
