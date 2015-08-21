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
// Written by Jason Dick.
//
// Includes classes for handling factorial tables.
// Both plain factorials and the logarithms of factorials are supported.

#include <vector>
#include <cmath>

#ifndef FACTORIAL_TABLE_H
#define FACTORIAL_TABLE_H

namespace __gnu_test
{

  //Look-up table for storing and obtaining factorials
  template<typename _Tp>
    class factorial_table
    {

    private:

      static std::vector<_Tp> _S_ftab;
      factorial_table() = delete;

    public:

      static _Tp
      get(size_t n)
      {
	if (n >= _S_ftab.size())
          add_factorials(n);
	return _S_ftab[n];
      }

      static void
      add_factorials(size_t max_fact)
      {
	if (_S_ftab.size() == 0)
          _S_ftab.push_back(_Tp(1));

	for (_Tp ii = _S_ftab.size(); ii <= _Tp(max_fact); ++ii)
          _S_ftab.push_back(ii * _S_ftab.back());
      }
    };

  template<typename _Tp>
    std::vector<_Tp> factorial_table<_Tp>::_S_ftab{};

  //  Look-up table for storing and obtaining logarithms of factorials
  template<class _Tp>
    class lnfactorial_table
    {

    private:

      static std::vector<long double> _S_lnftab;
      lnfactorial_table() = delete;

    public:

      static _Tp
      get(size_t n)
      {
	if (n >= _S_lnftab.size())
          add_factorials(n);

	return _S_lnftab[n];
      }

      static void
      add_factorials(size_t max_fact)
      {
	if (_S_lnftab.size() == 0)
          _S_lnftab.push_back(_Tp(0));

	for (_Tp ii = _S_lnftab.size(); ii <= _Tp(max_fact); ++ii)
          _S_lnftab.push_back(std::log(ii) + _S_lnftab.back());
      }
    };

  template<class _Tp>
    std::vector<long double> lnfactorial_table<_Tp>::_S_lnftab{};

  //  Aliases to easily access factorials as simple functions
  float
  factorialf(size_t n)
  { return factorial_table<float>::get(n); }

  double
  factoriald(size_t n)
  { return factorial_table<double>::get(n); }

  long double
  factorialld(size_t n)
  { return factorial_table<long double>::get(n); }

  int
  factoriali(size_t n)
  { return factorial_table<int>::get(n); }

  long
  factoriall(size_t n)
  { return factorial_table<long>::get(n); }

  long long
  factorialll(size_t n)
  { return factorial_table<long long>::get(n); }

  float
  lnfactorialf(size_t n)
  { return lnfactorial_table<float>::get(n); }

  double
  lnfactoriald(size_t n)
  { return lnfactorial_table<double>::get(n); }

  long double
  lnfactorialld(size_t n)
  { return lnfactorial_table<long double>::get(n); }

} // namespace __gnu_test

#endif // FACTORIAL_TABLE_H
