// { dg-do run }
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

// NOTE: This makes use of the fact that we know how moveable
// is implemented on string (via swap). If the implementation changed
// this test may begin to fail.

#include <chrono>
//#include <testsuite_hooks.h>
#include <cassert>
#define VERIFY(x) assert(x)

void
test03()
{
  using namespace std::literals::chrono_literals;

  auto jiffy = 23.0ns;
  auto blip = 14.0us;
  auto bit = 77.0ms;
  auto warmup = 33.0s;
  auto classtime = 50.0min;
  auto longtime = 1.0h + 30.0min;
  auto workday = 8.0h;

  VERIFY( jiffy == 23 * std::chrono::nanoseconds );
  VERIFY( blip == 14 * std::chrono::microseconds );
  VERIFY( bit == 77 * std::chrono::milliseconds );
  VERIFY( warmup == 33 * std::chrono::seconds );
  VERIFY( classtime == 50 * std::chrono::minutes );
  VERIFY( longtime == 90 * std::chrono::minutes );
  VERIFY( workday == 8 * std::chrono::hours );
}

int
main()
{
  test03();
}
