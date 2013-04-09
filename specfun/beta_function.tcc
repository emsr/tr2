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

#ifndef _GLIBCXX_BETA_FUNCTION_TCC
#define _GLIBCXX_BETA_FUNCTION_TCC 1


///  We'll pull this into the main (TR1) beta.tcc ultimately.


namespace __detail {


template<typename _Tp>
  _Tp
  __beta_inc_cont_frac(_Tp __a, _Tp __b, _Tp __x)
  {
    const unsigned int __itmax = 100;
    const _Tp __eps = std::numeric_limits<_Tp>::epsilon();

    _Tp __apb = __a + __b;
    _Tp __ap1 = __a + _Tp(1);
    _Tp __am1 = __a - _Tp(1);
    _Tp __c = _Tp(1);
    _Tp __d = _Tp(1) - __apb * __x / __ap1;
    if (std::abs(__d) < __eps)
      __d = __eps;
    __d = _Tp(1) / __d;
    _Tp __h = __d;
    for (unsigned int __m = 1; __m <= __itmax; ++__m)
      {
        _Tp __m2(2 * __m);

        _Tp __aa = _Tp(__m) * (__b - _Tp(__m)) * __x
                 / ((__am1 + __m2) * (__a + __m2));
        __d = _Tp(1) + __aa * __d;
        if (std::abs(__d) < __eps)
          __d = __eps;
        __c = _Tp(1) + __aa / __c;
        if (std::abs(__c) < __eps)
          __c = __eps;
        __d = _Tp(1) / __d;
        __h *= __d * __c;

        __aa = -(__a + _Tp(__m)) * (__apb + __m) * __x
             / ((__a + __m2) * (__ap1 + __m2));
        __d = _Tp(1) + __aa * __d;
        if (std::abs(__d) < __eps)
          __d = __eps;
        __c = _Tp(1) + __aa / __c;
        if (std::abs(__c) < __eps)
          __c = __eps;
        __d = _Tp(1) / __d;
        _Tp __del = __d * __c;
        __h *= __del;

        if (std::abs(__del - _Tp(1)) < __eps)
          return __h;
      }
    throw std::logic_error("__beta_inc_cont_frac");
  }


template<typename _Tp>
  _Tp
  __beta_inc(_Tp __a, _Tp __b, _Tp __x)
  {
    if (__isnan(__x) || __isnan(__a) || __isnan(__b))
      return std::numeric_limits<_Tp>::quiet_NaN();

    if (__x < _Tp(0) || __x > _Tp(1))
      throw std::domain_error("__beta_inc: x out of range");

    _Tp __fact;
    if (__x == _Tp(0) || __x == _Tp(1))
      __fact = _Tp(0);
    else
      __fact = std::exp(std::lgamma(__a + __b)
                      - std::lgamma(__a) - std::lgamma(__b)
                      + __a * std::log(__x) + __b * std::log(_Tp(1) - __x));

    if (__x < (__a + _Tp(1)) / (__a + __b + _Tp(2)))
      return __fact * __beta_inc_cont_frac(__a, __b, __x) / __a;
    else
      return _Tp(1) - __fact * __beta_inc_cont_frac(__b, __a, _Tp(1) - __x) / __b;
  }


} // __detail

#endif // _GLIBCXX_BETA_FUNCTION_TCC
