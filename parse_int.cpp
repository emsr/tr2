
// /home/ed/bin/bin/g++ -std=c++1y -o parse_int parse_int.cpp

#include <vector> // Get parse_numbers.h to work.
#include "parse_numbers.h"
#include <cassert>

namespace __gnu_cxx
{
template<char... Digits>
  using __parse_int = std::__parse_int::_Parse_int<Digits...>;

template<char... Digits>
  using __select_int_type = std::__select_int::_Select_int<Digits...>;
}

template<char... Digits>
  constexpr int
  operator""_int()
  { return std::__parse_int::_Parse_int<Digits...>::value; }

template<char... Digits>
  constexpr int
  operator""_intx()
  { return __gnu_cxx::__parse_int<Digits...>::value; }

int
main()
{
  int i = 1048576;

  i = 1'048'576_int;
  assert(i == 1048576);

  i = 0x10'0000_int;
  assert(i == 1048576);

  i = 0'004'000'000_int;
  assert(i == 1048576);

  i = 0b0001'0000'0000'0000'0000'0000_int;
  assert(i == 1048576);

  unsigned long j = 1567978715;

  j = 1'567'978'715_intx;
  assert(j == 1567978715);

  j = 0x5d75'74db_intx;
  assert(j == 1567978715);

  j = 013'535'272'333_intx;
  assert(j == 1567978715);

  j = 0b0101'1101'0111'0101'0111'0100'1101'1011_intx;
  assert(j == 1567978715);

  double x = 3.141592654e10;

  x = 3.141'592'654e10;
  assert(x == 3.141592654e10);

  x = 3.141'592'654e1'0;
  assert(x == 3.141592654e10);
}
