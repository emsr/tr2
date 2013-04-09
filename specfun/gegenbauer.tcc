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

/** @file tbd/gegenbauer.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tbd/cmath}
 */

#ifndef _GLIBCXX_GEGENBAUER_TCC
#define _GLIBCXX_GEGENBAUER_TCC 1

namespace __detail {


template<typename _Tp>
  _Tp
  __gegenbauer_poly(unsigned int __n, _Tp __alpha, _Tp __x)
  {
    _Tp __c0 = _Tp(1);
    if (__n == 0)
      return __c0;

    _Tp __c1 = _Tp(2) * __alpha * __x;
    if (__n == 1)
      return __c1;

    _Tp __c(0);
    for (unsigned int __nn = 2; __nn <= __n; ++__nn)
      {
        __c = (_Tp(2) * (_Tp(__nn) - _Tp(1) + __alpha) * __x * __c1
                    - (_Tp(__nn) - _Tp(2) + _Tp(2) * __alpha) * __c0) / _Tp(__nn);
        __c0 = __c1;
        __c1 = __c;
      }
    return __c;
  }


}

#endif // _GLIBCXX_GEGENBAUER_TCC
