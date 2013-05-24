#ifndef SUFFIXEN_H_
#define SUFFIXEN_H_

#pragma GCC system_header


// n3468.pdf by Peter Sommerlad


#include <string>
#include <complex>
#include <chrono>


#include "parse_numbers.h"


namespace std {

#if __cplusplus > 201103L

inline namespace literals {
inline namespace string_literals {

  inline std::basic_string<char>
  operator"" s(const char* __str, size_t __len)
  { return std::basic_string<char>{__str, __len}; }

  inline std::basic_string<wchar_t>
  operator"" s(const wchar_t* __str, size_t __len)
  { return std::basic_string<wchar_t>{__str, __len}; }

  inline std::basic_string<char16_t>
  operator"" s(const char16_t* __str, size_t __len)
  { return std::basic_string<char16_t>{__str, __len}; }

  inline std::basic_string<char32_t>
  operator"" s(const char32_t* __str, size_t __len)
  { return std::basic_string<char32_t>{__str, __len}; }

} // string_literals
} // literals

#endif // __cplusplus > 201103L

} // std


namespace std {

#if __cplusplus > 201103L

inline namespace literals {
inline namespace complex_literals {

  inline constexpr std::complex<float>
  operator"" i_f(long double __num)
  { return std::complex<float>{0.0F, static_cast<float>(__num)}; }

  inline constexpr std::complex<float>
  operator"" i_f(unsigned long long __num)
  { return std::complex<float>{0.0F, static_cast<float>(__num)}; }

  inline constexpr std::complex<double>
  operator"" i(long double __num)
  { return std::complex<double>{0.0, static_cast<double>(__num)}; }

  inline constexpr std::complex<double>
  operator"" i(unsigned long long __num)
  { return std::complex<double>{0.0, static_cast<double>(__num)}; }

  inline constexpr std::complex<long double>
  operator"" il(long double __num)
  { return std::complex<long double>{0.0L, __num}; }

  inline constexpr std::complex<long double>
  operator"" il(unsigned long long __num)
  { return std::complex<long double>{0.0L, static_cast<long double>(__num)}; }

} // complex_literals
} // literals

#endif // __cplusplus > 201103L

} // std


namespace std {

#if __cplusplus > 201103L

inline namespace literals {
inline namespace chrono_literals {

  namespace __detail {

    using namespace std::__parse_int;

    template<unsigned long long _Val, typename _Dur>
      struct _Select_type
      : std::conditional<
	  _Val <= static_cast<unsigned long long>
		    (std::numeric_limits<typename _Dur::rep>::max()),
	  _Dur, void>
      {
	static constexpr typename _Select_type::type
	  value{static_cast<typename _Select_type::type>(_Val)};
      };

    template<unsigned long long _Val, typename _Dur>
      constexpr typename _Select_type<_Val, _Dur>::type
      _Select_type<_Val, _Dur>::value;

  } // __detail

  inline constexpr chrono::duration<long double, ratio<3600,1>>
  operator"" h(long double __hours)
  { return chrono::duration<long double, ratio<3600,1>>{__hours}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
			   chrono::hours>::type
    operator"" h()
    {
      return __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
				    chrono::hours>::value;
    }

  inline constexpr chrono::duration<long double, ratio<60,1>>
  operator"" min(long double __mins)
  { return chrono::duration<long double, ratio<60,1>>{__mins}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
			   chrono::minutes>::type
    operator"" min()
    {
      return __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
				    chrono::minutes>::value;
    }

  inline constexpr chrono::duration<long double>
  operator"" s(long double __secs)
  { return chrono::duration<long double>{__secs}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
			   chrono::seconds>::type
    operator"" s()
    {
      return __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
				    chrono::seconds>::value;
    }

  inline constexpr chrono::duration<long double, milli>
  operator"" ms(long double __msecs)
  { return chrono::duration<long double, milli>{__msecs}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
			   chrono::milliseconds>::type
    operator"" ms()
    {
      return __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
				    chrono::milliseconds>::value;
    }

  inline constexpr chrono::duration<long double, micro>
  operator"" us(long double __usecs)
  { return chrono::duration<long double, micro>{__usecs}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
			   chrono::microseconds>::type
    operator"" us()
    {
      return __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
				    chrono::microseconds>::value;
    }

  inline constexpr chrono::duration<long double, nano>
  operator"" ns(long double __nsecs)
  { return chrono::duration<long double, nano>{__nsecs}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
			   chrono::nanoseconds>::type
    operator"" ns()
    {
      return __detail::_Select_type<std::__select_int_type::_Select_int_type<_Digits...>::value,
				    chrono::nanoseconds>::value;
    }

} // chrono_literals
} // literals

#endif // __cplusplus > 201103L

} // std


#endif // SUFFIXEN_H_
