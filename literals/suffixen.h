#ifndef _SUFFIXEN_H
#define _SUFFIXEN_H

#pragma GCC system_header


// n3642.pdf by Peter Sommerlad


#include <string>
#include <complex>
#include <chrono>


#include "parse_numbers.h"


////#if __cplusplus > 201103L

namespace std
{

inline namespace literals {
inline namespace string_literals {

  inline basic_string<char>
  operator"" s(const char* __str, size_t __len)
  { return basic_string<char>{__str, __len}; }

  inline basic_string<wchar_t>
  operator"" s(const wchar_t* __str, size_t __len)
  { return basic_string<wchar_t>{__str, __len}; }

  inline basic_string<char16_t>
  operator"" s(const char16_t* __str, size_t __len)
  { return basic_string<char16_t>{__str, __len}; }

  inline basic_string<char32_t>
  operator"" s(const char32_t* __str, size_t __len)
  { return basic_string<char32_t>{__str, __len}; }

} // inline namespace string_literals
} // inline namespace literals

} // namespace std

////#endif // __cplusplus > 201103L


////#if __cplusplus > 201103L

namespace std
{

inline namespace literals {
inline namespace complex_literals {

  inline constexpr complex<float>
  operator"" i_f(long double __num)
  { return complex<float>{0.0F, static_cast<float>(__num)}; }

  inline constexpr complex<float>
  operator"" i_f(unsigned long long __num)
  { return complex<float>{0.0F, static_cast<float>(__num)}; }

  inline constexpr complex<double>
  operator"" i(long double __num)
  { return complex<double>{0.0, static_cast<double>(__num)}; }

  inline constexpr complex<double>
  operator"" i(unsigned long long __num)
  { return complex<double>{0.0, static_cast<double>(__num)}; }

  inline constexpr complex<long double>
  operator"" il(long double __num)
  { return complex<long double>{0.0L, __num}; }

  inline constexpr complex<long double>
  operator"" il(unsigned long long __num)
  { return complex<long double>{0.0L, static_cast<long double>(__num)}; }

} // inline namespace complex_literals
} // inline namespace literals

} // namespace std

////#endif // __cplusplus > 201103L


////#if __cplusplus > 201103L

namespace std
{

inline namespace literals {
inline namespace chrono_literals {

  namespace __detail {

    using namespace __parse_int;

    template<unsigned long long _Val, typename _Dur>
      struct _Select_type
      : conditional<
	  _Val <= static_cast<unsigned long long>
		    (numeric_limits<typename _Dur::rep>::max()),
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
    __detail::_Select_type<__select_int::_Select_int<_Digits...>::value,
			   chrono::hours>::type
    operator"" h()
    {
      return __detail::_Select_type<
			__select_int::_Select_int<_Digits...>::value,
			chrono::hours>::value;
    }

  inline constexpr chrono::duration<long double, ratio<60,1>>
  operator"" min(long double __mins)
  { return chrono::duration<long double, ratio<60,1>>{__mins}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<__select_int::_Select_int<_Digits...>::value,
			   chrono::minutes>::type
    operator"" min()
    {
      return __detail::_Select_type<
			__select_int::_Select_int<_Digits...>::value,
			chrono::minutes>::value;
    }

  inline constexpr chrono::duration<long double>
  operator"" s(long double __secs)
  { return chrono::duration<long double>{__secs}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<__select_int::_Select_int<_Digits...>::value,
			   chrono::seconds>::type
    operator"" s()
    {
      return __detail::_Select_type<
			__select_int::_Select_int<_Digits...>::value,
			chrono::seconds>::value;
    }

  inline constexpr chrono::duration<long double, milli>
  operator"" ms(long double __msecs)
  { return chrono::duration<long double, milli>{__msecs}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<__select_int::_Select_int<_Digits...>::value,
			   chrono::milliseconds>::type
    operator"" ms()
    {
      return __detail::_Select_type<
			__select_int::_Select_int<_Digits...>::value,
			chrono::milliseconds>::value;
    }

  inline constexpr chrono::duration<long double, micro>
  operator"" us(long double __usecs)
  { return chrono::duration<long double, micro>{__usecs}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<__select_int::_Select_int<_Digits...>::value,
			   chrono::microseconds>::type
    operator"" us()
    {
      return __detail::_Select_type<
			__select_int::_Select_int<_Digits...>::value,
			chrono::microseconds>::value;
    }

  inline constexpr chrono::duration<long double, nano>
  operator"" ns(long double __nsecs)
  { return chrono::duration<long double, nano>{__nsecs}; }

  template <char... _Digits>
    inline constexpr typename
    __detail::_Select_type<__select_int::_Select_int<_Digits...>::value,
			   chrono::nanoseconds>::type
    operator"" ns()
    {
      return __detail::_Select_type<
			__select_int::_Select_int<_Digits...>::value,
			chrono::nanoseconds>::value;
    }

} // inline namespace chrono_literals
} // inline namespace literals

} // namespace std

////#endif // __cplusplus > 201103L


#endif // _SUFFIXEN_H
