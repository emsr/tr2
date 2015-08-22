// -*- C++ -*-
// Integration utilities for the C++ library testsuite.
//
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
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.
//
// Ported from GSL by Jason Dick
// Originally written by Brian Gaugh
//
//This file implements Gauss-Kronrod integration with singularities
//Based upon gsl-1.9/integration/qags.c

#include <utility>
#include <limits>
#include <tuple>
#include <stdexcept>

#include "gauss_kronrad_quadrature.h"
#include "integration_workspace.h"
#include "extrapolation_table.h"

#ifndef ADAPTIVE_SINGULAR_QUADRATURE_H
#define ADAPTIVE_SINGULAR_QUADRATURE_H

namespace __gnu_test
{

  template<typename _Tp>
    inline bool
    test_positivity(_Tp result, _Tp resabs)
    {
      return(std::abs(result) >=
                (1 - 50 * std::numeric_limits<_Tp>::epsilon()) * resabs);
    }

  //  Throws appropriate error if errcode nonzero
  void check_error(int errcode);

  //  Integrate potentially singular function from a to b using recursive
  //  Gauss-Kronrod algorithm
  template<typename _FunTp, typename _Tp>
    std::pair<_Tp, _Tp>
    qags_integrate(const _FunTp &func,
                   _Tp a, _Tp b,
                   _Tp epsabs, _Tp epsrel,
                   size_t limit);

  //  Integrate function from -infinity to +infinity
  template<typename _FunTp, typename _Tp>
    std::pair<_Tp, _Tp>
    qagi_integrate(const _FunTp &func,
                   _Tp epsabs, _Tp epsrel,
                   size_t limit);

  //  Integrate function from -infinity to b
  template<typename _FunTp, typename _Tp>
    std::pair<_Tp, _Tp>
    qagil_integrate(const _FunTp &func, _Tp b,
                    _Tp epsabs, _Tp epsrel,
                    size_t limit);

  //  Integrate function from a to +infinity
  template<typename _FunTp, typename _Tp>
    std::pair<_Tp, _Tp>
    qagiu_integrate(const _FunTp &func,
                    _Tp a,
                    _Tp epsabs, _Tp epsrel,
                    size_t limit);

