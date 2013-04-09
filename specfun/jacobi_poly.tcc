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

/** @file tbd/jacobi_poly.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tbd/cmath}
 */

#ifndef _GLIBCXX_JACOBI_POLY_TCC
#define _GLIBCXX_JACOBI_POLY_TCC 1

namespace __detail {


template<typename _Tp>
  _Tp
  __poly_jacobi(unsigned int __n, _Tp __alpha, _Tp __beta, _Tp __x)
  {
    _Tp __pm2 = _Tp(1);
    if (__n == 0)
      return __pm2;

    _Tp __apb = __alpha + __beta;
    _Tp __amb = __alpha - __beta;
    _Tp __pm1 = (__amb + (_Tp(2) + __apb) * __x) / _Tp(2);
    if (__n == 1)
      return __pm1;

    _Tp __p(0);
    for (unsigned int __j = 2; __j < __n; ++__j )
      {
        _Tp __japb = _Tp(__j) + __apb;
        _Tp __japb1 = __japb + _Tp(1);
        _Tp __japb2 = __japb1 + _Tp(1);
        _Tp __c = _Tp(2) * _Tp(__j + 1) * __japb * __japb1;
        _Tp __d = (__japb1) * __apb * __amb;
        _Tp __e = __japb * __japb1 * __japb2;
        _Tp __f = _Tp(2) * (__j + __alpha) * (__j + __beta) * __japb2;

        if (__c == _Tp(0))
          throw std::logic_error("Error in __poly_jacobi.");
        __p = ((__d + __e * __x) * __pm1 - __f * __pm2) / __c;
        __pm2 = __pm1;
        __pm1 = __p;
      }
    return __p;
  }


template<typename _Tp>
  _Tp
  __poly_radial_jacobi(unsigned int __n, unsigned int __m, _Tp __rho)
  {
    if (__m > __n)
      throw std::range_error("__poly_radial_jacobi: m > n");
    else if ((__n - __m) % 2 == 1)
      return _Tp(0);
    else
      {
        unsigned __k = (__n - __m) / 2;
        return (__k % 2 == 0 ? 1 : -1) * std::pow(__rho, __m)
             * __poly_jacobi(__k, _Tp(__m), _Tp(0),
                             _Tp(1) - _Tp(2) * __rho * __rho);
      }
  }


}

#endif // _GLIBCXX_JACOBI_POLY_TCC
