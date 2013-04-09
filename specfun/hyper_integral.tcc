// Special functions -*- C++ -*-

// Copyright (C) 2013 Free Software Foundation, Inc.
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

/** @file tbd/.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tbd/cmath}
 */

#ifndef _GLIBCXX_HYPER_INTEGRAL_TCC
#define _GLIBCXX_HYPER_INTEGRAL_TCC 1


#include <tr1/cmath>
#include <complex>
#include <stdexcept>


///
///  @brief This routine computes the hyperbolic cosine @f$ Chi(x) @f$
///         and hyperbolic sine @f$ Shi(x) @f$ integrals
///         by continued fraction for positive argument.
///

////FIXME!!!!
template<typename _Tp>
  void
  __chshint_cont_frac(_Tp __t, _Tp& __chi, _Tp& __shi)
  {
    const unsigned int __max_iter = 100;
    const _Tp __eps = _Tp(5) * std::numeric_limits<_Tp>::epsilon();
    const _Tp __fp_min = std::numeric_limits<_Tp>::min();
    const _Tp __pi_2 = std::tr1::__detail::__numeric_constants<_Tp>::__pi_2();

    //  Evaluate Chi and Shi by Lentz's modified method of continued fracions.
    std::complex<_Tp> __b(_Tp(1), __t);
    std::complex<_Tp> __c(_Tp(1) / __fp_min);
    std::complex<_Tp> __d(_Tp(1) / __b);
    std::complex<_Tp> __h(__d);
    unsigned int i = 2;
    while (true)
      {
        _Tp __a = -(i - 1) * (i - 1);
        __b += _Tp(2);
        __d = _Tp(1) / (__a * __d + __b);
        __c = __b + __a / __c;
        std::complex<_Tp> __del = __c * __d;
        __h *= __del;
        if (std::abs(__del.real() - _Tp(1)) + std::abs(__del.imag()) < __eps)
          break;
        if (i > __max_iter)
          throw std::logic_error("Continued fraction evaluation failed in __chishi.");
        ++i;
      }
    __h *= std::polar(_Tp(1), -__t);
    __chi = -__h.real();
    __shi = __pi_2 + __h.imag();

    return;
  }


///
///  @brief This routine computes the hyperbolic cosine @f$ Chi(x) @f$
///         and hyperbolic sine @f$ Shi(x) @f$ integrals
///         by series summation for positive argument.
///
template<typename _Tp>
  void
  __chshint_series(_Tp __t, _Tp& __chi, _Tp& __shi)
  {
    const unsigned int __max_iter = 100;
    const _Tp __eps = _Tp(5) * std::numeric_limits<_Tp>::epsilon();
    const _Tp __fp_min = std::numeric_limits<_Tp>::min();
    const _Tp __gamma_e = std::tr1::__detail::__numeric_constants<_Tp>::__gamma_e();

    //  Evaluate Chi and Shi by series simultaneously.
    _Tp __sumc(0), __sums(0);
    if (__t * __t < __fp_min)
      {
        //  Avoid underflow.
        __sumc = _Tp(0);
        __sums = __t;
      }
    else
      {
        /*
         *    Evaluate Shi and Chi by series expansion.
         */
        _Tp __sum(0);
        _Tp __fact(1);
        bool __odd = true;
        unsigned int __k = 1;
        while (true)
          {
            __fact *= __t / __k;
            _Tp __term = __fact / __k;
            __sum += __term;
            _Tp __err = __term / std::abs(__sum);
            if (__odd)
              {
                __sums = __sum;
                __sum = __sumc;
              }
            else
              {
                __sumc = __sum;
                __sum = __sums;
              }
            if (__err < __eps)
              break;
            __odd = !__odd;
            ++__k;
            if (__k > __max_iter)
              throw std::logic_error("Series evaluation failed in __chishi.");
          }
      }
    __chi = __gamma_e + std::log(__t) + __sumc;
    __shi = __sums;

    return;
  }


///
///  @brief This routine returns the hyperbolic cosine @f$ Ci(x) @f$
///         and hyperbolic sine @f$ Si(x) @f$ integrals as a pair.
///
///  The hyperbolic cosine integral is defined by:
///  @f[
///      Chi(x) = \gamma_E + \log(x) + \int_0^x dt \frac{\cosh(t) - 1}{t}
///  @f]
///
///  The hyperbolic sine integral is defined by:
///  @f[
///      Shi(x) = \int_0^x dt \frac{\sinh(t)}{t}
///  @f]
///
template<typename _Tp>
  std::pair<_Tp, _Tp>
  __chshint(_Tp __x, _Tp& __chi, _Tp& __shi)
  {
    _Tp __t = std::abs(__x);
    if (__t == _Tp(0))
      {
        __chi = -std::numeric_limits<_Tp>::infinity();
        __shi = _Tp(0);
      }
    else if (__t > _Tp(2))
      __chshint_cont_frac(__t, __chi, __shi);
    else
      __chshint_series(__t, __chi, __shi);

    if (__x < _Tp(0))
      __shi = -__shi;

    return std::make_pair(__chi, __shi);
}

#endif // _GLIBCXX_HYPER_INTEGRAL_TCC
