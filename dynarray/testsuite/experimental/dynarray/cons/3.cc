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
#include <testsuite_allocator.h>
#include <testsuite_hooks.h>

// test range constructors and range-fill constructor
void
test03()
{
  bool test [[std::unused]] = true;
  const int A[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
  const int B[]{7, 7, 7, 7, 7};
  const int N = sizeof(A) / sizeof(int);
  const int M = sizeof(B) / sizeof(int);

  std::experimental::dynarray<int> da3(A, A + N);
  VERIFY(std::equal(da3.begin(), da3.end(), A));

  std::experimental::dynarray<int> da4(da3.begin(), da3.end());
  VERIFY(std::equal(da4.begin(), da4.end(), A));

  std::experimental::dynarray<int> da5(M, 7);
  VERIFY(std::equal(da5.begin(), da5.end(), B));
  VERIFY(std::equal(B, B + M, da5.begin()));
}

int
main()
{
  test03();
  return 0;
}
