#ifndef _QUANTIZE_H
#define _QUANTIZE_H 1

#include <cmath>

///  mid-tread quantization
template<typename _Tp>
  constexpr _Tp
  quantize_mid_tread(_Tp __x, _Tp __delta) noexcept
  {
    return std::copysign(_Tp(1), __x)
	   * __delta * std::floor(std::abs(__x) / __delta + _Tp(0.5L));
  }

///  mid-riser quantization
template<typename _Tp>
  constexpr _Tp
  quantize_mid_riser(_Tp __x, _Tp __delta) noexcept
  {
    return __delta * (std::floor(__x / __delta) + _Tp(0.5L));
  }

#endif // _QUANTIZE_H
