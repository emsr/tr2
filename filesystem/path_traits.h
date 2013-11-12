// C++ experimantal <filesystem> -*- C++ -*-

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

/** @file tr2/path_traits.h
 *  This is an experimental C++ Library header.
 */

#ifndef _GLIBCXX_EXPERIMENTAL_PATH_TRAITS_H
#define _GLIBCXX_EXPERIMENTAL_PATH_TRAITS_H 1

#pragma GCC system_header

#if __cplusplus <= 201103L
# include <bits/c++14_warning.h>
#else

#include <list>
#include <vector>
#include <array>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace experimental
{
namespace filesystem
{
namespace __detail
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

/**
 *
 */
namespace _Path_traits
{

#ifdef _GLIBCXX_USE_WCHAR_T
  using codecvt_type = std::codecvt<wchar_t, char, std::mbstate_t>;
#endif

  //  is_pathable type trait;
  //  Allows disabling over-agressive class path member templates

  template<typename _Tp>
    struct is_pathable
    {
      static const bool value = false;
    };

  template<>
    struct is_pathable<char*>
    {
      static const bool value = true;
    };

  template<>
    struct is_pathable<const char*>
    {
      static const bool value = true;
    };

#ifdef _GLIBCXX_USE_WCHAR_T
  template<>
    struct is_pathable<wchar_t*>
    {
      static const bool value = true;
    };

  template<>
    struct is_pathable<const wchar_t*>
    {
      static const bool value = true;
    };
#endif

  template<>
    struct is_pathable<std::string>
    {
      static const bool value = true;
    };

#ifdef _GLIBCXX_USE_WCHAR_T
  template<>
    struct is_pathable<std::wstring>
    {
      static const bool value = true;
    };
#endif

  template<std::size_t _Num>
    struct is_pathable<std::array<char, _Num>>
    {
      static const bool value = true;
    };

#ifdef _GLIBCXX_USE_WCHAR_T
  template<std::size_t _Num>
    struct is_pathable<std::array<wchar_t, _Num>>
    {
      static const bool value = true;
    };
#endif

  template<>
    struct is_pathable<std::vector<char>>
    {
      static const bool value = true;
    };

#ifdef _GLIBCXX_USE_WCHAR_T
  template<>
    struct is_pathable<std::vector<wchar_t>>
    {
      static const bool value = true;
    };
#endif

  template<>
    struct is_pathable<std::list<char>>
    {
      static const bool value = true;
    };

#ifdef _GLIBCXX_USE_WCHAR_T
  template<>
    struct is_pathable<std::list<wchar_t>>
    {
      static const bool value = true;
    };
#endif

  //  Pathable empty

  template<typename _Container>
    inline std::enable_if_t<!std::is_array<_Container>::value, bool>
    empty(const _Container & __c)
    { return __c.begin() == __c.end(); }

  template<typename _Tp>
    inline bool
    empty(_Tp* const& __str)
    {
      assert(__str);
      return !*__str;
    }

  template<typename _Tp, size_t _Num>
    inline bool
    empty(_Tp (&__arr)[_Num])
    { return _Num == 0 || !__arr[0]; }

  // value types differ  -----------------------------------------------------//

  // Use from_end = 0 for null terminated MBCS.
  // A from_end argument of 0 is less efficient than a known end, so use only if needed
  void convert(const char* from, const char* from_end,
	       std::wstring& to, const codecvt_type& cvt);

  inline void
  convert(const char* __from, std::wstring& __to, const codecvt_type& __cvt)
  {
    assert(__from);
    convert(__from, 0, __to, __cvt);
  }

#ifdef _GLIBCXX_USE_WCHAR_T
  // Use from_end = 0 for null terminated MBCS.
  // A from_end argument of 0 is less efficient than a known end, so use only if needed
  void convert(const wchar_t* from, const wchar_t* from_end,
	       std::string& to, const codecvt_type& cvt);

  inline void
  convert(const wchar_t* __from, std::string& __to, const codecvt_type& __cvt)
  {
    assert(__from);
    convert(__from, 0, __to, __cvt);
  }
#endif

  // value types same  -------------------------------------------------------//

  // char

  inline void
  convert(const char* __from_begin, const char* __from_end,
	  std::string& __to, const codecvt_type&)
  {
    assert(__from_begin);
    assert(__from_end);
    __to.append(__from_begin, __from_end);
  }

  inline void
  convert(const char* __from, std::string& __to, const codecvt_type&)
  {
    assert(__from);
    __to += __from;
  }

#ifdef _GLIBCXX_USE_WCHAR_T
  // wchar_t

  inline void
  convert(const wchar_t* __from_begin, const wchar_t* __from_end,
	  std::wstring& __to, const codecvt_type&)
  {
    assert(__from_begin);
    assert(__from_end);
    __to.append(__from_begin, __from_end);
  }

  inline void
  convert(const wchar_t* __from, std::wstring& __to,
	  const codecvt_type&)
  {
    assert(__from);
    __to += __from;
  }
#endif

  //  Source dispatch  -------------------------------------------------------//

  //  contiguous containers
  template<typename _Up>
    inline void
    dispatch(const std::string& __str, _Up& __to, const codecvt_type& __cvt)
    {
      if (__str.size() > 0)
	convert(&*__str.begin(), &*__str.begin() + __str.size(), __to, __cvt);
    }

  template<typename _Up>
    inline void
    dispatch(const std::vector<char>& c, _Up& to, const codecvt_type& cvt)
    {
      if (c.size() > 0)
	convert(&*c.begin(), &*c.begin() + c.size(), to, cvt);
    }

#ifdef _GLIBCXX_USE_WCHAR_T
  template<typename _Up>
    inline void
    dispatch(const std::wstring& c, _Up& to, const codecvt_type& cvt)
    {
      if (c.size() > 0)
	convert(&*c.begin(), &*c.begin() + c.size(), to, cvt);
    }

  template<typename _Up>
    inline void
    dispatch(const std::vector<wchar_t>& __str, _Up& __to,
	     const codecvt_type& __cvt)
    {
      if (__str.size() > 0)
	convert(&*__str.begin(), &*__str.begin() + __str.size(), __to, __cvt);
    }
#endif

  //  non-contiguous containers
  template<typename _Container, typename _Up>
    inline std::enable_if_t<!std::is_array<_Container>::value, void>
      dispatch(const _Container& __str, _Up& __to, const codecvt_type& __cvt)
  {
    if (__str.size() > 0)
    {
      std::basic_string<typename _Container::value_type>
	__stemp(__str.begin(), __str.end());
      convert(__stemp.c_str(), __stemp.c_str() + __stemp.size(), __to, __cvt);
    }
  }

  //  c_str
  template<typename _Tp, typename _Up>
  inline void dispatch(_Tp* const& __str, _Up& __to, const codecvt_type& __cvt)
  {
    assert(__str);
    convert(__str, __to, __cvt);
  }

} // namespace _Path_traits


_GLIBCXX_END_NAMESPACE_VERSION
} // namespace __detail
} // namespace filesystem
} // namespace experimental
} // namespace std

#endif // __cplusplus <= 201103L

#endif // _GLIBCXX_EXPERIMENTAL_PATH_TRAITS_H
