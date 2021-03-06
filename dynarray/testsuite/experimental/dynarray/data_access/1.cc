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

#include <experimental/dynarray>
#include <testsuite_hooks.h>

void
test01() 
{ 
  bool test [[gnu::unused]] = true;
  using dynarray_type = std::experimental::dynarray<int>;

  {
    const int A[]{0, 1, 2, 3, 4};    
    dynarray_type da{0, 1, 2, 3, 4};
    VERIFY(da.data() == &da.front());
    int* pi = da.data();
    VERIFY(*pi == 0);
  }

  {
    const int A[]{4, 3, 2, 1, 0};    
    const dynarray_type cda{4, 3, 2, 1, 0};
    VERIFY(cda.data() == &cda.front());
    const int* pci = cda.data();
    VERIFY(*pci == 4);
  }
}

int
main()
{
  test01();
  return 0;
}
