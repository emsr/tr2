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
#include <testsuite_hooks.h>
#include <testsuite_api.h>

void
test01()
{
  bool test [[gnu::unused]] = true;

  std::experimental::dynarray<__gnu_test::NonCopyConstructible> da(1000);
  VERIFY( std::distance(da.begin(), da.end()) == 1000 );
  for (auto& it : da)
    VERIFY( it == -1 );
}

int
main()
{
  test01();
  return 0;
}
