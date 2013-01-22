// TR2 <filesystem.tcc> -*- C++ -*-

// Copyright (C) 2012, 2013 Free Software Foundation, Inc.
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

/** @file tr2/filesystem.tcc
 *  This is an internal TR2 C++ Library header, included by other library headers.
 *  Do not attempt to use it directly. @headername{filesystem}
 */

#ifndef _GLIBCXX_TR2_FILESYSTEM_TCC
#define _GLIBCXX_TR2_FILESYSTEM_TCC 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @brief  Opens an external file.
   *  @param  __pth  The path of the file.
   *  @param  __mode  The open mode flags.
   *  @return  @c this on success, NULL on failure
   */
  template<typename _CharT, typename _Traits>
    typename basic_filebuf<_CharT, _Traits>::__filebuf_type*
    basic_filebuf<_CharT, _Traits>::
    open(const path& __pth, std::ios_base::openmode __mode)
    { return open(__pth.c_str(), __mode); }

  template<typename _CharT, typename _Traits>
    basic_ifstream<_CharT, _Traits>::
    basic_ifstream(const path& __pth, std::ios_base::openmode __mode
  	   = std::ios_base::in)
    : basic_ifstream(__pth.c_str(), __mode)
    { }
  template<typename _CharT, typename _Traits>
    void
    basic_ifstream<_CharT, _Traits>::
    open(const path& __pth, std::ios_base::openmode __mode
	 = std::ios_base::in)
    { open(__pth.c_str(), __mode); }

  template<typename _CharT, typename _Traits>
    basic_ofstream<_CharT, _Traits>::
    basic_ofstream(const path& __pth, std::ios_base::openmode __mode
		   = std::ios_base::out)
    : basic_ofstream(__pth.c_str(), __mode)
    { }
  template<typename _CharT, typename _Traits>
    void
    basic_ofstream<_CharT, _Traits>::
    open(const path& __pth,
	 std::ios_base::openmode __mode = std::ios_base::out)
    { open(__pth.c_str(), __mode); }

  template<typename _CharT, typename _Traits>
    basic_fstream<_CharT, _Traits>::
    basic_fstream(const path& __pth, std::ios_base::openmode __mode
		  = std::ios_base::in | std::ios_base::out)
    : basic_fstream(__pth.c_str(), __mode)
    { }
  template<typename _CharT, typename _Traits>
    void
    basic_fstream<_CharT, _Traits>::
    open(const path& __pth,
	 std::ios_base::openmode __mode = std::ios_base::in | std::ios_base::out)
    { open(__pth.c_str(), __mode); }

} // std
