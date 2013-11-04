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

#include <new>
#include <string_view>
#include <stdexcept>
#include <testsuite_hooks.h>

void
test01()
{
  bool test [[gnu::unused]] = true;
  typedef std::string_view::size_type csize_type;
  typedef std::string_view::iterator citerator;
  csize_type npos = std::string_view::npos;
  csize_type csz01;

  const char str_lit01[] = "rodeo beach, marin";
  const std::string_view str01(str_lit01);
  const std::string_view str02("baker beach, san francisco");

  // basic_string_view(const string_view&, size_type pos = 0, size_type n = npos)
  csz01 = str01.size();
  try {
    std::string_view str03(str01, csz01 + 1);
    VERIFY( false );
  }		 
  catch(std::out_of_range& fail) {
    VERIFY( true );
  }
  catch(...) {
    VERIFY( false );
  }

  try {
    std::string_view str03(str01, csz01);
    VERIFY( str03.size() == 0 );
    VERIFY( str03.size() <= str03.capacity() );
  }		 
  catch(...) {
    VERIFY( false );
  }

  // basic_string_view(const char* s, size_type n)
  csz01 = str01.max_size();
  // NB: As strlen(str_lit01) != csz01, this test is undefined. It
  // should not crash, but what gets constructed is a bit arbitrary.
  try {
    std::string_view str03(str_lit01, csz01 + 1);
    VERIFY( true );
  }		 
  catch(std::length_error& fail) {
    VERIFY( true );
  }
  catch(...) {
    VERIFY( false );
  }

  // NB: As strlen(str_lit01) != csz01, this test is undefined. It
  // should not crash, but what gets constructed is a bit arbitrary.
  // The "maverick's" of all string_view objects.
  try {
    std::string_view str04(str_lit01, npos); 
    VERIFY( true );
  }		 
  catch(std::length_error& fail) {
    VERIFY( true );
  }
  catch(...) {
    VERIFY( false );
  }

  // Build a maxsize - 1 lengthed string_view consisting of all A's
  try {
    std::string_view str03(csz01 - 1, 'A');
    VERIFY( str03.size() == csz01 - 1 );
    VERIFY( str03.size() <= str03.capacity() );
  }		 
  // NB: bad_alloc is regrettable but entirely kosher for
  // out-of-memory situations.
  catch(std::bad_alloc& fail) {
    VERIFY( true );
  }
  catch(...) {
    VERIFY( false );
  }

  // basic_string_view(const char* s)
  std::string_view str04(str_lit01);
  VERIFY( str01 == str04 );


  // basic_string_view(size_type n, char c)
  csz01 = str01.max_size();
  try {
    std::string_view str03(csz01 + 1, 'z');
    VERIFY( false );
  }		 
  catch(std::length_error& fail) {
    VERIFY( true );
  }
  catch(...) {
    VERIFY( false );
  }

  try {
    std::string_view str04(npos, 'b'); // the "maverick's" of all string_view objects.
    VERIFY( false );
  }		 
  catch(std::length_error& fail) {
    VERIFY( true );
  }
  catch(...) {
    VERIFY( false );
  }
}

int
main()
{ 
  __gnu_test::set_memory_limits();
  test01();

  return 0;
}
