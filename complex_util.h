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
  bool
  safe_div(std::complex<_Tp> z1, std::complex<_Tp> z2,
	   std::complex<_Tp> & z1dz2);

/**
 *  Carefully compute @c s/z2 avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Sp, typename _Tp>
  inline bool
  safe_div(_Sp s, std::complex<_Tp> z2,
	   std::complex<_Tp> & z1dz2)
  { return safe_div(std::complex<_Tp>(s), z2, z1dz2); }

/**
 *  Carefully compute @c z1/s avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Sp, typename _Tp>
  inline bool
  safe_div(std::complex<_Tp> z1, _Sp s,
	   std::complex<_Tp> & z1dz2)
  { return safe_div(z1, std::complex<_Tp>(s), z1dz2); }

#include "complex_util.tcc"

#endif // COMPLEX_UTIL_H
