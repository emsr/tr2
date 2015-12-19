// Special functions -*- C++ -*-

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

/** @file bits/sf_fresnel.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{cmath}
 */

#ifndef _GLIBCXX_SF_FRESNEL_TCC
#define _GLIBCXX_SF_FRESNEL_TCC 1

#include <limits>
#include <complex>
#include <stdexcept>

namespace std
{
namespace __detail
{

  /**
   *  @brief This function returns the Fresnel cosine and sine integrals
   *    as a pair by series expansion for positive argument.
   */
  template <typename _Tp>
    void
    __fresnel_series(const _Tp __ax, _Tp & _C, _Tp & _S)
    {
      constexpr auto _S_max_iter = 100;
      constexpr auto _S_eps = _Tp(5) * std::numeric_limits<_Tp>::epsilon();
      constexpr auto _S_piO2 = _Tp(M_PI / 2);

      //  Evaluate S and C by series expansion.
      auto __sum = _Tp(0);
      auto _Ssum = _Tp(0);
      auto _Csum = __ax;
      auto __sign = _Tp(1);
      auto __fact = _S_piO2 * __ax * __ax;
      auto __odd = true;
      auto __term = __ax;
      auto __n = 3;
      auto __k = 0;
      for (__k = 1; __k <= _S_max_iter; ++__k)
	{
	  __term *= __fact / __k;
	  __sum += __sign * __term / __n;
	  _Tp __test = std::abs(__sum) * _S_eps;
	  if (__odd)
	    {
	      __sign = -__sign;
	      _Ssum = __sum;
	      __sum = _Csum;
	    }
	  else
	    {
	      _Csum = __sum;
	      __sum = _Ssum;
	    }

	  if (__term < __test)
	    break;

	  __odd = ! __odd;

	  __n += 2;
      }
      if (__k > _S_max_iter)
	std::__throw_runtime_error("fresnel_series: series evaluation failed");

      _C = _Csum;
      _S = _Ssum;

      return;
    }


  /**
   *  @brief This function computes the Fresnel cosine and sine integrals
   *    by continued fractions for positive argument.
   */
  template <typename _Tp>
    void
    __fresnel_cont_frac(const _Tp __ax, _Tp & _C, _Tp & _S)
    {
      constexpr auto _S_max_iter = 100;
      constexpr auto _S_eps = _Tp(5) * std::numeric_limits<_Tp>::epsilon();
      constexpr auto _S_fp_min = std::numeric_limits<_Tp>::min();
      constexpr auto _S_pi = _Tp(M_PI);

      //  Evaluate S and C by Lentz's complex continued fraction method.
      const auto __pix2 = _S_pi * __ax * __ax;
      std::complex<_Tp> __b(_Tp(1), -__pix2);
      std::complex<_Tp> __cc(_Tp(1) / _S_fp_min, _Tp(0));
      auto __h = _Tp(1) / __b;
      auto __d = __h;
      auto __n = -1;
      auto __k = 0;
      for (__k = 2; __k <= _S_max_iter; ++__k)
	{
	  __n += 2;
	  const auto __a = -_Tp(__n * (__n + 1));
	  __b += _Tp(4);
	  __d = _Tp(1) / (__a * __d + __b);
	  __cc = __b + __a / __cc;
	  const auto __del = __cc * __d;
	  __h *= __del;
	  if (std::abs(__del.real() - _Tp(1))
	    + std::abs(__del.imag()) < _S_eps)
	    break;
	}
      if (__k > _S_max_iter)
	std::__throw_runtime_error("fresnel_cont_frac: "
				 "continued fraction evaluation failed");

      __h *= std::complex<_Tp>(__ax, -__ax);
      auto __phase = std::polar(_Tp(1), __pix2/_Tp(2));
      auto __cs = std::complex<_Tp>(_Tp(0.5L), _Tp(0.5L))
		* (_Tp(1) - __phase * __h);
      _C = __cs.real();
      _S = __cs.imag();

      return;
    }


  /**
   *  @brief This function returns the Fresnel cosine and sine integrals
   *    as a pair.
   * 
   *  The Fresnel cosine integral is defined by:
   *  @f[
   *      C(x) = \int_0^x \cos(\frac{\pi}{2}t^2) dt
   *  @f]
   * 
   *  The Fresnel sine integral is defined by:
   *  @f[
   *      S(x) = \int_0^x \sin(\frac{\pi}{2}t^2) dt
   *  @f]
   */
  template <typename _Tp>
    std::pair<_Tp, _Tp>
    __fresnel(const _Tp __x)
    {

      constexpr auto _S_fp_min = std::numeric_limits<_Tp>::min();
      constexpr auto _S_x_min = _Tp(1.5L);

      auto _C = _Tp(0);
      auto _S = _Tp(0);

      const _Tp __ax = std::abs(__x);
      if (__ax < std::sqrt(_S_fp_min))
	{
	  _C = __ax;
	  _S = _Tp(0);
	}
      else if (__ax < _S_x_min)
	__fresnel_series(__ax, _C, _S);
      else
	__fresnel_cont_frac(__ax, _C, _S);

      if (__x < _Tp(0))
	{
	  _C = -_C;
	  _S = -_S;
	}

      return std::make_pair(_C, _S);
    }

} // namespace __detail
} // namespace std

#endif // _GLIBCXX_SF_FRESNEL_TCC