  template<typename _FunTp, typename _Tp>
    std::pair<_Tp, _Tp>
    qags_integrate(const _FunTp &func,
                   _Tp a, _Tp b,
                   _Tp epsabs, _Tp epsrel,
                   size_t limit)
    {
      const qk_intrule qkintrule = Gauss_Kronrad_21;

      _Tp area, errsum;
      _Tp res_ext, err_ext;
      _Tp result0, abserr0, resabs0, resasc0;
      _Tp tolerance;

      _Tp ertest = 0;
      _Tp error_over_large_intervals = 0;
      _Tp reseps = 0, abseps = 0, correc = 0;
      size_t ktmin = 0;
      int roundoff_type1 = 0, roundoff_type2 = 0, roundoff_type3 = 0;
      int error_type = 0, error_type2 = 0;

      size_t iteration = 0;

      bool positive_integrand = false;
      int extrapolate = 0;
      int disallow_extrapolation = 0;

      //  Initialize results.

      integration_workspace<_Tp> workspace(limit);

      //_Tp result = 0;
      //_Tp abserr = 0;

      //  Test on accuracy.

      if (epsabs <= 0
          && (epsrel < _Tp(50) * std::numeric_limits<_Tp>::epsilon() || epsrel < _Tp(0.5e-28)))
	throw std::logic_error("tolerance cannot be acheived with given epsabs"
                               " and epsrel in qags_integrate()");

      //  Perform the first integration.

      typedef std::tuple<_Tp&,_Tp&,_Tp&,_Tp&> ret_type;

      ret_type{result0,abserr0,resabs0,resasc0}
          = qk_integrate(func, a, b, qkintrule);

      workspace.set_initial(a, b, result0, abserr0);

      tolerance = std::max(epsabs, epsrel * std::abs(result0));

      if (abserr0 <= 100 * std::numeric_limits<_Tp>::epsilon() * resabs0 &&
          abserr0 > tolerance)
	throw std::runtime_error("cannot reach tolerance because of roundoff"
				 " error on first attempt in qags_integrate()");
      else if ((abserr0 <= tolerance && abserr0 != resasc0) || abserr0 == 0.0)
	return std::make_pair(result0, abserr0);
      else if (limit == 1)
	throw std::runtime_error("a maximum of one iteration was insufficient"
                            " in qags_integrate()");

      //  Initialization.

      extrapolation_table<_Tp> table;
      table.append(result0);

      area = result0;
      errsum = abserr0;

      res_ext = result0;
      err_ext = std::numeric_limits<_Tp>::max();

      positive_integrand = test_positivity (result0, resabs0);

      iteration = 1;

      do
	{
          size_t current_level;
          _Tp a1, b1, a2, b2;
          _Tp a_i, b_i, r_i, e_i;
          _Tp area1 = 0, area2 = 0, area12 = 0;
          _Tp error1 = 0, error2 = 0, error12 = 0;
          _Tp resasc1, resasc2;
          _Tp resabs1, resabs2;
          _Tp last_e_i;

          //  Bisect the subinterval with the largest error estimate.

          workspace.retrieve(a_i, b_i, r_i, e_i);

          current_level = workspace.current_level();

          a1 = a_i;
          b1 = 0.5 * (a_i + b_i);
          a2 = b1;
          b2 = b_i;

          ++iteration;

          ret_type{area1, error1, resabs1, resasc1}
              = qk_integrate(func, a1, b1, qkintrule);

          ret_type{area2, error2, resabs2, resasc2}
              = qk_integrate(func, a2, b2, qkintrule);

          area12 = area1 + area2;
          error12 = error1 + error2;
          last_e_i = e_i;

          //  Improve previous approximations to the integral and test for
          //  accuracy.

          //  We write these expressions in the same way as the original
          //  QUADPACK code so that the rounding errors are the same, which
          //  makes testing easier.

          errsum = errsum + error12 - e_i;
          area = area + area12 - r_i;

          tolerance = std::max(epsabs, epsrel*std::abs(area));

          if (resasc1 != error1 && resasc2 != error2)
            {
              _Tp delta = r_i - area12;

              if (std::abs(delta) <= 1.0e-5 * std::abs(area12)
				  && error12 >= 0.99 * e_i)
        	{
                  if (!extrapolate)
                    ++roundoff_type1;
                  else
                    ++roundoff_type2;
        	}
              if (iteration > 10 && error12 > e_i)
        	roundoff_type3++;
            }

          //  Test for roundoff and eventually set error flag.

          if (roundoff_type1 + roundoff_type2 >= 10 || roundoff_type3 >= 20)
            error_type = 2;       /* round off error */

          if (roundoff_type2 >= 5)
            error_type2 = 1;

          //  Set error flag in the case of bad integrand behaviour at
          //  a point of the integration range.

          if (workspace.subinterval_too_small (a1, a2, b2))
            error_type = 4;

          // Append the newly-created intervals to the list.
          workspace.update(a1, b1, area1, error1, a2, b2, area2, error2);

          if (errsum <= tolerance)
            {
              check_error(error_type);
              return std::make_pair(workspace.sum_results(), errsum);
            }

          if (error_type)
            break;

          if (iteration >= limit - 1)
            {
              error_type = 1;
              break;
            }

          if (iteration == 2)  //  Set up variables on first iteration.
            {
              error_over_large_intervals = errsum;
              ertest = tolerance;
              table.append(area);
              continue;
            }

          if (disallow_extrapolation)
            continue;

          error_over_large_intervals += -last_e_i;

          if (current_level < workspace.max_level())
            error_over_large_intervals += error12;

          if (!extrapolate)
            {
              //  Test whether the interval to be bisected next is the
	      //  smallest interval.

              if (workspace.large_interval())
        	continue;

              extrapolate = 1;
              workspace.set_nrmax(1);
            }

          if (!error_type2 && error_over_large_intervals > ertest)
            if (workspace.increase_nrmax())
              continue;

          //  Perform extrapolation.

          table.append(area);

          std::pair<_Tp&, _Tp&> qelg_res(reseps, abseps);
          qelg_res = table.qelg();

          ++ktmin;

          if (ktmin > 5 && err_ext < _Tp(0.001) * errsum)
            error_type = 5;

          if (abseps < err_ext)
            {
              ktmin = 0;
              err_ext = abseps;
              res_ext = reseps;
              correc = error_over_large_intervals;
              ertest = std::max(epsabs, epsrel * std::abs(reseps));
              if (err_ext <= ertest)
        	break;
            }

          //  Prepare bisection of the smallest interval.

          if (table.get_nn() == 1)
            disallow_extrapolation = 1;

          if (error_type == 5)
            break;

          //  Work on interval with largest error.

          workspace.reset_nrmax();
          extrapolate = 0;
          error_over_large_intervals = errsum;

	}
      while (iteration < limit);

      //result = res_ext;
      //abserr = err_ext;

      if (err_ext == std::numeric_limits<_Tp>::max())
	{
	  check_error(error_type);
	  return std::make_pair(workspace.sum_results(), errsum);
	}

      if (error_type || error_type2)
	{
          if (error_type2)
            err_ext += correc;

          if (error_type == 0)
            error_type = 3;

          if (res_ext != 0.0 && area != 0.0)
            {
              if (err_ext / std::abs(res_ext) > errsum / std::abs(area))
        	{
                  check_error(error_type);
                  return std::make_pair(workspace.sum_results(), errsum);
        	}
            }
          else if (err_ext > errsum)
            {
              check_error(error_type);
              return std::make_pair(workspace.sum_results(), errsum);
            }
          else if (area == 0.0)
            {
              check_error(error_type);
              throw std::runtime_error("Unknown error in qags_integrate()");
            }
	}

      //  Test on divergence.

      {
	_Tp max_area = std::max(std::abs(res_ext), std::abs(area));

	if (!positive_integrand && max_area < 0.01 * resabs0)
	{
          check_error(error_type);
          throw std::runtime_error("Unknown error in qags_integrate()");
	}
      }

      {
	_Tp ratio = res_ext / area;

	if (ratio < 0.01 || ratio > 100.0 || errsum > std::abs(area))
          error_type = 6;
      }

      check_error(error_type);
      throw std::runtime_error("Unknown error in qags_integrate()");
    }


