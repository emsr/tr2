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
// Implements the integration_workspace class which stores temporary data
// for performing integrals
// Based upon gsl-1.9/integration/workspace.c

#include <vector>
#include <limits>
#include <cmath>

#ifndef INTEGRATION_WORKSPACE_H
#define INTEGRATION_WORKSPACE_H
namespace __gnu_test
{
  template<class _Tp>
    class integration_workspace
  {
  private:
    size_t limit;
    size_t size;
    size_t nrmax;
    size_t ii;
    size_t maximum_level;
    std::vector<_Tp> alist, blist, rlist, elist;
    std::vector<size_t> order, level;

    void qpsrt();

  public:

    integration_workspace(size_t lim)
    : limit(lim),
      size(0),
      nrmax(0),
      ii(0),
      maximum_level(0),
      alist(lim),
      blist(lim),
      rlist(lim),
      elist(lim),
      order(lim),
      level(lim)
    { }

    void
    set_initial(_Tp a0, _Tp b0, _Tp result0, _Tp error0)
    {
      alist[0] = a0;
      blist[0] = b0;
      rlist[0] = result0;
      elist[0] = error0;
      order[0] = 0;
      level[0] = 0;
      size = 1;
    }

    void
    update(_Tp a1, _Tp b1, _Tp area1, _Tp error1,
           _Tp a2, _Tp b2, _Tp area2, _Tp error2);

    void
    retrieve(_Tp &a, _Tp &b, _Tp &r, _Tp &e) const
    {
      a = alist[ii];
      b = blist[ii];
      r = rlist[ii];
      e = elist[ii];
    }

    bool
    increase_nrmax()
    {
      int id = nrmax;

      size_t last = size - 1;

      int jupbnd;
      if (last > (1 + limit / 2))
        jupbnd = limit + 1 - last;
      else
        jupbnd = last;

      for (int k = id; k <= jupbnd; ++k)
        {
          size_t i_max = order[nrmax];

          ii = i_max;

          if (level[i_max] < maximum_level)
            {
              return true;
            }

          nrmax++;

        }
      return false;
    }

    void
    reset_nrmax()
    {
      nrmax = 0;
      ii = order[0];
    }

    size_t
    get_nrmax() const
    { return nrmax; }

    void
    set_nrmax(size_t NRMAX)
    { nrmax = NRMAX; }

    _Tp
    sum_results() const
    {
      _Tp result_sum = 0;

      for (size_t kk = 0; kk < size; ++kk)
        result_sum += rlist[kk];

      return result_sum;
    }

    static bool
    subinterval_too_small(_Tp a1, _Tp a2, _Tp b2)
    {
      using namespace std;
      const _Tp e = std::numeric_limits<_Tp>::epsilon();
      const _Tp u = std::numeric_limits<_Tp>::min();

      _Tp tmp = (1 + 100 * e) * (std::abs(a2) + 1000 * u);

      return std::abs(a1) <= tmp && std::abs(b2) <= tmp;
    }

    size_t
    current_level() const
    { return level[ii]; }

    size_t
    max_level() const
    { return maximum_level; }

    bool
    large_interval() const
    {
      if (level[ii] < maximum_level)
        return true;
      else
        return false;
    }
  };

  template<class _Tp>
    void
    integration_workspace<_Tp>::qpsrt()
    {
      const size_t last = size - 1;

      double errmax;
      double errmin;
      int top;

      size_t i_nrmax = nrmax;
      size_t i_maxerr = order[i_nrmax];

      // Check whether the list contains more than two error estimates
      if (last < 2)
	{
          order[0] = 0;
          order[1] = 1;
          ii = i_maxerr;
          return;
	}

      errmax = elist[i_maxerr];

      /* This part of the routine is only executed if, due to a difficult
	integrand, subdivision increased the error estimate. In the normal
	case the insert procedure should start after the nrmax-th largest
	error estimate. */

      while (i_nrmax > 0 && errmax > elist[order[i_nrmax - 1]])
	{
          order[i_nrmax] = order[i_nrmax - 1];
          --i_nrmax;
	}

      /* Compute the number of elements in the list to be maintained in
	descending order. This number depends on the number of
	subdivisions still allowed. */

      if(last < (limit/2 + 2))
	top = last;
      else
	top = limit - last + 1;

      /* Insert errmax by traversing the list top-down, starting
	comparison from the element elist(order(i_nrmax+1)). */


      /* The order of the tests in the following line is important to
	prevent a segmentation fault */

      int jj = i_nrmax + 1;
      while (jj < top && errmax < elist[order[jj]])
	{
          order[jj - 1] = order[jj];
          jj++;
	}

      order[jj - 1] = i_maxerr;

      // Insert errmin by traversing the list bottom-up

      errmin = elist[last];

      int kk = top - 1;
      while (kk > jj - 2 && errmin >= elist[order[kk]])
	{
          order[kk + 1] = order[kk];
          --kk;
	}

      order[kk + 1] = last;

      // Set i_max and e_max

      i_maxerr = order[i_nrmax];

      ii = i_maxerr;
      nrmax = i_nrmax;
    }

  template<class _Tp>
    void
    integration_workspace<_Tp>::update(_Tp a1, _Tp b1,
                                         _Tp area1, _Tp error1,
                                         _Tp a2, _Tp b2,
                                         _Tp area2, _Tp error2)
    {
      const size_t i_max = ii;
      const size_t i_new = size;

      const size_t new_level = level[i_max] + 1;

      // append the newly-created intervals to the list

      if (error2 > error1)
	{
          alist[i_max] = a2;        // blist[maxerr] is already == b2
          rlist[i_max] = area2;
          elist[i_max] = error2;
          level[i_max] = new_level;

          alist[i_new] = a1;
          blist[i_new] = b1;
          rlist[i_new] = area1;
          elist[i_new] = error1;
          level[i_new] = new_level;
	}
      else
	{
          blist[i_max] = b1;        // alist[maxerr] is already == a1
          rlist[i_max] = area1;
          elist[i_max] = error1;
          level[i_max] = new_level;

          alist[i_new] = a2;
          blist[i_new] = b2;
          rlist[i_new] = area2;
          elist[i_new] = error2;
          level[i_new] = new_level;
	}

      ++size;

      if (new_level > maximum_level)
	maximum_level = new_level;

      qpsrt();
    }

} // namespace

#endif // 
