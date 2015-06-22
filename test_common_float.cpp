// $HOME/bin/bin/g++ -std=c++1y -o test_common_float test_common_float.cpp

#include <type_traits>
#include <iostream>
#include "common_float.h"

int
main()
{
  std::cout << std::boolalpha;

  using cta = std::common_type_t<int, float>;
  using ctb = std::common_type_t<int, double>;
  using ctc = std::common_type_t<int, long double>;

  std::cout << std::endl;
  std::cout << "common_type_t<int, float>                == double     : " << std::is_same<cta,double>::value << std::endl;
  std::cout << "common_type_t<int, double>               == double     : " << std::is_same<ctb,double>::value << std::endl;
  std::cout << "common_type_t<int, long double>          == long double: " << std::is_same<ctc,long double>::value << std::endl;

  using cfa = common_float<char>::type;
  using cfb = common_float<int>::type;
  using cfc = common_float<const unsigned long long>::type;
  using cfd = common_float<float>::type;
  using cfe = common_float<double>::type;
  using cff = common_float<long double>::type;
  using cfg = common_float<int, float>::type;
  using cfh = common_float<int, double>::type;
  using cfi = common_float<int, long double>::type;

  std::cout << std::endl;
  std::cout << "common_float_t<char>                     == double     : " << std::is_same<cfa,double>::value << std::endl;
  std::cout << "common_float_t<int>                      == double     : " << std::is_same<cfb,double>::value << std::endl;
  std::cout << "common_float_t<const unsigned long long> == double     : " << std::is_same<cfc,double>::value << std::endl;
  std::cout << "common_float_t<float>                    == float      : " << std::is_same<cfd,float>::value << std::endl;
  std::cout << "common_float_t<double>                   == double     : " << std::is_same<cfe,double>::value << std::endl;
  std::cout << "common_float_t<long double>              == long double: " << std::is_same<cff,long double>::value << std::endl;
  std::cout << "common_float_t<int, float>               == double     : " << std::is_same<cfg,double>::value << std::endl;
  std::cout << "common_float_t<int, double>              == double     : " << std::is_same<cfh,double>::value << std::endl;
  std::cout << "common_float_t<int, long double>         == long double: " << std::is_same<cfi,long double>::value << std::endl;

  using cfx = common_float_t<char, float, long double>;
  std::cout << "common_float_t<char, float, long double> == long double: " << std::is_same<cfx,long double>::value << std::endl;
}
