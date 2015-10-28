#ifndef COMPLEX_UTIL_H
#define COMPLEX_UTIL_H 1

/**
 *  Return the norm-1 modulus or the Manhattan metric distance of a complex number.
 */
template<typename _Tp>
  inline _Tp
  norm1(const std::complex<_Tp> & z)
  { return std::abs(std::real(z)) + std::abs(std::imag(z)); }

/**
 *  Carefully compute @c z1/z2 avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Tp>
  inline bool
  zdiv(std::complex<_Tp> z1, std::complex<_Tp> z2,
       std::complex<_Tp> & z1dz2)
  {
    //  Note that xhinf is a machine floating-point dependent constant
    //  set equal to half the largest available floating-point number.
    static constexpr auto xhinf = 0.5L * std::numeric_limits<_Tp>::max();

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

/**
 *  Carefully compute @c s/z2 avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Sp, typename _Tp>
  inline bool
  zdiv(_Sp s, std::complex<_Tp> z2,
       std::complex<_Tp> & z1dz2)
  { return zdiv(std::complex<_Tp>(s), z2, z1dz2); }

/**
 *  Carefully compute @c z1/s avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Sp, typename _Tp>
  inline bool
  zdiv(std::complex<_Tp> z1, _Sp s,
       std::complex<_Tp> & z1dz2)
  { return zdiv(z1, std::complex<_Tp>(s), z1dz2); }

#endif // COMPLEX_UTIL_H
