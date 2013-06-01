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

#include <string>

void
test01()
{
  using namespace std::literals::string_literals;

  static_assert(std::is_same<decltype("hallo"s), std::string>::value,
		"s is std::string");

  static_assert(std::is_same<decltype(u8"hallo"s), std::string>::value,
		"u8 s is std::string");

  static_assert(std::is_same<decltype(L"hallo"s), std::wstring>::value,
		"L s is std::wstring");

  static_assert(std::is_same<decltype(u"hallo"s), std::u16string>::value,
		"u s is std::u16string");

  static_assert(std::is_same<decltype(U"hallo"s), std::u32string>::value,
		"U s is std::u32string");
}
