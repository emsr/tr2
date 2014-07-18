// C++ experimantal <filesystem> -*- C++ -*-

// Copyright (C) 2011-2014 Free Software Foundation, Inc.
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

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file experimental/filesystem.tcc
 *  This is an experimental C++ Library header.
 */

#ifndef _GLIBCXX_EXPERIMENTAL_FILESYSTEM_TCC
#define _GLIBCXX_EXPERIMENTAL_FILESYSTEM_TCC 1

#pragma GCC system_header

#if __cplusplus <= 201103L
# include <bits/c++14_warning.h>
#else

#include <iomanip> // for quoted

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace experimental
{
namespace filesystem
{
inline namespace v1
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION


  template<typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os, const path& __pth)
    { return __os << std::quoted(__pth.string(), '"', '&'); }

  template<typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is, path& __pth)
    {
      __pth.clear();
      std::string __pathname;

      __is >> std::quoted(__pathname, '"', '&');

      __pth = path{__pathname};

      return __is;
    }

_GLIBCXX_END_NAMESPACE_VERSION
} // inline namespace v1
} // namespace filesystem
} // namespace experimental
} // namespace std

#endif // __cplusplus <= 201103L

#endif // _GLIBCXX_EXPERIMENTAL_FILESYSTEM_TCC
