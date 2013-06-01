// { dg-options "-std=gnu++1y" }

// Copyright (C) 2013 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <complex>

void
test02()
{
  using namespace std::literals::complex_literals;

  static_assert(std::is_same<decltype(1i_f), std::complex<float>>::value,
		"i_f is std::complex<float>");

  static_assert(std::is_same<decltype(1i), std::complex<double>>::value,
		"i is std::complex<double>");

  static_assert(std::is_same<decltype(1il), std::complex<long double>>::value,
		"il is std::complex<long double>");
}
