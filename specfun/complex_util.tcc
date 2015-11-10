// TR29124 math special functions -*- C++ -*-

// Copyright (C) 2015 Free Software Foundation, Inc.
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

/** @file bits/complex_util.tcc
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef _GLIBCXX_BITS_COMPLEX_UTIL_TCC
#define _GLIBCXX_BITS_COMPLEX_UTIL_TCC 1

#pragma GCC system_header

  /**
   *  @brief Carefully compute @c z1/z2 avoiding overflow
   *         and destructive underflow.
   *	   If the quotient can be successfully computed, @c true
   *	   is returned and the quotient is returned in @c z1dz2.
   *	   Otherwise, std::runtime_error is thrown.
   *
   *  @todo  In a better world optional might be a batter solution.
   *         This would be nothrow...
   *
   *  @param[in]  z1  Dividend
   *  @param[in]  z2  Divisor
   *  @param[out]  z1dz2  Quotient
   *  @return  @c true on success
   *  @throws  std::runtime_error on division overflow.
   */
  template<typename _Tp>
    bool
    __safe_div(std::complex<_Tp> __z1, std::complex<_Tp> __z2,
	       std::complex<_Tp> & __z1dz2)
    {
      //  Note that xhinf is a machine floating-point dependent constant
      //  set equal to half the largest available floating-point number.
      static constexpr _Tp __xhinf = _Tp(0.5) * std::numeric_limits<_Tp>::max();

      //  Separate real and imaginary parts of arguments
      auto __re1 = std::real(__z1);
      auto __im1 = std::imag(__z1);
      auto __re2 = std::real(__z2);
      auto __im2 = std::imag(__z2);

      //  Set up largest and smallest magnitudes needed
      auto __z1b = std::max(std::abs(__re1), std::abs(__im1));
      auto __z2b = std::abs(__re2);
      auto __z2ub = std::abs(__im2);

      if (__z2b < __z2ub)
	std::swap(__z2b, __z2ub);

      if (__z2b < _Tp(1) && __z1b > __z2b * __xhinf)
	std::__throw_runtime_error(__N("safe_div: "
				       "overflow in complex division"));

      __re1 /= __z1b;
      __im1 /= __z1b;
      __re2 /= __z2b;
      __im2 /= __z2b;
      auto __term = __z2ub / __z2b;
      auto __denom = _Tp(1) + __term * __term;
      auto __scale = __z1b / __z2b / __denom;
      auto __qr = (__re1 * __re2 + __im1 * __im2) * __scale;
      auto __qi = (__re2 * __im1 - __re1 * __im2) * __scale;
      __z1dz2 = std::complex<_Tp>{__qr, __qi};

      return true;
    }

#endif // _GLIBCXX_BITS_COMPLEX_UTIL_TCC
