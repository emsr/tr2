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

// 21.3.6.8 basic_string_view::compare [lib.string_view::compare]

#include <string_view>
#include <testsuite_hooks.h>

// libstdc++/13650
void test01()
{
  using namespace std;
  bool test [[gnu::unused]] = true;

  const wchar_t lit_01[] = { L'w', L'e', L'\0', L'r', L'd' };
  const wchar_t lit_02[] = { L'w', L'e', L'i', L'\0', L'd' };

  const wchar_t lit_ref_a[] = { L'w', L'e', L'\0', L'q', L'd' };
  const wstring_view str_a(lit_ref_a, 5);
  VERIFY( str_a.compare(0, 5, lit_01, 5) < 0 );

  const wchar_t lit_ref_b[] = { L'w', L'e', L'i' };
  const wstring_view str_b(lit_ref_b, 3);
  VERIFY( str_b.compare(0, 3, lit_02, 5) < 0 );
}

int main()
{
  test01();
  return 0;
}
