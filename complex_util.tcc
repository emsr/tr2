#ifndef COMPLEX_UTIL_TCC
#define COMPLEX_UTIL_TCC 1

/**
 *  @brief Carefully compute @c z1/z2 avoiding overflow and destructive underflow.
 *	   If the quotient is successfully computed, then the logical value @c true
 *	   is returned and the quotient is returned in @c z1dz2.
 *	   Otherwise, @c false is returned and the quotient is not.
 *
 *  @param[in]  z1  Dividend
 *  @param[in]  z2  Divisor
 *  @param[out]  z1dz2  Quotient
 *  @return @c true on success @c false on ailure
 */
template<typename _Tp>
  bool
  __safe_div(std::complex<_Tp> __z1, std::complex<_Tp> __z2,
	   std::complex<_Tp> & __z1dz2)
  {
    //  Note that xhinf is a machine floating-point dependent constant
    //  set equal to half the largest available floating-point number.
    static constexpr _Tp __xhinf = 0.5L * std::numeric_limits<_Tp>::max();

    //  Separate real and imaginary parts of arguments
    auto __re1 = std::real(__z1);
    auto __im1 = std::imag(__z1);
    auto __re2 = std::real(__z2);
    auto __im2 = std::imag(__z2);

    //  Set up largest and smallest magnitudes needed
    auto __z1b = std::max(std::abs(__re1), std::abs(__im1));
    auto __z2b = std::abs(__re2);
    auto __z2ub = std::abs(__im2);

    if (__z2b < __z2ub)
      std::swap(__z2b, __z2ub);

    //  If overflow will occur, then abort
    if (__z2b < _Tp(1) && __z1b > __z2b * __xhinf)
      return false;

    __re1 /= __z1b;
    __im1 /= __z1b;
    __re2 /= __z2b;
    __im2 /= __z2b;
    auto __term = __z2ub / __z2b;
    auto __denom = _Tp(1) + __term * __term;
    auto __scale = __z1b / __z2b / __denom;
    auto __qr = (__re1 * __re2 + __im1 * __im2) * __scale;
    auto __qi = (__re2 * __im1 - __re1 * __im2) * __scale;
    __z1dz2 = std::complex<_Tp>{__qr, __qi};

    return true;
  }

#endif // COMPLEX_UTIL_TCC
