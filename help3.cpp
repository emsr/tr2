// /home/ed/bin/bin/g++ -std=c++11 -fdiagnostics-color=auto -o help3 help3.cpp

#include <string>
#include <iostream>
#include <sstream>

template<typename _String>
  struct _A
  {
    _A(_String& __str)
    : __string{__str}
    { }

    _A&
    operator=(const _A&) = delete;

    _String& __string;
  };

template<typename _Char, typename _Traits, typename _String>
  std::basic_ostream<_Char, _Traits>&
  operator<<(std::basic_ostream<_Char, _Traits>& __os, const _A<_String>& __a)
  {
    __os << __a.__string;
  }

template<typename _Char, typename _Traits, typename _String>
  std::basic_istream<_Char, _Traits>&
  operator>>(std::basic_istream<_Char, _Traits>& __is, _A<_String>& __a)
  {
    __is >> __a.__string;
  }

//template<typename _String>
//  const _A<_String>&
//  quoted(const _A<_String>& __a)
//  { return __a; }

//  Works...
/*
template<typename _String>
  _String&
  quoted(_A<_String>& __a)
  { return __a.__string; }

template<typename _String>
  _String&
  quoted(_A<_String>&& __a)
  { return __a.__string; }
*/

//  Works...
//template<typename _String>
//  _A<_String>&
//  quoted(_A<_String>& __a)
//  { return __a; }

//template<typename _String>
//  _A<_String>&
//  quoted(_A<_String>&& __a)
//  { return __a; }

template<typename _String>
  _A<_String&>
  quoted(_String& __str)
  { return _A<_String&>(__str); }

int
main()
{
  std::stringstream ss;
  int original = 42;
  int round_trip = 0;
  _A<int> a_round_trip(round_trip);

  ss << _A<int>(original);
  std::cout << "_A<int>(original): " << ss.str() << '\n';
  ss >> a_round_trip; // Works with quoted(_A<int>& __a).
  std::cout << "a_round_trip: " << a_round_trip << '\n';
  //ss >> _A<int>(round_trip); // Fails.

  original = 666;
  std::cout << "original: " << original << '\n';
  std::cout << "_A(original): " << _A<int>(original) << '\n';
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
