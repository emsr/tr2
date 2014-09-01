// C++ experimantal filesystem.cc -*- C++ -*-

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

#include "filesystem"

#include <dirent.h> // For DIR, opendir, ...

namespace std
{
namespace experimental
{
namespace filesystem
{
inline namespace v1
{

// Directory_iterator implementation.

struct directory_iterator::_Impl
{
  _Impl() noexcept
  : _M_path{}, _M_dir{nullptr}, _M_dirent{}
  { }

  _Impl(_Impl&&) noexcept = default;

  _Impl(const path& pth)
  : _M_path{pth},
    _M_dir{nullptr},
    _M_dirent{}
  {
    errno = 0;
    std::error_code ec;
    _M_dir = ::opendir(_M_path.c_str());
    this->_M_incr(ec);
    if (ec)
      throw filesystem_error{"directory_iterator", this->_M_path, ec};
  }

  _Impl(const path& pth, std::error_code& ec) noexcept
  : _M_path{pth},
    _M_dir{nullptr},
    _M_dirent{}
  {
    ec.clear();
    errno = 0;
    _M_dir = ::opendir(_M_path.c_str());
    if (errno != 0)
      std::make_error_code(static_cast<std::errc>(errno));
    else
      this->_M_incr(ec);
  }

  _Impl(const path& pth, directory_options opts, std::error_code& ec) noexcept
  : _M_path{pth},
    _M_dir{nullptr},
    _M_dirent{}
  {
    ec.clear();
    errno = 0;
    _M_dir = ::opendir(_M_path.c_str());
    if (errno == EACCES
	&& (opts & directory_options::skip_permissions_denied)
	   != directory_options::none)
      this->_M_incr(ec);
  }

  ~_Impl() noexcept
  {
    if (this->_M_dir)
      ::closedir(this->_M_dir);
  }

  // modifiers
  _Impl&
  operator=(_Impl&&) noexcept = default;

  void
  _M_incr(std::error_code& ec) noexcept
  {
    ec.clear();
    if (this->_M_dir)
      {
	struct dirent dirent, *res;
	int result, len;
	do
	  {
	    result = ::readdir_r(this->_M_dir, &dirent, &res);
	    len = __builtin_strlen(dirent.d_name);
	  }
	while (result == 0 && res != nullptr
	       && ((len > 0 && dirent.d_name[0] == '.'
		 && (dirent.d_name[1] == '\0'
		   || (len > 1 && dirent.d_name[1] == '.'
		     && dirent.d_name[2] == '\0')))));
	if (result != 0)
	  ec = std::make_error_code(static_cast<std::errc>(result));
	if (res != nullptr)
	  this->_M_dirent = directory_entry(this->_M_path
					    / std::string(dirent.d_name));
	else
	  this->_M_dirent = directory_entry{};
      }
    return;
  }

  path _M_path;
  DIR *_M_dir;
  directory_entry _M_dirent;
};

// Create the "end" iterator.
directory_iterator::directory_iterator() noexcept
: _M_impl{std::make_unique<_Impl>()}
{ }

directory_iterator::directory_iterator(directory_iterator&& diter) noexcept
 = default;

directory_iterator::directory_iterator(const path& pth)
: _M_impl{std::make_unique<_Impl>(pth)}
{ }

directory_iterator::directory_iterator(const path& pth,
				       directory_options opts,
				       std::error_code& ec)
: _M_impl{std::make_unique<_Impl>(pth, opts, ec)}
{ }

directory_iterator::directory_iterator(const path& pth,
				       std::error_code& ec) noexcept
: _M_impl{std::make_unique<_Impl>(pth, ec)}
{ }

directory_iterator::~directory_iterator() = default;

directory_iterator&
directory_iterator::operator=(const directory_iterator&)
{
  //FIXME!!!
  return *this;
}

directory_iterator&
directory_iterator::operator=(directory_iterator&&) = default;

directory_iterator&
directory_iterator::operator++()
{
  std::error_code ec;
  this->increment(ec);
  if (ec)
    throw filesystem_error{"operator++", ec};
  return *this;
}

directory_iterator&
directory_iterator::increment(std::error_code& ec) noexcept
{
  this->_M_impl.get()->_M_incr(ec);
  return *this;
}

directory_iterator::reference
directory_iterator::operator*()
{ return this->_M_impl.get()->_M_dirent; }

directory_iterator::pointer
directory_iterator::operator->()
{ return &this->_M_impl.get()->_M_dirent; }

bool
directory_iterator::operator==(const directory_iterator& di) const noexcept
{
  return this->_M_impl.get()->_M_dirent.path().native()
	 == di._M_impl.get()->_M_dirent.path().native();
}

bool
directory_iterator::operator!=(const directory_iterator& di) const noexcept
{ return !this->operator==(di); }

} // inline namespace v1
} // namespace filesystem
} // namespace experimental
} // namespace std
