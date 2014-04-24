// { dg-do compile }
// { dg-options "-std=gnu++14" }

// Copyright (C) 2014 Free Software Foundation, Inc.
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

#include <experimental/dynarra>
#include <testsuite_allocator.h>

using namespace __gnu_test;

int
main()
{
  using Container = std::experimental::dynarray<int>;
  const int arr10[10]{ 2, 4, 1, 7, 3, 8, 10, 5, 9, 6 };
  bool ok = true;

  tracker_allocator_counter::reset();
  {
    Container c{std::allocator_arg_t, tracker_allocator<int>,
		arr10, arr10 + 10};
    ok = check_construct_destroy("Construct from range", 10, 0) && ok;
  }
  ok = check_construct_destroy("Construct from range", 10, 10) && ok;

  tracker_allocator_counter::reset();
  {
    Container c{std::allocator_arg_t, tracker_allocator<int>,
		{2, 4, 1}};
    ok = check_construct_destroy("Construct from init-list", 3, 0) && ok;
    ok &= (c[0] == 2);
    ok &= (c[1] == 4);
  }
  ok = check_construct_destroy("Construct from init-list", 3, 3) && ok;

  return ok ? 0 : 1;
}

