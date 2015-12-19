// Special functions -*- C++ -*-

// Copyright (C) 2006-2013 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file tbd/beta.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tbd/cmath}
 */

#ifndef _GLIBCXX_MODIFIED_BESSEL_FUNC_TCC
#define _GLIBCXX_MODIFIED_BESSEL_FUNC_TCC 1


///  We'll pull this into the main (TR1) modified_bessel_func.tcc ultimately.

namespace std
{
namespace __detail
{


/**
 *   @brief  Compute the spherical modified Bessel functions
 *           @f$ i_n(x) @f$ and @f$ k_n(x) @f$ and their first
 *           derivatives @f$ i'_n(x) @f$ and @f$ k'_n(x) @f$
 *           respectively.
 *
 *   @param  __n  The order of the modified spherical Bessel function.
 *   @param  __x  The argument of the modified spherical Bessel function.
 *   @param  __i_n  The output regular modified spherical Bessel function.
 *   @param  __k_n  The output irregular modified spherical
 *                  Bessel function.
 *   @param  __ip_n  The output derivative of the regular modified
 *                   spherical Bessel function.
 *   @param  __kp_n  The output derivative of the irregular modified
 *                   spherical Bessel function.
 */
template <typename _Tp>
  void
  __sph_bessel_ik(unsigned int __n, _Tp __x,
                  _Tp & __i_n, _Tp & __k_n, _Tp & __ip_n, _Tp & __kp_n)
  {
    const _Tp __nu = _Tp(__n) + _Tp(0.5L);

    _Tp __I_nu, __Ip_nu, __K_nu, __Kp_nu;
    __bessel_ik(__nu, __x, __I_nu, __K_nu, __Ip_nu, __Kp_nu);

    const _Tp __factor = __numeric_constants<_Tp>::__sqrtpio2()
                       / std::sqrt(__x);

    __i_n = __factor * __I_nu;
    __k_n = __factor * __K_nu;
    __ip_n = __factor * __Ip_nu - __i_n / (_Tp(2) * __x);
    __kp_n = __factor * __Kp_nu - __k_n / (_Tp(2) * __x);

    return;
  }


/**
 *   @brief  Compute the Airy functions
 *           @f$ Ai(x) @f$ and @f$ Bi(x) @f$ and their first
 *           derivatives @f$ Ai'(x) @f$ and @f$ Bi(x) @f$
 *           respectively.
 *
 *   @param  __x  The argument of the Airy functions.
 *   @param  __Ai  The output Airy function of the first kind.
 *   @param  __Bi  The output Airy function of the second kind.
 *   @param  __Aip  The output derivative of the Airy function
 *                  of the first kind.
 *   @param  __Bip  The output derivative of the Airy function
 *                  of the second kind.
 */
template <typename _Tp>
  void
  __airy(_Tp __x, _Tp & __Ai, _Tp & __Bi, _Tp & __Aip, _Tp & __Bip)
  {
    const _Tp __absx = std::abs(__x);
    const _Tp __rootx = std::sqrt(__absx);
    const _Tp __z = _Tp(2) * __absx * __rootx / _Tp(3);

    if (__x > _Tp(0))
      {
        _Tp __I_nu, __Ip_nu, __K_nu, __Kp_nu;

        __bessel_ik(_Tp(1) / _Tp(3), __z, __I_nu, __K_nu, __Ip_nu, __Kp_nu);
        __Ai = __rootx * __K_nu
             / (__numeric_constants<_Tp>::__sqrt3()
              * __numeric_constants<_Tp>::__pi());
        __Bi = __rootx * (__K_nu / __numeric_constants<_Tp>::__pi()
               + _Tp(2) * __I_nu / __numeric_constants<_Tp>::__sqrt3());

        __bessel_ik(_Tp(2) / _Tp(3), __z, __I_nu, __K_nu, __Ip_nu, __Kp_nu);
        __Aip = -__x * __K_nu
              / (__numeric_constants<_Tp>::__sqrt3()
               * __numeric_constants<_Tp>::__pi());
        __Bip = __x * (__K_nu / __numeric_constants<_Tp>::__pi()
                    + _Tp(2) * __I_nu
                    / __numeric_constants<_Tp>::__sqrt3());
      }
    else if (__x < _Tp(0))
      {
        _Tp __J_nu, __Jp_nu, __N_nu, __Np_nu;

        __bessel_jn(_Tp(1) / _Tp(3), __z, __J_nu, __N_nu, __Jp_nu, __Np_nu);
        __Ai = __rootx * (__J_nu
                  - __N_nu / __numeric_constants<_Tp>::__sqrt3()) / _Tp(2);
        __Bi = -__rootx * (__N_nu
                  + __J_nu / __numeric_constants<_Tp>::__sqrt3()) / _Tp(2);

        __bessel_jn(_Tp(2) / _Tp(3), __z, __J_nu, __N_nu, __Jp_nu, __Np_nu);
        __Aip = __absx * (__N_nu / __numeric_constants<_Tp>::__sqrt3()
                        + __J_nu) / _Tp(2);
        __Bip = __absx * (__J_nu / __numeric_constants<_Tp>::__sqrt3()
                        - __N_nu) / _Tp(2);
      }
    else
      {
        //  Reference:
        //    Abramowitz & Stegun, page 446 section 10.4.4 on Airy functions.
        //  The number is Ai(0) = 3^{-2/3}/\Gamma(2/3).
        __Ai = _Tp(0.35502805388781723926L);
        __Bi = __Ai * __numeric_constants<_Tp>::__sqrt3();

        //  Reference:
        //    Abramowitz & Stegun, page 446 section 10.4.5 on Airy functions.
        //  The number is Ai'(0) = -3^{-1/3}/\Gamma(1/3).
        __Aip = -_Tp(0.25881940379280679840L);
        __Bip = -__Aip * __numeric_constants<_Tp>::__sqrt3();
      }

    return;
  }


/**
 *   @brief  Compute the Airy function of the first kind @f$ Ai(x) @f$.
 *
 *   @param  __x  The argument of the Airy function.
 *   @return  The Airy function.
 */
template<typename _Tp>
  _Tp
  __airy_ai(_Tp __x)
  {
    if (__isnan(__x))
      return std::numeric_limits<_Tp>::quiet_NaN();
    else
      {
        _Tp __Ai, __Bi, __Aip, __Bip;
        __airy(__x, __Ai, __Bi, __Aip, __Bip);
        return __Ai;
      }
  }


/**
 *   @brief  Compute the Airy function of the second kind @f$ Bi(x) @f$.
 *
 *   @param  __x  The argument of the Airy function.
 *   @return  The Airy function.
 */
template<typename _Tp>
  _Tp
  __airy_bi(_Tp __x)
  {
    if (__isnan(__x))
      return std::numeric_limits<_Tp>::quiet_NaN();
    else
      {
        _Tp __Ai, __Bi, __Aip, __Bip;
        __airy(__x, __Ai, __Bi, __Aip, __Bip);
        return __Bi;
      }
  }


} // __detail
} // namespace std

#endif // _GLIBCXX_MODIFIED_BESSEL_FUNC_TCC
