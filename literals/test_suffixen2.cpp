// /home/ed/bin/bin/g++ -std=c++1y -fdiagnostics-color=auto -o test_suffixen2 test_suffixen2.cpp

// n3642.pdf by Peter Sommerlad

#include <cassert>
#include <string>
#include <complex>
#include <chrono>

#include <iostream>

#include <cxxabi.h>

const std::string
demangle(const char* name)
{
  int status = -4;
  char* res = abi::__cxa_demangle(name, NULL, NULL, &status);
  const char* const demangled_name = (status == 0) ? res : name;
  std::string ret_val(demangled_name);
  free(res);
  return ret_val;
}

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
  constexpr auto c = 0x123abc_testit; // value 0x123
  assert(c == 0x123abc);
  constexpr auto d = 0b0101_testit; // value 0b0101
  assert(d == 0b0101);

  std::cout << sizeof(std::__select_int::_Select_int<1LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<10LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<100LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<1000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<10000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<100000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<1000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<10000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<100000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<1000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<10000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<100000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<1000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<10000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<100000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<1000000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<10000000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<100000000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int<1000000000000000000LL, char, short, int, long, long long>::value) << '\n';
  //std::cout << sizeof(std::__select_int::_Select_int<10000000000000000000LL, char, short, int, long, long long>::value) << '\n';
  //std::cout << sizeof(std::__select_int::_Select_int<100000000000000000000LL, char, short, int, long, long long>::value) << '\n';
  //std::cout << sizeof(std::__select_int::_Select_int<1000000000000000000000LL, char, short, int, long, long long>::value) << '\n';
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

  static_assert(std::is_same<decltype(1.0i_f), std::complex<float>>::value,
		"i_f is std::complex<float>");

  static_assert(std::is_same<decltype(1i_f), std::complex<float>>::value,
		"i_f is std::complex<float>");

  static_assert(std::is_same<decltype(1.0i), std::complex<double>>::value,
		"i is std::complex<double>");

  static_assert(std::is_same<decltype(1i), std::complex<double>>::value,
		"i is std::complex<double>");

  static_assert(std::is_same<decltype(1.0il), std::complex<long double>>::value,
		"il is std::complex<long double>");

  static_assert(std::is_same<decltype(1il), std::complex<long double>>::value,
		"il is std::complex<long double>");
}

