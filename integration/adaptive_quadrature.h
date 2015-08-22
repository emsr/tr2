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
//Implements integration using a recursive Gauss-Kronrod algorithm
//Based upon gsl-1.9/integration/qag.c

#include <utility>
#include <limits>
#include <string>
#include <sstream>
#include <stdexcept>

#include "gauss_kronrad_quadrature.h"
#include "integration_workspace.h"

#ifndef ADAPTIVE_QUADRATURE_H
#define ADAPTIVE_QUADRATURE_H
namespace __gnu_test
{
  // Integrates function func from a to b
  // epsabs is the limit on absolute error
  // epsrel is the limit on relative error
  // Once either the absolute or relative error limit is reached,
  // qag_integrate() returns
  // max_iter is the maximum number of integration steps allowed
  // qksz is the size of the Gauss-Kronrod integration
  // (Gauss_Kronrad_15, Gauss_Kronrad_21, Gauss_Kronrad_31, Gauss_Kronrad_41,
  // Gauss_Kronrad_51, or Gauss_Kronrad_61)
  // Returns a pair with the first value being the integration result,
  // and the second value being the estimated error.
  template <class _Tp, class _FunTp>
    std::pair<_Tp, _Tp>
    qag_integrate(const _FunTp &func, _Tp a, _Tp b,
                  _Tp epsabs, _Tp epsrel, const size_t max_iter,
                  const qk_intrule qkintrule)
    {
      _Tp area, errsum;
      _Tp result0, abserr0, resabs0, resasc0;
      _Tp tolerance;
      size_t iteration = 0;
      int roundoff_type1 = 0, roundoff_type2 = 0, error_type = 0;

      _Tp round_off;

      _Tp result = 0;
      _Tp abserr = 0;

      std::vector<_Tp> rlist(max_iter);
      std::vector<_Tp> elist(max_iter);

      if (epsabs <= 0 && (epsrel < 50 * std::numeric_limits<_Tp>::epsilon()))
	throw std::logic_error("tolerance cannot be achieved in qag_integrate()"
                               " with given absolute and relative error limits");

      using ret_type = std::tuple<_Tp&, _Tp&, _Tp&, _Tp&>;

      ret_type{result0,abserr0,resabs0,resasc0}
          = qk_integrate(func, a, b, qkintrule);

      rlist[0] = result0;
      elist[0] = abserr0;

      //Test on accuracy
      tolerance = std::max(epsabs, epsrel * std::abs(result0));

      //Compute roundoff
      round_off = 50 * std::numeric_limits<_Tp>::epsilon() * resabs0;

      if (abserr0 <= round_off && abserr0 > tolerance)
	{
          result = result0;
          abserr = abserr0;

          throw std::runtime_error("Cannot reach tolerance because of roundoff error"
                                   " on first attempt in qag_integrate()");
	}
      else if ((abserr0 <= tolerance && abserr0 != resasc0) || abserr0 == 0.0)
	{
          result = result0;
          abserr = abserr0;

          return std::make_pair(result, abserr);
	}
      else if (max_iter == 1)
	{
          result = result0;
          abserr = abserr0;

          throw std::runtime_error("a maximum of one iteration was insufficient");
	}

      area = result0;
      errsum = abserr0;

      iteration = 1;

      integration_workspace<_Tp> workspace(max_iter);
      workspace.set_initial(a, b, result0, abserr0);
      result = workspace.sum_results();

      do
	{
          _Tp a1, b1, a2, b2;
          _Tp a_i, b_i, r_i, e_i;
          _Tp area1 = 0, area2 = 0, area12 = 0;
          _Tp error1 = 0, error2 = 0, error12 = 0;
          _Tp resasc1, resasc2;
          _Tp resabs1, resabs2;

          // Bisect the subinterval with the largest error estimate

          workspace.retrieve(a_i, b_i, r_i, e_i);

          a1 = a_i;
          b1 = 0.5 * (a_i + b_i);
          a2 = b1;
          b2 = b_i;

          ret_type{area1,error1,resabs1,resasc1}
              = qk_integrate(func, a1, b1, qkintrule);

          ret_type{area2,error2,resabs2,resasc2}
              = qk_integrate(func, a2, b2, qkintrule);

          area12 = area1 + area2;
          error12 = error1 + error2;

          errsum += (error12 - e_i);
          area += area12 - r_i;

          if (resasc1 != error1 && resasc2 != error2)
            {
              _Tp delta = r_i - area12;

              if (std::abs(delta) <= _Tp(1.0e-5) * std::abs(area12) && error12 >= _Tp(0.99) * e_i)
        	++roundoff_type1;
              if (iteration >= 10 && error12 > e_i)
        	++roundoff_type2;
            }

          tolerance = std::max(epsabs, epsrel * std::abs(area));

          if (errsum > tolerance)
            {
              if (roundoff_type1 >= 6 || roundoff_type2 >= 20)
        	error_type = 2;   /* round off error */

              //  Set error flag in the case of bad integrand behaviour at
	      //  a point of the integration range.

              if (workspace.subinterval_too_small(a1, a2, b2))
        	error_type = 3;
            }

          workspace.update(a1, b1, area1, error1, a2, b2, area2, error2);

          workspace.retrieve(a_i, b_i, r_i, e_i);

          result = workspace.sum_results();

          ++iteration;
	}
      while (iteration < max_iter && !error_type && errsum > tolerance);

      result = workspace.sum_results();
      abserr = errsum;

      if (errsum <= tolerance)
	return std::make_pair(result, abserr);
      else if (error_type == 2)
	throw std::runtime_error("roundoff error prevents tolerance from being"
				 " achieved in qag_integrate()");
      else if (error_type == 3)
	throw std::runtime_error("bad integrand behavior found in integrand"
				 " inteveral in qag_integrate()");
      else if (iteration == max_iter)
	throw std::runtime_error("maximum number of iterations reached in"
				 " qag_integrate()");
      else
	throw std::runtime_error("could not integrate function in"
				 " qag_integrate()");
    }

} // namespace __gnu_test

#endif // ADAPTIVE_QUADRATURE_H
