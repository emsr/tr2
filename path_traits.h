// TR2 <filesystem> -*- C++ -*-

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
 *  This is a TR2 C++ Library header.
 */

#ifndef _GLIBCXX_TR2_PATH_TRAITS_H
#define _GLIBCXX_TR2_PATH_TRAITS_H 1

#pragma GCC system_header

#include <list>
#include <vector>

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace tbd
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

  typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_type;

  //  is_pathable type trait; allows disabling over-agressive class path member templates

  template <class _Tp>
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

  template<>
    struct is_pathable<std::string>
    {
      static const bool value = true;
    };

  template<>
    struct is_pathable<std::wstring>
    {
      static const bool value = true;
    };

  template<>
    struct is_pathable<std::vector<char>>
    {
      static const bool value = true;
    };

  template<>
    struct is_pathable<std::vector<wchar_t>>
    {
      static const bool value = true;
    };

  template<>
    struct is_pathable<std::list<char>>
    {
      static const bool value = true;
    };

  template<>
    struct is_pathable<std::list<wchar_t>>
    {
      static const bool value = true;
    };

  //  Pathable empty

  template<typename _Container>
    inline typename std::enable_if<!std::is_array<_Container>::value, bool>::type
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
    empty(_Tp (&x)[_Num])
    { return _Num == 0 || !x[0]; }

  // value types differ  ---------------------------------------------------------------//
  //
  //   A from_end argument of 0 is less efficient than a known end, so use only if needed

  void convert(const char* from, const char* from_end, // 0 for null terminated MBCS
         std::wstring& to,
	       const codecvt_type& cvt);

  void convert(const wchar_t* from, const wchar_t* from_end, // 0 for null terminated MBCS
	       std::string& to, const codecvt_type& cvt);

  inline void
  convert(const char* __from, std::wstring& __to, const codecvt_type& __cvt)
  {
    assert(__from);
    convert(__from, 0, __to, __cvt);
  }

  inline void
  convert(const wchar_t* __from, std::string& __to, const codecvt_type& __cvt)
  {
    assert(__from);
    convert(__from, 0, __to, __cvt);
  }

  // value types same  -----------------------------------------------------------------//

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

  //  Source dispatch  -----------------------------------------------------------------//

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
    dispatch(const std::wstring& c, _Up& to, const codecvt_type& cvt)
    {
      if (c.size() > 0)
	convert(&*c.begin(), &*c.begin() + c.size(), to, cvt);
    }

  template<typename _Up>
    inline void
    dispatch(const std::vector<char>& c, _Up& to, const codecvt_type& cvt)
    {
      if (c.size() > 0)
	convert(&*c.begin(), &*c.begin() + c.size(), to, cvt);
    }

  template<typename _Up>
    inline void
    dispatch(const std::vector<wchar_t>& __str, _Up& __to, const codecvt_type& __cvt)
    {
      if (__str.size() > 0)
	convert(&*__str.begin(), &*__str.begin() + __str.size(), __to, __cvt);
    }

  //  non-contiguous containers
  template<typename _Container, typename _Up>
    inline typename std::enable_if<!std::is_array<_Container>::value, void>::type
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
} // namespace tbd
} // namespace std

#endif // _GLIBCXX_TR2_PATH_TRAITS_H
