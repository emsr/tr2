// $HOME/bin/bin/g++ -std=c++14 -o is_final is_final.cpp

#include<type_traits>
#include<iostream>

namespace std
{

#define __cpp_lib_is_final 201402L

  /// is_final
  template<typename _Tp>
    struct is_final
    : public integral_constant<bool, __is_final(_Tp)>
    { };

  /// is_final
  template<bool>
    struct __is_final_helper
    : public false_type { };

  template<>
    struct __is_final_helper<true>
    : public true_type { };

  //template<typename _Tp>
  //  struct is_final
  //  : public __is_final_helper<__is_final(typename remove_cv<_Tp>::type)>::type
  //  { };
}

class C1 {};

class C2 : C1 {};

class Cn final : C2 {};

int
main()
{
  std::cout << "std::is_final<C1>::value = " << std::is_final<C1>::value << '\n';
  std::cout << "std::is_final<C2>::value = " << std::is_final<C2>::value << '\n';
  std::cout << "std::is_final<Cn>::value = " << std::is_final<Cn>::value << '\n';
}

/* I've got a patch in gcc_git
   I need to resyc git.
   Also the N4051 branch can be removed because that's in.

//--------------------- 20_util/is_final/value.cc ----------------------//
// { dg-options "-std=gnu++14" }
// { dg-do compile }

// Copyright (C) 2011-2014 Free Software Foundation, Inc.
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

#include <type_traits>
#include <testsuite_tr1.h>

void test01()
{
  using std::is_final;
  using namespace __gnu_test;

  // Positive test.
  static_assert(test_category<is_final, FinalType>(true), "");

  // Negative tests.
  static_assert(test_category<is_final, ClassType>(false), "");
  static_assert(test_category<is_final, DerivedType>(false), "");
}
//--------------------- 20_util/is_final/value.cc ----------------------//
//--------------------- 20_util/is_final/requirements/explicit_instantiation.cc ----------------------//
// { dg-options "-std=gnu++14" }
// { dg-do compile }

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

// NB: This file is for testing type_traits with NO OTHER INCLUDES.

#include <type_traits>

namespace std
{
  typedef short test_type;
  template struct is_final<test_type>;
}
//--------------------- 20_util/is_final/requirements/explicit_instantiation.cc ----------------------//
//--------------------- 20_util/is_final/requirements/typedefs.cc ----------------------//
// { dg-options "-std=gnu++14" }
// { dg-do compile }

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

// 
// NB: This file is for testing type_traits with NO OTHER INCLUDES.

#include <type_traits>

void test01()
{
  // Check for required typedefs
  typedef std::is_final<int> test_type;
  typedef test_type::value_type               value_type;
  typedef test_type::type                     type;
  typedef test_type::type::value_type         type_value_type;
  typedef test_type::type::type               type_type;
}
//--------------------- 20_util/is_final/requirements/typedefs.cc ----------------------//
*/
