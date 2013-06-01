// { dg-options "-std=gnu++1y" }
// { dg-do run }

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
//#include <testsuite_hooks.h>
#include <cassert>
#define VERIFY(x) assert(x)

void
test02()
{
  using namespace std::literals::complex_literals;

  std::complex<float> j1 = 1.0i_f;
  std::complex<double> j2 = 2.0i;
  std::complex<long double> j4 = 4.0il;

  VERIFY( j1 == std::complex<float>(0.0F, 1.0F) );
  VERIFY( j2 == std::complex<double>(0.0, 2.0) );
  VERIFY( j4 == std::complex<long double>(0.0L, 4.0L) );
}

int
main()
{
  test02();
}
