// -*- C++ -*-
// Integration utilities for the C++ library testsuite.
//
// Copyright (C) 2011, 2012 Free Software Foundation, Inc.
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
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.
//
// Written by Jason Dick.
//
// Provides readable access to integration functions

#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "qag_integrate.h"
#include "qags_integrate.h"

#include <limits>

namespace __gnu_test
{

  //  Integrates a smooth function from a to b
  //  absolute_error_lim and relative_error_lim are the absolute and relative
  //   error limits.
  //  max_iter is the maximum number of iterations allowed
  //  qkintrule is the Gauss-Kronrod integration rule, and can be either:
  //  QK_15, QK_21, QK_31, QK_41, QK_51, QK_61
  //  Higher-order rules converge more rapidly for most functions,
  //  but may slow convergence for less well-behaved ones.
  template<class _FunTp, class _Tp>
    inline std::pair<_Tp, _Tp>
    integrate_smooth(const _FunTp &func, _Tp a, _Tp b,
    		     _Tp absolute_error_lim,
    		     _Tp relative_error_lim,
    		     const size_t max_iter=1024,
    		     const qk_intrule qkintrule=QK_61)
  {
    return qag_integrate(func, a, b, absolute_error_lim, relative_error_lim,
                        max_iter, qkintrule);
  }

  //  Recursive Gauss-Kronrod integration optimized for
  //  discontinuous or singular functions
  template<class _FunTp, class _Tp>
    inline std::pair<_Tp, _Tp>
    integrate_singular(const _FunTp &func, _Tp a, _Tp b,
                       _Tp absolute_error_lim,
                       _Tp relative_error_lim,
                       const size_t max_iter=1024)
  {
    return qags_integrate(func, a, b, absolute_error_lim, relative_error_lim,
                          max_iter);
  }

  //  Integrates function from -infinity to +infinity
  template<class _FunTp, class _Tp>
    inline std::pair<_Tp, _Tp>
    integrate_infinite(const _FunTp &func,
                       _Tp absolute_error_lim,
                       _Tp relative_error_lim,
                       const size_t max_iter=1024)
  {
    return qagi_integrate(func, absolute_error_lim, relative_error_lim,
                          max_iter);
  }

  //  Integrations function from -infinity to b
  template<class _FunTp, class _Tp>
    inline std::pair<_Tp, _Tp>
    integrate_from_infinity(const _FunTp &func, _Tp b,
                            _Tp absolute_error_lim,
                            _Tp relative_error_lim,
                            const size_t max_iter=1024)
  {
    return qagil_integrate(func, b, absolute_error_lim, relative_error_lim,
                          max_iter);
  }

  //  Integrations function from a to +infinity
  template<class _FunTp, class _Tp>
    inline std::pair<_Tp, _Tp>
    integrate_to_infinity(const _FunTp &func, _Tp a,
                          _Tp absolute_error_lim,
                          _Tp relative_error_lim,
                          const size_t max_iter=1024)
  {
    return qagiu_integrate(func, a, absolute_error_lim, relative_error_lim,
                          max_iter);
  }

  //  Integrates function, allows setting of limits as being +/- infinity
  template<class _FunTp, class _Tp>
    inline std::pair<_Tp, _Tp>
    integrate(const _FunTp &func, _Tp a, _Tp b,
              _Tp absolute_error_lim,
              _Tp relative_error_lim,
              const size_t max_iter=1024)
    {
      const _Tp infty = std::numeric_limits<_Tp>::infinity();

      if (std::isnan(a) || std::isnan(b))
	throw std::runtime_error("NaN detected in integration limits in integration()");

      if (a == -infty)
	{
	  if (b == infty) //Integration from -infinity to +infinity
            return integrate_infinite(func, absolute_error_lim, relative_error_lim,
                                      max_iter);
	  else if (b == -infty)
            throw std::runtime_error("Attempted to integrate from -infinity"
                        	" to -infinity in integrate()");
	  else //Integration from -infinity to finite value
            return integrate_from_infinity(func, b, absolute_error_lim,
                                	  relative_error_lim, max_iter);
	}
      else if (a == infty)
	{
	  if (b == infty)
            throw std::runtime_error("Attempted to integrate from +infinity"
				     " to +infinity in integrate()");
	  else if (b == -infty) //Integration from +infinity to -infinity,
	  {                     // call integrate_infinite() and flip sign
            auto res = integrate_infinite(func, absolute_error_lim,
                                          relative_error_lim,
                                          max_iter);
            return {-res.first, res.second};
	  }
	  else //Integration from +infinity to finite value,
	  {    // call integrate_to_infinity and flip sign
            auto res = integrate_to_infinity(func, b,
                                             absolute_error_lim,
                                             relative_error_lim,
                                             max_iter);
            return {-res.first, res.second};
	  }
	}
      else //a is finite
	{
	  if (b == infty)
            return integrate_to_infinity(func, a, absolute_error_lim,
                                	relative_error_lim, max_iter);
	  else if (b == -infty) //Integration from finite value to -infinity,
	  {                     // call integrate_from_infinity and flip sign
            auto res = integrate_from_infinity(func, a,
                                               absolute_error_lim,
                                               relative_error_lim,
                                               max_iter);
            return {-res.first, res.second};
	  }
	  else //Both a and b finite, call integrate_singular
            return integrate_singular(func, a, b, absolute_error_lim,
                                      relative_error_lim, max_iter);
	}
    }

} // namespace __gnu_test

#endif // INTEGRATION_H
