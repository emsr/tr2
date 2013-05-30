// /home/ed/bin_tr2/bin/g++ -std=c++1y -fdiagnostics-color=auto -o test_suffixen test_suffixen.cpp

// n3642.pdf by Peter Sommerlad

#include <cassert>
#include <iostream>
#include <typeinfo>

#include "suffixen.h"

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

template<char... _Digs>
  constexpr unsigned long long
  operator"" _ternary()
  { return std::__parse_int::_Number<3, _Digs...>::value; }

template<char... _Digs>
  constexpr unsigned long long
  operator"" _testit()
  { return std::__parse_int::_Parse_int<_Digs...>::value; }

void
test00()
{
  constexpr auto five = 012_ternary;
  static_assert(five == 5, "_ternary should be three-based"); // value 123

  constexpr auto a = 123_testit;
  assert(a == 123);
  constexpr auto b = 0123_testit; // value 0123
  assert(b == 0123);
  constexpr auto c = 0x123abc_testit; // value 0x123abc
  assert(c == 0x123abc);
  constexpr auto d = 0b0101_testit; // value 0b0101
  assert(d == 0b0101);

  std::cout << sizeof(std::__select_int::_Select_int_base<1LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<10LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<100LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<1000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<10000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<100000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<1000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<10000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<100000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<1000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<10000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<100000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<1000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<10000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<100000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<1000000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<10000000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<100000000000000000LL, char, short, int, long, long long>::value) << '\n';
  std::cout << sizeof(std::__select_int::_Select_int_base<1000000000000000000LL, char, short, int, long, long long>::value) << '\n';
  //std::cout << sizeof(std::__select_int::_Select_int_base<10000000000000000000LL, char, short, int, long, long long>::value) << '\n';
  //std::cout << sizeof(std::__select_int::_Select_int_base<100000000000000000000LL, char, short, int, long, long long>::value) << '\n';
  //std::cout << sizeof(std::__select_int::_Select_int_base<1000000000000000000000LL, char, short, int, long, long long>::value) << '\n';
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
    assert( planet == std::string("Mercury") );
    std::wstring wplanet = L"Venus"s;
    assert( wplanet == std::wstring(L"Venus") );
    std::string u8planet = u8"Mars"s;
    assert( u8planet == std::string(u8"Mars") );
    std::u16string u16planet = u"Juiter"s;
    assert( u16planet == std::u16string(u"Juiter") );
    std::u32string u32planet = U"Saturn"s;
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
    assert( j1 == std::complex<float>(0.0F, 1.0F) );
    std::complex<float> k1 = 1i_f;
    assert( k1 == std::complex<float>(0.0F, 1.0F) );
    std::complex<double> j2 = 2.0i;
    assert( j2 == std::complex<double>(0.0, 2.0) );
    std::complex<double> k2 = 2i;
    assert( k2 == std::complex<double>(0.0, 2.0) );
    std::complex<long double> j4 = 4.0il;
    assert( j4 == std::complex<long double>(0.0L, 4.0L) );
    std::complex<long double> k4 = 4il;
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

    auto jiffy = 23ns;
    assert( jiffy == std::chrono::nanoseconds(23) );
    auto fjiffy = 23.0ns;
    assert( (fjiffy == std::chrono::duration<long double, std::nano>(23.0L)) );
    auto blip = 14us;
    assert( blip == std::chrono::microseconds(14) );
    auto fblip = 14.0us;
    assert( (fblip == std::chrono::duration<long double, std::micro>(14.0L)) );
    auto bit = 77ms;
    assert( bit == std::chrono::milliseconds(77) );
    auto fbit = 77.0ms;
    assert( (fbit == std::chrono::duration<long double, std::milli>(77.0L)) );
    auto warmup = 33s;
    assert( warmup == std::chrono::seconds(33) );
    auto fwarmup = 33.0s;
    assert( (fwarmup == std::chrono::duration<long double, std::ratio<1,1>>(33.0L)) );
    auto classtime = 50min;
    assert( classtime == std::chrono::minutes(50) );
    auto fclasstime = 50.0min;
    assert( (fclasstime == std::chrono::duration<long double, std::ratio<60,1>>(50.0L)) );
    auto longtime = 1h + 30min;
    assert( longtime == std::chrono::minutes(90) );
    auto flongtime = 1.0h + 30.0min;
    assert( (flongtime == std::chrono::duration<long double, std::ratio<3600,1>>(1.0L)
			+ std::chrono::duration<long double, std::ratio<60,1>>(30.0L)) );
    assert( (flongtime == std::chrono::duration<long double, std::ratio<60,1>>(90.0L)) );
    auto workday = 8h;
    assert( workday == std::chrono::hours(8) );
    auto fworkday = 8.0h;
    assert( (fworkday == std::chrono::duration<long double, std::ratio<3600,1>>(8.0L)) );
  }
}
