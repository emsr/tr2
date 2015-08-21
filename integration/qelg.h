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
//This file implements an extrapolation table for use in integration schemes
//Based upon gsl-1.9/integration/qelg.c

#include <array>
#include <utility>
#include <limits>
#include <cmath>
#include <algorithm>

#ifndef QELG_H
#define QELG_H

namespace __gnu_test
{
  template<typename _Tp>
    class extrapolation_table
    {

    private:

      size_t nn;
      std::array<_Tp, 52> rlist2;
      size_t nres;
      std::array<_Tp, 3> res3la;

    public:

      extrapolation_table()
      : nn{},
	nres{}
      {}

      explicit
      extrapolation_table(_Tp y)
      : nn{},
	nres{}
      {
	rlist2[0] = y;
      }

      void
      append(_Tp y)
      {
	rlist2[nn] = y;
	++nn;
      }

      std::pair<_Tp, _Tp> qelg();

      size_t
      get_nn() const
      { return nn; }
    };

  template<typename _Tp>
    std::pair<_Tp, _Tp>
    extrapolation_table<_Tp>::qelg()
    {
      const size_t cur_n = nn - 1;
      const _Tp current = rlist2[cur_n];

      _Tp absolute = std::numeric_limits<_Tp>::max();
      _Tp relative = _Tp(5) * std::numeric_limits<_Tp>::epsilon() * std::abs(current);

      const size_t newelm = cur_n/2;
      const size_t n_orig = cur_n;
      size_t n_final = cur_n;
      size_t ii;

      const size_t nres_orig = nres;

      _Tp result = current;
      _Tp abserr = std::numeric_limits<_Tp>::max();

      if (cur_n < 2)
	{
          result = current;
          abserr = std::max(absolute, relative);
          return std::make_pair(result, abserr);
	}

      rlist2[cur_n + 2] = rlist2[cur_n];
      rlist2[cur_n] = std::numeric_limits<_Tp>::max();

      for (ii = 0; ii < newelm; ++ii)
	{
          _Tp res = rlist2[cur_n - 2 * ii + 2];
          _Tp e0 = rlist2[cur_n - 2 * ii - 2];
          _Tp e1 = rlist2[cur_n - 2 * ii - 1];
          _Tp e2 = res;

          _Tp e1abs = std::abs(e1);
          _Tp delta2 = e2 - e1;
          _Tp err2 = std::abs(delta2);
          _Tp tol2 = std::max(std::abs(e2), e1abs)*std::numeric_limits<_Tp>::epsilon();
          _Tp delta3 = e1 - e0;
          _Tp err3 = std::abs(delta3);
          _Tp tol3 = std::max(e1abs, std::abs(e0))*std::numeric_limits<_Tp>::epsilon();

          _Tp e3, delta1, err1, tol1, ss;

          if (err2 <= tol2 && err3 <= tol3)
            {
              /* If e0, e1 and e2 are equal to within machine accuracy,
        	convergence is assumed.  */

              result = res;
              absolute = err2 + err3;
              relative = 5*std::numeric_limits<_Tp>::epsilon()*std::abs(res);
              abserr = std::max(absolute, relative);
              return std::make_pair(result, abserr);
            }

          e3 = rlist2[cur_n - 2 * ii];
          rlist2[cur_n - 2 * ii] = e1;
          delta1 = e1 - e3;
          err1 = std::abs(delta1);
          tol1 = std::max(e1abs, std::abs(e3)) * std::numeric_limits<_Tp>::epsilon();

          /* If two elements are very close to each other, omit a part of
            the table by adjusting the value of n */

          if (err1 <= tol1 || err2 <= tol2 || err3 <= tol3)
            {
              n_final = 2 * ii;
              break;
            }

          ss = (1 / delta1 + 1 / delta2) - 1 / delta3;

          /* Test to detect irregular behaviour in the table, and
            eventually omit a part of the table by adjusting the value of
            n. */

          if (std::abs(ss * e1) <= _Tp(0.0001L))
            {
              n_final = 2 * ii;
              break;
            }

          /* Compute a new element and eventually adjust the value of
            result. */

          res = e1 + 1 / ss;
          rlist2[cur_n - 2 * ii] = res;

          {
            const _Tp error = err2 + std::abs(res - e2) + err3;

            if (error <= abserr)
              {
        	abserr = error;
        	result = res;
              }
          }
	}

      /* Shift the table */

      {
	const size_t limexp = 50 - 1;

	if (n_final == limexp)
          n_final = 2 * (limexp / 2);
      }

      if (n_orig % 2 == 1)
	{
          for (ii = 0; ii <= newelm; ++ii)
            rlist2[1 + ii * 2] = rlist2[ii * 2 + 3];
	}
      else
	{
          for (ii = 0; ii <= newelm; ++ii)
            rlist2[ii * 2] = rlist2[ii*2 + 2];
	}

      if (n_orig != n_final)
	{
          for (ii = 0; ii <= n_final; ++ii)
            rlist2[ii] = rlist2[n_orig - n_final + ii];
	}

      nn = n_final + 1;

      if (nres_orig < 3)
	{
          res3la[nres_orig] = result;
          abserr = std::numeric_limits<_Tp>::max();
	}
      else
	{
          abserr = (std::abs(result - res3la[2])
		  + std::abs(result - res3la[1])
		  + std::abs(result - res3la[0]));

          res3la[0] = res3la[1];
          res3la[1] = res3la[2];
          res3la[2] = result;
	}

      /* In QUADPACK the variable table->nres is incremented at the top of
	qelg, so it increases on every call. This leads to the array
	res3la being accessed when its elements are still undefined, so I
	have moved the update to this point so that its value more
	useful. */

      nres = nres_orig + 1;

      abserr = std::max(abserr, _Tp(5) * std::numeric_limits<_Tp>::epsilon() * std::abs(result));

      return std::make_pair(result, abserr);
    }

} // namespace __gnu_test

#endif // QELG_H
