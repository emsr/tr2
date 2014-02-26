// /home/ed/bin/bin/g++ -std=c++1y -o test_numeric_limits test_numeric_limits.cpp

#include "numeric_limits"
#include <iostream>

template<typename _Tp>
  void
  test(_Tp __x)
  {
    std::cout << '\n';
    std::cout << "  " << '\n';
    std::cout << "  " << std::is_specialized(__x) << '\n';
    std::cout << "  " << std::min(__x) << '\n';
    std::cout << "  " << std::max(__x) << '\n';
    std::cout << "  " << std::lowest(__x) << '\n';
    std::cout << "  " << std::digits(__x) << '\n';
    std::cout << "  " << std::digits10(__x) << '\n';
    std::cout << "  " << std::max_digits10(__x) << '\n';
    std::cout << "  " << std::xxx_is_signed(__x) << '\n';
    std::cout << "  " << std::is_integer(__x) << '\n';
    std::cout << "  " << std::is_exact(__x) << '\n';
    std::cout << "  " << std::radix(__x) << '\n';
    std::cout << "  " << std::epsilon(__x) << '\n';
    std::cout << "  " << std::round_error(__x) << '\n';
    std::cout << "  " << std::min_exponent(__x) << '\n';
    std::cout << "  " << std::min_exponent10(__x) << '\n';
    std::cout << "  " << std::max_exponent(__x) << '\n';
    std::cout << "  " << std::max_exponent10(__x) << '\n';
    std::cout << "  " << std::has_infinity(__x) << '\n';
    std::cout << "  " << std::has_quiet_NaN(__x) << '\n';
    std::cout << "  " << std::has_signaling_NaN(__x) << '\n';
    std::cout << "  " << std::has_denorm(__x) << '\n';
    std::cout << "  " << std::has_denorm_loss(__x) << '\n';
    std::cout << "  " << std::infinity(__x) << '\n';
    std::cout << "  " << std::quiet_NaN(__x) << '\n';
    std::cout << "  " << std::signaling_NaN(__x) << '\n';
    std::cout << "  " << std::denorm_min(__x) << '\n';
    std::cout << "  " << std::is_iec559(__x) << '\n';
    std::cout << "  " << std::is_bounded(__x) << '\n';
    std::cout << "  " << std::is_modulo(__x) << '\n';
    std::cout << "  " << std::traps(__x) << '\n';
    std::cout << "  " << std::tinyness_before(__x) << '\n';
    std::cout << "  " << std::round_style(__x) << '\n';
  }

int
main()
{
  test(1.0f);
  test(1.0);
  test(1.0l);
}
