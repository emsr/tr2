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
test03()
{
  bool test [[gnu::unused]] = true;
  const char* with_nulls = "This contains \0 a zero byte.";

  // These are tests to see how basic_string_view handles data with NUL
  // bytes.  Obviously basic_string_view(char*) will halt at the first one, but
  // nothing else should.
  std::string_view s1 (with_nulls, 28);
  VERIFY( s1.size() == 28 );
  std::string_view s2 (s1);
  VERIFY( s2.size() == 28 );

  // Not defined, but libstdc++ throws an exception.
  const char* bogus = 0;
  try 
    {
      std::string_view str1(bogus);
      VERIFY( false );
    }		 
  catch(std::exception& fail) 
    {
      VERIFY( true );
    }

  // Not defined, but libstdc++ throws an exception.
  try 
    {
      std::string_view str2(bogus, 5);
      VERIFY( false );
    }		 
  catch(std::exception& fail) 
    {
      VERIFY( true );
    }
}

int
main()
{ 
  test03();

  return 0;
}