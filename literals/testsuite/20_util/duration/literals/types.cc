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

void
test03()
{
  using namespace std::literals::chrono_literals;

  static_assert(std::is_same<decltype(1h), std::chrono::hours>::value,
		"h is std::chrono::hours");

  static_assert(std::is_same<decltype(1min), std::chrono::minutes>::value,
		"min is std::chrono::minutes");

  static_assert(std::is_same<decltype(1s), std::chrono::seconds>::value,
		"s is std::chrono::seconds");

  static_assert(std::is_same<decltype(1ms), std::chrono::milliseconds>::value,
		"ms is std::chrono::milliseconds");

  static_assert(std::is_same<decltype(1us), std::chrono::microseconds>::value,
		"us is std::chrono::microseconds");

  static_assert(std::is_same<decltype(1ns), std::chrono::nanoseconds>::value,
		"ns is std::chrono::nanoseconds");
}
