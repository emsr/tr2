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
  safe_div(std::complex<_Tp> z1, std::complex<_Tp> z2,
	   std::complex<_Tp> & z1dz2)
  {
    //  Note that xhinf is a machine floating-point dependent constant
    //  set equal to half the largest available floating-point number.
    static constexpr _Tp xhinf = 0.5L * std::numeric_limits<_Tp>::max();

    //  Separate real and imaginary parts of arguments
    auto re1 = std::real(z1);
    auto im1 = std::imag(z1);
    auto re2 = std::real(z2);
    auto im2 = std::imag(z2);

    //  Set up largest and smallest magnitudes needed
    auto z1b = std::max(std::abs(re1), std::abs(im1));
    auto z2b = std::abs(re2);
    auto z2ub = std::abs(im2);

    if (z2b < z2ub)
      std::swap(z2b, z2ub);

    //  If overflow will occur, then abort
    if (z2b < _Tp(1) && z1b > z2b * xhinf)
      return false;

    re1 /= z1b;
    im1 /= z1b;
    re2 /= z2b;
    im2 /= z2b;
    auto term = z2ub / z2b;
    auto denom = _Tp(1) + term * term;
    auto scale = z1b / z2b / denom;
    auto qr = (re1 * re2 + im1 * im2) * scale;
    auto qi = (re2 * im1 - re1 * im2) * scale;
    z1dz2 = std::complex<_Tp>{qr, qi};

    return true;
  }

#endif // COMPLEX_UTIL_TCC
