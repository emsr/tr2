#ifndef COMPLEX_UTIL_H
#define COMPLEX_UTIL_H 1

/**
 *  Return the L1 norm modulus or the Manhattan metric distance of a complex number.
 */
template<typename _Tp>
  inline _Tp
  __norm_L1(const std::complex<_Tp> & z)
  { return std::abs(std::real(z)) + std::abs(std::imag(z)); }

/**
 *  Return the Linf norm modulus of a complex number.
 */
template<typename _Tp>
  inline _Tp
  __norm_Linf(const std::complex<_Tp> & z)
  { return std::max(std::abs(std::real(z)), std::abs(std::imag(z))); }

/**
 *  Carefully compute @c z1/z2 avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Tp>
  bool
  __safe_div(std::complex<_Tp> __z1, std::complex<_Tp> __z2,
	   std::complex<_Tp> & __z1dz2);

/**
 *  Carefully compute @c s/z2 avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Sp, typename _Tp>
  inline bool
  __safe_div(_Sp __s, std::complex<_Tp> __z,
	     std::complex<_Tp> & __sdz)
  { return __safe_div(std::complex<_Tp>(__s), __z, __sdz); }

/**
 *  Carefully compute @c z1/s avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Sp, typename _Tp>
  inline bool
  __safe_div(std::complex<_Tp> __z, _Sp __s,
	     std::complex<_Tp> & __zds)
  { return __safe_div(__z, std::complex<_Tp>(__s), __zds); }

#include "complex_util.tcc"

#endif // COMPLEX_UTIL_H
