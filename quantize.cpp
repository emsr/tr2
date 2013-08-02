
//  min-step quantization
template<typename _Tp>
  _Tp
  quantize(_Tp __x, _Tp __delta)
  {
    return std::copysign(_Tp(1), value)
	   * __delta * std::floor(std::abs(__x) / __delta + _Tp(0.5L));
  }

//  min-riser quantization
template<typename _Tp>
  _Tp
  quantize(_Tp __x, _Tp __delta)
  {
    return __delta * (std::floor(__x / __delta) + _Tp(0.5L));
  }
