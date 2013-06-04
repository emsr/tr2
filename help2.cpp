// /home/ed/bin/bin/g++ -std=c++11 -fdiagnostics-color=auto -o help2 help2.cpp

#include <iostream>
#include <sstream>

struct _A
{
  _A(int& __i)
  : __int{__i}
  { }

  _A&
  operator=(const _A&) = delete;

  int& __int;
};

template<typename _Char, typename _Traits>
  std::basic_ostream<_Char, _Traits>&
  operator<<(std::basic_ostream<_Char, _Traits>& __os, const _A& __a)
  {
    __os << __a.__int;
  }

template<typename _Char, typename _Traits>
  std::basic_istream<_Char, _Traits>&
  operator>>(std::basic_istream<_Char, _Traits>& __is, _A& __a)
  {
    __is >> __a.__int;
  }

const _A&
quoted(const _A& __a)
{ return __a; }

//  Works...
/*
int&
quoted(_A& __a)
{ return __a.__int; }

int&
quoted(_A&& __a)
{ return __a.__int; }
*/

//  Works...
_A&
quoted(_A& __a)
{ return __a; }

_A&
quoted(_A&& __a)
{ return __a; }

int
main()
{
  std::stringstream ss;
  int original = 42;
  int round_trip = 0;
  _A a_round_trip(round_trip);

  ss << _A(original);
  std::cout << "_A(original): " << ss.str() << '\n';
  ss >> a_round_trip; // Works with quoted(_A& __a).
  std::cout << "a_round_trip: " << a_round_trip << '\n';
  //ss >> _A(round_trip); // Fails.

  original = 666;
  std::cout << "original: " << original << '\n';
  std::cout << "_A(original): " << _A(original) << '\n';
  ss.seekp(0);
  ss.seekg(0);
  ss.clear();
  ss << quoted(original);
  std::cout << "_A(original): " << ss.str() << '\n';
  ss >> quoted(a_round_trip); // Works with int& quoted(_A& __a).
  std::cout << "a_round_trip: " << a_round_trip << '\n';
  ss >> quoted(round_trip); // Works with int& quoted(_A&& __a).
  std::cout << "quoted(round_trip): " << quoted(round_trip) << '\n';
}
