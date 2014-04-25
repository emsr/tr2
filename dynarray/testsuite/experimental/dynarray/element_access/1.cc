// { dg-options "-std=gnu++14" }

// Copyright (C) 2014 Free Software Foundation, Inc.
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

#include <experimental/dynarray>
#include <stdexcept>
#include <testsuite_hooks.h>

template<typename T>
  struct A { };

struct B { };

// http://gcc.gnu.org/ml/libstdc++/2000-09/msg00002.html
bool
test01()
{
  bool test [[gnu::unused]] = true;
  std::experimental::dynarray<A<B>> da01(1);
  std::experimental::dynarray<A<B>> da02(5);
  using size_type = std::experimental::dynarray<A<B>>::size_type;
  using reference = std::experimental::dynarray<A<B>>::reference;

  try
    {
      reference r01 [[gnu::unused]] = da01.at(6); 
      VERIFY(false); // Should not get here, as exception thrown.
    }
  catch(std::out_of_range& err)
    {
      VERIFY(true);
    }
  catch(...)
    {
      VERIFY(false);
    }
  return test;
}

int
main()
{
  test01();
  return 0;
}
