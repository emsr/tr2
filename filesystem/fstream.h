// C++ experimantal fstream.h -*- C++ -*-

// Copyright (C) 2011, 2012, 2013 Free Software Foundation, Inc.
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

/** @file tr2/fstream.h
 *  This is an experimental C++ Library header.
 */

#ifndef _GLIBCXX_EXPTL_FSTREAM_H
#define _GLIBCXX_EXPTL_FSTREAM_H 1

#pragma GCC system_header

#include "filesystem"
#include <fstream>

namespace std {
namespace experimental {
namespace filesystem {

  template <typename _CharT, typename _Traits = std::char_traits<_CharT>>
    class basic_filebuf : public std::basic_filebuf<_CharT, _Traits>
    {
    public:

      basic_filebuf()
      { }

      basic_filebuf(const basic_filebuf&) = delete;

      virtual
      ~basic_filebuf()
      { }

      const basic_filebuf& 
      operator=(const basic_filebuf&) = delete;

      basic_filebuf<_CharT, _Traits>*
      open(const path& __pth, std::ios_base::openmode __mode) 
      {
	return std::basic_filebuf<_CharT, _Traits>::open(__pth.c_str(), __mode);
      }
    };

  template <typename _CharT, typename _Traits = std::char_traits<_CharT>>
    class basic_ifstream : public std::basic_ifstream<_CharT, _Traits>
    {
    public:

      basic_ifstream()
      { }

      basic_ifstream(const basic_ifstream&) = delete;

      basic_ifstream(const path& __pth,
		     std::ios_base::openmode __mode = std::ios_base::in)
      : std::basic_ifstream<_CharT, _Traits>(__pth.c_str(), __mode)
      { }

      void
      open(const path& __pth,
	   std::ios_base::openmode __mode = std::ios_base::in)
      { std::basic_ifstream<_CharT, _Traits>::open(__pth.c_str(), __mode); }

      virtual
      ~basic_ifstream()
      { }

      const basic_ifstream&
      operator=(const basic_ifstream&) = delete;
    };

  template <typename _CharT, typename _Traits = std::char_traits<_CharT>>
    class basic_ofstream : public std::basic_ofstream<_CharT, _Traits>
    {
    public:

      basic_ofstream()
      { }

      basic_ofstream(const basic_ofstream&) = delete;

      explicit
      basic_ofstream(const path& __pth,
		     std::ios_base::openmode __mode = std::ios_base::out)
      : std::basic_ofstream<_CharT, _Traits>(__pth.c_str(), __mode)
      { }

      void
      open(const path& __pth,
	   std::ios_base::openmode __mode = std::ios_base::out)
      { std::basic_ofstream<_CharT, _Traits>::open(__pth.c_str(), __mode); }

      virtual
      ~basic_ofstream()
      { }

      const basic_ofstream&
      operator=(const basic_ofstream&) = delete;
    };

  template <typename _CharT, typename _Traits = std::char_traits<_CharT>>
    class basic_fstream : public std::basic_fstream<_CharT, _Traits>
    {
    public:

      basic_fstream()
      { }

      basic_fstream(const basic_fstream&) = delete;

      explicit
      basic_fstream(const path& __pth,
		    std::ios_base::openmode __mode
			   = std::ios_base::in | std::ios_base::out)
      : std::basic_fstream<_CharT, _Traits>(__pth.c_str(), __mode)
      { }

      void
      open(const path& __pth,
	   std::ios_base::openmode __mode
			   = std::ios_base::in | std::ios_base::out)
      { std::basic_fstream<_CharT, _Traits>::open(__pth.c_str(), __mode); }

      virtual
      ~basic_fstream()
      { }

      const basic_fstream&
      operator=(const basic_fstream&) = delete;
    };

  typedef basic_filebuf<char> filebuf;
  typedef basic_ifstream<char> ifstream;
  typedef basic_ofstream<char> ofstream;
  typedef basic_fstream<char> fstream;

  typedef basic_filebuf<wchar_t> wfilebuf;
  typedef basic_ifstream<wchar_t> wifstream;
  typedef basic_ofstream<wchar_t> wofstream;
  typedef basic_fstream<wchar_t> wfstream;

} // filesystem
} // experimental
} // std

#endif // _GLIBCXX_EXPTL_FSTREAM_H
