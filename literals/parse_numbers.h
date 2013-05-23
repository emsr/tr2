#ifndef PARSE_NUMBERS_H_
#define PARSE_NUMBERS_H_

#include <cstddef>

// Stolen from n3468.pdf except I added binary literal prefixes and digit separator (`).

namespace std {

#if __cplusplus > 201103L

namespace __parse_int {

  template<unsigned _Base, char... _Digits>
    struct parse_int
    {
      static_assert(_Base <= 16U, "only support up to hexadecimal");
      static_assert(!sizeof...(_Digits), "invalid integral constant");
      static constexpr unsigned long long value = 0;
    };

  template<char... _Digits>
    struct _Base_dispatch;

  template<char... _Digits>
    struct _Base_dispatch<'0', 'b', _Digits...>
    {
      static constexpr unsigned long long value{parse_int<2U, _Digits...>::value};
    };

  template<char... _Digits>
    struct _Base_dispatch<'0', 'B', _Digits...>
    {
      static constexpr unsigned long long value{parse_int<2U, _Digits...>::value};
    };

  template<char... _Digits>
    struct _Base_dispatch<'0', 'x', _Digits...>
    {
      static constexpr unsigned long long value{parse_int<16U, _Digits...>::value};
    };

  template<char... _Digits>
    struct _Base_dispatch<'0', 'X', _Digits...>
    {
      static constexpr unsigned long long value{parse_int<16U, _Digits...>::value};
    };

  template<char... _Digits>
    struct _Base_dispatch<'0', _Digits...>
    {
      static constexpr unsigned long long value{parse_int<8U, _Digits...>::value};
    };

  template<char... _Digits>
    struct _Base_dispatch
    {
      static constexpr unsigned long long value{parse_int<10U, _Digits...>::value};
    };


  constexpr unsigned long long
  pow(unsigned _Base, size_t __p)
  {
    return __p ? (__p%2 ? _Base : 1) * pow(_Base, __p/2) * pow(_Base, __p/2) : 1;
  }


  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '0', _Digits...>
    {
      static constexpr unsigned long long
      value{parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '1', _Digits...>
    {
      static constexpr unsigned long long
      value{1 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '2', _Digits...>
    {
      static_assert(_Base > 2, "invalid digit");
      static constexpr unsigned long long
      value{2 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '3', _Digits...>
    {
      static_assert(_Base > 3, "invalid digit");
      static constexpr unsigned long long
      value{3 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '4', _Digits...>
    {
      static_assert(_Base > 4, "invalid digit");
      static constexpr unsigned long long
      value{4 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '5', _Digits...>
    {
      static_assert(_Base > 5, "invalid digit");
      static constexpr unsigned long long
      value{5 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '6', _Digits...>
    {
      static_assert(_Base > 6, "invalid digit");
      static constexpr unsigned long long
      value{6 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '7', _Digits...>
    {
      static_assert(_Base > 7, "invalid digit");
      static constexpr unsigned long long
      value{7 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '8', _Digits...>
    {
      static_assert(_Base > 8, "invalid digit");
      static constexpr unsigned long long
      value{8 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, '9', _Digits...>
    {
      static_assert(_Base > 9, "invalid digit");
      static constexpr unsigned long long
      value{9 * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'a', _Digits...>
    {
      static_assert(_Base > 0xa, "invalid digit");
      static constexpr unsigned long long
      value{0xa * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'A', _Digits...>
    {
      static_assert(_Base > 0xa, "invalid digit");
      static constexpr unsigned long long
      value{0xa * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'b', _Digits...>
    {
      static_assert(_Base > 0xb, "invalid digit");
      static constexpr unsigned long long
      value{0xb * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'B', _Digits...>
    {
      static_assert(_Base > 0xb, "invalid digit");
      static constexpr unsigned long long
      value{0xb * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'c', _Digits...>
    {
      static_assert(_Base > 0xc, "invalid digit");
      static constexpr unsigned long long
      value{0xc * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'C', _Digits...>
    {
      static_assert(_Base > 0xc, "invalid digit");
      static constexpr unsigned long long
      value{0xc * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'd', _Digits...>
    {
      static_assert(_Base > 0xd, "invalid digit");
      static constexpr unsigned long long
      value{0xd * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'D', _Digits...>
    {
      static_assert(_Base > 0xd, "invalid digit");
      static constexpr unsigned long long
      value{0xd * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'e', _Digits...>
    {
      static_assert(_Base > 0xe, "invalid digit");
      static constexpr unsigned long long
      value{0xe * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'E', _Digits...>
    {
      static_assert(_Base > 0xe, "invalid digit");
      static constexpr unsigned long long
      value{0xe * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'f', _Digits...>
    {
      static_assert(_Base > 0xf, "invalid digit");
      static constexpr unsigned long long
      value{0xf * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

  template<unsigned _Base, char... _Digits>
    struct parse_int<_Base, 'F', _Digits...>
    {
      static_assert(_Base > 0xf, "invalid digit");
      static constexpr unsigned long long
      value{0xf * pow(_Base, sizeof...(_Digits)) + parse_int<_Base, _Digits...>::value};
    };

} // __parse_int


namespace __select_int_type {

  template<unsigned long long _Val, typename... _Ints>
    struct _Select_int_type;

  template<unsigned long long _Val, typename _IntType, typename... _Ints>
    struct _Select_int_type<_Val, _IntType, _Ints...>
         : integral_constant<typename conditional<
          _Val <= static_cast<unsigned long long>(std::numeric_limits<_IntType>::max()),
          _IntType,
          typename _Select_int_type<_Val, _Ints...>::value_type >::type, _Val>
    { };

  template<unsigned long long _Val>
    struct _Select_int_type<_Val> : integral_constant<unsigned long long, _Val>
    { };

} // __select_int_type

#endif // __cplusplus > 201103L

} // std

#endif // PARSE_NUMBERS_H_
