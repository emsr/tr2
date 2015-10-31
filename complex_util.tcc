#ifndef COMPLEX_UTIL_TCC
#define COMPLEX_UTIL_TCC 1

/**
 *  Carefully compute @c z1/z2 avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Tp>
  bool
  zdiv(std::complex<_Tp> z1, std::complex<_Tp> z2,
       std::complex<_Tp> & z1dz2)
  {
    //  Note that xhinf is a machine floating-point dependent constant
    //  set equal to half the largest available floating-point number.
    static constexpr _Tp xhinf = 0.5L * std::numeric_limits<_Tp>::max();

    //  Separate real and imaginary parts of arguments
    auto z1r = std::real(z1);
    auto z1i = std::imag(z1);
    auto z2r = std::real(z2);
    auto z2i = std::imag(z2);

    //  Set up largest and smallest magnitudes needed
    auto z1b = std::max(std::abs(z1r), std::abs(z1i));
    auto z2b = std::abs(z2r);
    auto z2ub = std::abs(z2i);

    if (z2b < z2ub)
      std::swap(z2b, z2ub);

    //  If overflow will occur, then abort
    if (z2b < _Tp(1) && z1b > z2b * xhinf)
      return false;

    //  Compute the quotient
    z1r /= z1b;
    z1i /= z1b;
    z2r /= z2b;
    z2i /= z2b;
    auto term = z2ub / z2b;
    auto denom = _Tp(1) + term * term;
    auto scale = z1b / z2b / denom;
    auto qr = (z1r * z2r + z1i * z2i) * scale;
    auto qi = (z2r * z1i - z1r * z2i) * scale;
    z1dz2 = std::complex<_Tp>{qr, qi};

    return true;
  }

#endif // COMPLEX_UTIL_TCC
