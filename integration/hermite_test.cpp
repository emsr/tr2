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
// Provides test example of using integration suite with Hermite polynomials
// Written by Jason Dick

#define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <string>

//#include "simple_integrate.h"
#include "factorial_table.h"
#include "integration.h"

using namespace std;
using namespace __gnu_test;

//  Function which should integrate to 1 for n1 = n2, 0 otherwise.
template<typename _Tp>
  _Tp
  normalized_hermite(int n1, int n2, _Tp x)
  {
    _Tp lnnorm_fact = _Tp(0.5) * (log(M_PI) + (n1 + n2) * std::log(_Tp(2L))
                       + lnfactorialld(n1) + lnfactorialld(n2));
    return std::exp(-x * x - lnnorm_fact)
	 * std::hermite(n1, x)
         * std::hermite(n2, x);
  }

template<typename _Tp>
  void
  test_hermite()
  {
    constexpr _Tp infty = numeric_limits<_Tp>::infinity();

    for (int n1 = 1; ; ++n1)
    {
      for (int n2 = 0; n2 <= n1; ++n2)
      {
	auto func = [n1, n2](_Tp x){ return normalized_hermite<_Tp>(n1, n2, x); };
	_Tp integ_precision = std::numeric_limits<_Tp>::epsilon() * _Tp(1000);
	_Tp comp_precision = integ_precision * _Tp(10);
	_Tp integration_result, integration_error;

	typedef std::pair<_Tp&, _Tp&> ret_type;
	ret_type{integration_result, integration_error}
            = integrate(func, -infty, infty, integ_precision, _Tp());

	if (n1 == n2)
	{
          // integration_result should be 1, throw error if differs from one by
          // more than integration precision (with an additional fudge factor in
          // case integration isn't quite that accurate)
          if (std::abs(1.0 - integration_result) > comp_precision)
          {
            std::stringstream ss;
            ss.precision(-int(std::log10(std::numeric_limits<_Tp>::epsilon())));
            ss << "Integration failed at n1 = " << n1 << ", n2 = " << n2
               << ", returning result " << integration_result
               << " instead of the expected 1\n";
            throw std::logic_error(ss.str());
          }
	}
	else
	{
          // integration_result should be 0, throw error if differs from zero by
          // more than integration precision (with an additional fudge factor in
          // case integration isn't quite that accurate)
          if (std::abs(integration_result) > comp_precision)
          {
            std::stringstream ss;
            ss.precision(std::numeric_limits<_Tp>::digits10);
            ss << "Integration failed at n1 = " << n1 << ", n2 = " << n2
               << ", returning result " << integration_result
               << " instead of the expected 0\n";
            throw std::logic_error(ss.str());
          }
	}
      }
      std::cout << "Integration successful for hermite polynomials up to n = " << n1 << '\n';
    }
  }

 
int
main()
{
  try
    {
      test_hermite<double>();
    }
  catch(...)
    {}

  try
    {
      test_hermite<long double>();
    }
  catch(...)
    {}

}