void
test03()
{
  using namespace std::literals::chrono_literals;

  static_assert(std::is_same<decltype(1h), std::chrono::hours>::value,
		"h is std::chrono::hours");

  static_assert(std::is_same<decltype(1.0h),
		std::chrono::duration<long double, std::ratio<3600L, 1L>>>::value,
		"h is std::chrono::duration<long double, std::ratio<3600L, 1L>>");

  static_assert(std::is_same<decltype(1min), std::chrono::minutes>::value,
		"min is std::chrono::minutes");

  static_assert(std::is_same<decltype(1.0min),
		std::chrono::duration<long double, std::ratio<60L, 1L>>>::value,
		"min is std::chrono::duration<long double, std::ratio<60L, 1L>>");

  static_assert(std::is_same<decltype(1s), std::chrono::seconds>::value,
		"s is std::chrono::seconds");

  static_assert(std::is_same<decltype(1.0s),
		std::chrono::duration<long double, std::ratio<1L, 1L>>>::value,
		"s is std::chrono::duration<long double, std::ratio<1L, 1L>>");

  static_assert(std::is_same<decltype(1ms), std::chrono::milliseconds>::value,
		"ms is std::chrono::milliseconds");

  static_assert(std::is_same<decltype(1.0ms),
		std::chrono::duration<long double, std::ratio<1L, 1000L>>>::value,
		"ms is std::chrono::duration<long double, std::ratio<1L, 1000L>>");

  static_assert(std::is_same<decltype(1us), std::chrono::microseconds>::value,
		"us is std::chrono::microseconds");

  static_assert(std::is_same<decltype(1.0us),
		std::chrono::duration<long double, std::ratio<1L, 1000000L>>>::value,
		"us is std::chrono::duration<long double, std::ratio<1L, 1000000L>>");

  static_assert(std::is_same<decltype(1ns), std::chrono::nanoseconds>::value,
		"ns is std::chrono::nanoseconds");

  static_assert(std::is_same<decltype(1.0ns),
		std::chrono::duration<long double, std::ratio<1L, 1000000000L>>>::value,
		"ns is std::chrono::duration<long double, std::ratio<1L, 1000000000L>>");
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

    std::cout << '\n';

    auto h = "Hello!"s;

    auto h8 = u8"Hello!"s;

    auto hw = L"Hello!"s;

    auto h16 = u"Hello!"s;

    auto h32 = U"Hello!"s;

    std::string planet = "Mercury"s;
    std::wstring wplanet = L"Venus"s;
    std::string u8planet = u8"Mars"s;
    std::u16string u16planet = u"Juiter"s;
    std::u32string u32planet = U"Saturn"s;

    assert( planet == std::string("Mercury") );
    assert( wplanet == std::wstring(L"Venus") );
    assert( u8planet == std::string(u8"Mars") );
    assert( u16planet == std::u16string(u"Juiter") );
    assert( u32planet == std::u32string(U"Saturn") );
  }
  {
    using namespace std::literals::complex_literals;

    std::cout << '\n';

    auto fi = -1.1i_f;
    std::cout << "sizeof(fi)  = " << sizeof(fi) << '\n';

    auto ifi = -11i_f;
    std::cout << "sizeof(ifi) = " << sizeof(ifi) << '\n';

    auto i = -2.2i;
    std::cout << "sizeof(i)   = " << sizeof(i) << '\n';

    auto ii = -22i;
    std::cout << "sizeof(ii)  = " << sizeof(ii) << '\n';

    auto li = -3.3il;
    std::cout << "sizeof(li)  = " << sizeof(li) << '\n';

    auto ili = -33il;
    std::cout << "sizeof(ili) = " << sizeof(ili) << '\n';

    std::complex<float> j1 = 1.0i_f;
    std::complex<float> k1 = 1i_f;
    std::complex<double> j2 = 2.0i;
    std::complex<double> k2 = 2i;
    std::complex<long double> j4 = 4.0il;
    std::complex<long double> k4 = 4il;

    assert( j1 == std::complex<float>(0.0F, 1.0F) );
    assert( k1 == std::complex<float>(0.0F, 1.0F) );
    assert( j2 == std::complex<double>(0.0, 2.0) );
    assert( k2 == std::complex<double>(0.0, 2.0) );
    assert( j4 == std::complex<long double>(0.0L, 4.0L) );
    assert( k4 == std::complex<long double>(0.0L, 4.0L) );
  }
  {
    using namespace std::literals::chrono_literals;

    std::cout << '\n';

    auto th = 1h;
    std::cout << "sizeof(th)  = " << sizeof(th) << '\n';
    std::cout << "typeid(th)  = " << demangle(typeid(th).name()) << '\n';

    auto fth = 1.0h;
    std::cout << "sizeof(fth) = " << sizeof(fth) << '\n';
    std::cout << "typeid(fth) = " << demangle(typeid(fth).name()) << '\n';

    auto tmin = 1min;
    std::cout << "sizeof(tmin)  = " << sizeof(tmin) << '\n';
    std::cout << "typeid(tmin)  = " << demangle(typeid(tmin).name()) << '\n';

    auto ftmin = 1.0min;
    std::cout << "sizeof(ftmin) = " << sizeof(ftmin) << '\n';
    std::cout << "typeid(ftmin) = " << demangle(typeid(ftmin).name()) << '\n';

    auto ts = 1s;
    std::cout << "sizeof(ts)  = " << sizeof(ts) << '\n';
    std::cout << "typeid(ts)  = " << demangle(typeid(ts).name()) << '\n';

    auto fts = 1.0s;
    std::cout << "sizeof(fts) = " << sizeof(fts) << '\n';
    std::cout << "typeid(fts) = " << demangle(typeid(fts).name()) << '\n';

    auto tm = 1ms;
    std::cout << "sizeof(tm)  = " << sizeof(tm) << '\n';
    std::cout << "typeid(tm)  = " << demangle(typeid(tm).name()) << '\n';

    auto ftm = 1.0ms;
    std::cout << "sizeof(ftm) = " << sizeof(ftm) << '\n';
    std::cout << "typeid(ftm) = " << demangle(typeid(ftm).name()) << '\n';

    auto tu = 1us;
    std::cout << "sizeof(tu)  = " << sizeof(tu) << '\n';
    std::cout << "typeid(tu)  = " << demangle(typeid(tu).name()) << '\n';

    auto ftu = 1.0us;
    std::cout << "sizeof(ftu) = " << sizeof(ftu) << '\n';
    std::cout << "typeid(ftu) = " << demangle(typeid(ftu).name()) << '\n';

    auto tn = 1ns;
    std::cout << "sizeof(tn)  = " << sizeof(tn) << '\n';
    std::cout << "typeid(tn)  = " << demangle(typeid(tn).name()) << '\n';

    auto ftn = 1.0ns;
    std::cout << "sizeof(ftn) = " << sizeof(ftn) << '\n';
    std::cout << "typeid(ftn) = " << demangle(typeid(ftn).name()) << '\n';

/*
    auto jiffy = 23ns;
    auto fjiffy = 23.0ns;
    auto blip = 14.0us;
    auto fblip = 14us;
    auto bit = 77ms;
    auto fbit = 77.0ms;
    auto warmup = 33s;
    auto fwarmup = 33.0s;
    auto classtime = 50min;
    auto fclasstime = 50.0min;
    auto longtime = 1h + 30min;
    auto flongtime = 1.0h + 30.0min;
    auto workday = 8h;
    auto fworkday = 8.0h;

    assert( jiffy == 23 * std::chrono::nanoseconds );
    assert( fjiffy == 23.0 * std::chrono::nanoseconds );
    assert( blip == 14 * std::chrono::microseconds );
    assert( fblip == 14.0 * std::chrono::microseconds );
    assert( bit == 77 * std::chrono::milliseconds );
    assert( fbit == 77.0 * std::chrono::milliseconds );
    assert( warmup == 33 * std::chrono::seconds );
    assert( fwarmup == 33.0 * std::chrono::seconds );
    assert( classtime == 50 * std::chrono::minutes );
    assert( fclasstime == 50.0 * std::chrono::minutes );
    assert( longtime == 90 * std::chrono::minutes );
    assert( flongtime == 90.0 * std::chrono::minutes );
    assert( workday == 8 * std::chrono::hours );
    assert( fworkday == 8.0 * std::chrono::hours );
*/
  }
}
