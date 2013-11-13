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

// http://gcc.gnu.org/ml/libstdc++/2002-06/msg00025.html
void
test04()
{
  bool test [[gnu::unused]] = true;

  std::string_view str01("portofino");

  std::string_view::reverse_iterator i1 = str01.rbegin();
  std::string_view::reverse_iterator i2 = str01.rend();
  std::string_view str02(i1, i2);
  VERIFY( str02 == "onifotrop" );
}

int
main()
{ 
  test04();

  return 0;
}