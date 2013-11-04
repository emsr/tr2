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

// 21.3.1 basic_string_view constructors.

#include <string_view>
#include <testsuite_hooks.h>

void
test02()
{
  bool test [[gnu::unused]] = true;

  // template<typename _InputIter>
  //   basic_string_view(_InputIter begin, _InputIter end, const allocator& a)
  // where _InputIter is integral [21.3.1 para 15]
  std::string_view s(10,0);
  VERIFY( s.size() == 10 );
}

int
main()
{ 
  test02();

  return 0;
}
