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

/** @file tbd/chebyshev.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tbd/cmath}
 */

#ifndef _GLIBCXX_CHEBYSHEV_TCC
#define _GLIBCXX_CHEBYSHEV_TCC 1


namespace __detail {


template<typename _Tp>
  _Tp
  __chebyshev_recur(unsigned int __n, _Tp __x, _Tp __c0, _Tp __c1)
  {
    _Tp __c = _Tp(0);
    for (unsigned int __j = 1; __j < __n; ++__j)
    {
      __c = _Tp(2) * __x * __c1 - __c0;
      __c0 = __c1;
      __c1 = __c;
    }
    return __c;
  }

template<typename _Tp>
  _Tp
  __chebyshev_t(unsigned int __n, _Tp __x)
  {
    _Tp __t0 = _Tp(1);
    if (__n == 0)
      return __t0;

    _Tp __t1 = __x;
    if (__n == 1)
      return __t1;

    return __chebyshev_recur(__n, __x, __t0, __t1);
  }

template<typename _Tp>
  _Tp
  __chebyshev_u(unsigned int __n, _Tp __x)
  {
    _Tp __u0 = _Tp(1);
    if (__n == 0)
      return __u0;

    _Tp __u1 = _Tp(2) * __x;
    if (__n == 1)
      return __u1;

    return __chebyshev_recur(__n, __x, __u0, __u1);
  }

template<typename _Tp>
  _Tp
  __chebyshev_v(unsigned int __n, _Tp __x)
  {
    _Tp __v0 = _Tp(1);
    if (__n == 0)
      return __v0;

    _Tp __v1 = _Tp(2) * __x - _Tp(1);
    if (__n == 1)
      return __v1;

    return __chebyshev_recur(__n, __x, __v0, __v1);
  }

template<typename _Tp>
  _Tp
  __chebyshev_w(unsigned int __n, _Tp __x)
  {
    _Tp __w0 = _Tp(1);
    if (__n == 0)
      return __w0;

    _Tp __w1 = _Tp(2) * __x + _Tp(1);
    if (__n == 1)
      return __w1;

    return __chebyshev_recur(__n, __x, __w0, __w1);
  }


}

#endif // _GLIBCXX_CHEBYSHEV_TCC
