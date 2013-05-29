// /home/ed/bin/bin/g++ -std=c++1y -fdiagnostics-color=auto -o test_suffixen2 test_suffixen2.cpp

// n3642.pdf by Peter Sommerlad

#include <cassert>
#include <string>
#include <complex>
#include <chrono>

template<char... _Digits>
  constexpr unsigned long long
  operator"" _ternary()
  { return std::__parse_int::_Number<3, _Digits...>::value; }

template<char... _Digits>
  constexpr unsigned long long
  operator"" _testit()
  { return std::__parse_int::_Parse_int<_Digits...>::value; }

void
test00()
{
  constexpr auto five = 012_ternary;
  static_assert(five == 5, "_ternary should be three-based"); // value 123

  constexpr auto a = 123_testit;
  assert(a == 123);
  constexpr auto b = 0123_testit; // value 0123
  assert(b == 0123);
  constexpr auto c = 0x123_testit; // value 0x123
  assert(c == 0x123);
  constexpr auto d = 0b0101_testit; // value 0b0101
  assert(d == 0b0101);
}

void
test01()
{
  using namespace std::literals::string_literals;

  static_assert(std::is_same<decltype("Hello"s), std::string>::value,
		"\"Hello\"s is std::string");

  static_assert(std::is_same<decltype(u8"Hello"s), std::string>::value,
		"u8\"Hello\"s is std::string");

  static_assert(std::is_same<decltype(L"Hello"s), std::wstring>::value,
		"L\"Hello\"s is std::wstring");

  static_assert(std::is_same<decltype(u"Hello"s), std::u16string>::value,
		"u\"Hello\"s is std::u16string");

  static_assert(std::is_same<decltype(U"Hello"s), std::u32string>::value,
		"U\"Hello\"s is std::u32string");
}

void
test02()
{
  using namespace std::literals::complex_literals;

  static_assert(std::is_same<decltype(1i_f), std::complex<float>>::value,
		"i_f is std::complex<float>");

  static_assert(std::is_same<decltype(1.0i_f), std::complex<float>>::value,
		"i_f is std::complex<float>");

  static_assert(std::is_same<decltype(1i), std::complex<double>>::value,
		"i is std::complex<double>");

  static_assert(std::is_same<decltype(1.0i), std::complex<double>>::value,
		"i is std::complex<double>");

  static_assert(std::is_same<decltype(1il), std::complex<long double>>::value,
		"il is std::complex<long double>");

  static_assert(std::is_same<decltype(1.0il), std::complex<long double>>::value,
		"il is std::complex<long double>");
}

void
test03()
{
  using namespace std::literals::chrono_literals;

  static_assert(std::is_same<decltype(1h), std::chrono::hours>::value,
		"h is std::chrono::hours");

//  static_assert(std::is_same<decltype(1.0h), std::chrono::hours>::value,
//		"h is std::chrono::hours");

  static_assert(std::is_same<decltype(1min), std::chrono::minutes>::value,
		"min is std::chrono::minutes");

//  static_assert(std::is_same<decltype(1.0min), std::chrono::minutes>::value,
//		"min is std::chrono::minutes");

  static_assert(std::is_same<decltype(1s), std::chrono::seconds>::value,
		"s is std::chrono::seconds");

//  static_assert(std::is_same<decltype(1.0s), std::chrono::seconds>::value,
//		"s is std::chrono::seconds");

  static_assert(std::is_same<decltype(1ms), std::chrono::milliseconds>::value,
		"ms is std::chrono::milliseconds");

//  static_assert(std::is_same<decltype(1.0ms), std::chrono::milliseconds>::value,
//		"ms is std::chrono::milliseconds");

  static_assert(std::is_same<decltype(1us), std::chrono::microseconds>::value,
		"us is std::chrono::microseconds");

//  static_assert(std::is_same<decltype(1.0us), std::chrono::microseconds>::value,
//		"us is std::chrono::microseconds");

  static_assert(std::is_same<decltype(1ns), std::chrono::nanoseconds>::value,
		"ns is std::chrono::nanoseconds");

//  static_assert(std::is_same<decltype(1.0ns), std::chrono::nanoseconds>::value,
//		"ns is std::chrono::nanoseconds");
}

int
main()
{
  test00();
  test01();
  test02();
  test03();

  {
    using namespace std::literals::string_literals;
    auto h = "Hello!"s;
    auto h8 = u8"Hello!"s;
    auto hw = L"Hello!"s;
    auto h16 = u"Hello!"s;
    auto h32 = U"Hello!"s;
  }
  {
    using namespace std::literals::complex_literals;
    auto fi = -1.1i_f;
    auto ifi = -11i_f;
    auto i = -2.2i;
    auto ii = -22i;
    auto li = -3.3il;
    auto ili = -33il;
  }
  {
    using namespace std::literals::chrono_literals;
    auto th = 1h;
    auto fth = 1.0h;
    auto tmin = 1min;
    auto ftmin = 1.0min;
    auto ts = 1s;
    auto fts = 1.0s;
    auto tm = 1ms;
    auto ftm = 1.0ms;
    auto tu = 1us;
    auto ftu = 1.0us;
    auto tn = 1ns;
    auto ftn = 1.0ns;
  }
}