  //  Throws appropriate error if errcode nonzero
  void
  check_error(int errcode)
  {
    if (errcode > 2)
      --errcode;
    switch(errcode)
      {
      case 0: break;
      case 1:
        throw std::runtime_error("number of iterations was insufficient"
				 " in qags_integrate()");
      case 2:
        throw std::runtime_error("cannot reach tolerance because of roundoff"
				 " error in qags_integrate()");
      case 3:
        throw std::runtime_error("bad integrand behavior found in the"
				 " integration interval in qags_integrate()");
      case 4:
        throw std::runtime_error("roundoff error detected in the extrapolation"
				 " table in qags_integrate()");
      case 5:
        throw std::runtime_error("integral is divergent, or slowly convergent"
				 " in qags_integrate()");
      default:
        throw std::runtime_error("could not integrate function in"
				 " qags_integrate()");
      }
  }


  template<typename _FunTp, typename _Tp>
    _Tp
    i_transform(const _FunTp &func, _Tp t)
    {
      _Tp x = (1 - t) / t;
      _Tp y = func(x) + func(-x);
      return (y / t) / t;
    }


  template<typename _FunTp, typename _Tp>
    std::pair<_Tp, _Tp>
    qagi_integrate(const _FunTp &func,
                   const _Tp epsabs,
                   const _Tp epsrel,
                   const size_t limit)
    {
      return qags_integrate([func](_Tp t){ return i_transform(func, t); },
	                    _Tp(0), _Tp(1), epsabs, epsrel, limit);
    }


  template<typename _FunTp, typename _Tp>
    _Tp
    il_transform(const _FunTp &func, _Tp b, _Tp t)
    {
      _Tp x = b - (1 - t) / t;
      _Tp y = func(x);
      return (y / t) / t;
    }


  template<typename _FunTp, typename _Tp>
    std::pair<_Tp, _Tp>
    qagil_integrate(const _FunTp &func, _Tp b,
                    const _Tp epsabs,
                    const _Tp epsrel,
                    const size_t limit)
    {
      return qags_integrate([func, b](_Tp t){ return il_transform(func, b, t); },
	                    _Tp(0), _Tp(1), epsabs, epsrel, limit);
    }


  template<typename _FunTp, typename _Tp>
    _Tp
    iu_transform(const _FunTp &func, _Tp a, _Tp t)
    {
      _Tp x = a + (1 - t) / t;
      _Tp y = func(x);
      return (y / t) / t;
    }


  template <class _FunTp, class _Tp>
    std::pair<_Tp, _Tp>
    qagiu_integrate(const _FunTp &func, _Tp a,
                    const _Tp epsabs,
                    const _Tp epsrel,
                    const size_t limit)
    {
      return qags_integrate([func, a](_Tp t){ return iu_transform(func, a, t); },
	                    _Tp(0), _Tp(1), epsabs, epsrel, limit);
    }

} // namespace __gnu_test

#endif // ADAPTIVE_SINGULAR_QUADRATURE_H
