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
#include <stack>
#include <vector>

namespace std
{
namespace experimental
{
namespace filesystem
{
inline namespace v1
{

// Recursive_directory_iterator implementation.

struct recursive_directory_iterator::_Impl
{
  _Impl()
  : _M_pending_push{false}, _M_options{directory_options::none},
    _M_path{}, _M_dir{}, _M_dirent{}
  { }

  _Impl(_Impl&&) noexcept = default;

  _Impl(const path& pth)
  : _M_pending_push{false},
    _M_options{directory_options::none},
    _M_path{pth},
    _M_dir{}, _M_dirent{}
  {
    std::error_code ec;
    errno = 0;
    this->_M_dir.push(directory_iterator{this->_M_path});
    if (this->_M_dir.top() != directory_iterator{})
      this->_M_dirent = directory_entry{this->_M_path
				      / this->_M_dir.top()->path().filename()};
    else
      ec = std::make_error_code(static_cast<std::errc>(errno));
    if (ec)
      throw filesystem_error{"recursive_directory_iterator", this->_M_path, ec};
  }

  _Impl(const path& pth, directory_options opts)
  : _M_pending_push{false},
    _M_options{opts},
    _M_path{pth},
    _M_dir{}, _M_dirent{}
  {
    std::error_code ec;
    errno = 0;
    this->_M_dir.push(directory_iterator{this->_M_path});
    if (this->_M_dir.top() != directory_iterator{})
      this->_M_dirent = directory_entry{this->_M_path
				      / this->_M_dir.top()->path().filename()};
    else
      ec = std::make_error_code(static_cast<std::errc>(errno));
    if (ec)
      throw filesystem_error{"recursive_directory_iterator", this->_M_path, ec};
  }

  _Impl(const path& pth, directory_options opts, std::error_code& ec) noexcept
  : _M_pending_push{false},
    _M_options{opts},
    _M_path{pth},
    _M_dir{}, _M_dirent{}
  {
    ec.clear();
    errno = 0;
    this->_M_dir.push(directory_iterator{this->_M_path, ec});
    if (this->_M_dir.top() != directory_iterator{})
      this->_M_dirent = directory_entry{this->_M_path
				      / this->_M_dir.top()->path().filename()};
    else
      ec = std::make_error_code(static_cast<std::errc>(errno));
  }

  _Impl(const path& pth, std::error_code& ec) noexcept
  : _M_pending_push{false},
    _M_options{directory_options::none},
    _M_path{pth},
    _M_dir{}, _M_dirent{}
  {
    ec.clear();
    errno = 0;
    this->_M_dir.push(directory_iterator{this->_M_path, ec});
    if (this->_M_dir.top() != directory_iterator{})
      this->_M_dirent = directory_entry{this->_M_path
				      / this->_M_dir.top()->path().filename()};
    else
      ec = std::make_error_code(static_cast<std::errc>(errno));
  }

  _Impl(const _Impl& impl)
  : _M_pending_push{impl._M_pending_push},
    _M_options{impl._M_options},
    _M_path{impl._M_path},
    _M_dir{}, _M_dirent{}
  {
    std::error_code ec;
    errno = 0;
    this->_M_dir.push(directory_iterator{this->_M_path});
    if (this->_M_dir.top() != directory_iterator{})
      this->_M_dirent = directory_entry{this->_M_path
				      / this->_M_dir.top()->path().filename()};
    else
      ec = std::make_error_code(static_cast<std::errc>(errno));
    if (ec)
      throw filesystem_error{"recursive_directory_iterator", this->_M_path, ec};
  }

  ~_Impl() = default;

  // observers
  int
  _M_depth() const
  { return this->_M_dir.size() - 1; }

  bool
  _M_recursion_pending() const
  { return this->_M_pending_push; }

  // modifiers
  _Impl&
  operator=(const _Impl& impl)
  {
    //FIXME;
  }

  _Impl&
  operator=(_Impl&&) = default;

  _Impl&
  _M_incr(std::error_code& ec) noexcept
  {
    ec.clear();
    static const directory_iterator end{};
    if (this->_M_recursion_pending()
	&& is_directory(status(this->_M_dirent.path(), ec))
	&& (!is_symlink(symlink_status(this->_M_dirent.path(), ec))
	    || this->_M_options == directory_options::follow_directory_symlink))
      {
	this->_M_path = this->_M_dirent.path();
	this->_M_dir.push(directory_iterator{this->_M_path});
	this->pending_push(false);
      }
    else
      this->_M_dir.top().increment(ec);

    if (ec)
      return *this;

    if (this->_M_dir.top() != end)
      {
	this->_M_dirent
	  = directory_entry{this->_M_path
			  / this->_M_dir.top()->path().filename()};
	if (is_directory(status(this->_M_dirent.path(), ec))
	    && (!is_symlink(symlink_status(this->_M_dirent.path(), ec))
	    || this->_M_options == directory_options::follow_directory_symlink))
	  this->pending_push(true);
      }
    else
      this->_M_pop(ec);

    return *this;
  }

  void
  _M_pop()
  {
    std::error_code ec;
    this->_M_pop(ec);
    if (ec)
      throw filesystem_error{"pop", ec};
  }

  // NOTE: I added an error code version of pop.
  void
  _M_pop(std::error_code& ec) noexcept
  {
    ec.clear();
    if (this->_M_dir.size() > 1)
      {
	this->_M_path = this->_M_path.parent_path();
	this->_M_dir.pop();
	this->_M_dirent = directory_entry{this->_M_path}; // Superfluous?
	std::error_code ec;
	this->_M_incr(ec);
      }
    else
      *this = _Impl{};
  }

  void
  pending_push(bool value)
  { this->_M_pending_push = value; }

  bool _M_pending_push;
  directory_options _M_options;

  path _M_path;
  std::stack<directory_iterator, std::vector<directory_iterator>> _M_dir;
  directory_entry _M_dirent;
};

// constructors and destructor
recursive_directory_iterator::recursive_directory_iterator() noexcept
: _M_impl{std::make_unique<_Impl>()}
{ }

recursive_directory_iterator::
recursive_directory_iterator(const path& pth)
: _M_impl{std::make_unique<_Impl>(pth)}
{ }

recursive_directory_iterator::
recursive_directory_iterator(const path& pth, directory_options opts)
: _M_impl{std::make_unique<_Impl>(pth, opts)}
{ }

recursive_directory_iterator::
recursive_directory_iterator(const path& pth, directory_options opts,
			     std::error_code& ec) noexcept
: _M_impl{std::make_unique<_Impl>(pth, opts, ec)}
{ }

recursive_directory_iterator::
recursive_directory_iterator(const path& pth, std::error_code& ec) noexcept
: _M_impl{std::make_unique<_Impl>(pth, ec)}
{ }

//recursive_directory_iterator::
//recursive_directory_iterator(const recursive_directory_iterator& rdi)
//: _M_impl{std::make_unique<_Impl>(rdi._M_impl)}
//{ };

recursive_directory_iterator::
recursive_directory_iterator(recursive_directory_iterator&&) = default;

recursive_directory_iterator::
~recursive_directory_iterator() = default;

// observers
int
recursive_directory_iterator::depth() const
{ return this->_M_impl.get()->_M_depth(); }

bool
recursive_directory_iterator::recursion_pending() const
{ return this->_M_impl.get()->_M_recursion_pending(); }

// modifiers
recursive_directory_iterator&
recursive_directory_iterator::
operator=(const recursive_directory_iterator& rdi)
{
  if (&rdi != this)
  {
    
  }
  return *this;
}

recursive_directory_iterator&
recursive_directory_iterator::
operator=(recursive_directory_iterator&&) = default;

recursive_directory_iterator&
recursive_directory_iterator::operator++()
{
  std::error_code ec;
  this->increment(ec);
  if (ec)
    throw filesystem_error{"operator++", ec};
  return *this;
}

recursive_directory_iterator&
recursive_directory_iterator::increment(std::error_code& ec) noexcept
{
  this->_M_impl.get()->_M_incr(ec);

  return *this;
}

void
recursive_directory_iterator::pop()
{ this->_M_impl.get()->_M_pop(); }

// NOTE: I added an error code version of pop.
void
recursive_directory_iterator::pop(std::error_code& ec) noexcept
{ this->_M_impl.get()->_M_pop(ec); }

void
recursive_directory_iterator::disable_recursion_pending()
{ this->_M_impl.get()->pending_push(false); }

recursive_directory_iterator::reference
recursive_directory_iterator::operator*()
{ return this->_M_impl.get()->_M_dirent; }

recursive_directory_iterator::pointer
recursive_directory_iterator::operator->()
{ return &this->_M_impl.get()->_M_dirent; }

bool
recursive_directory_iterator::
operator==(const recursive_directory_iterator& rdi) const noexcept
{
  return this->_M_impl.get()->_M_dirent.path().native()
	== rdi._M_impl.get()->_M_dirent.path().native();
}

bool
recursive_directory_iterator::
operator!=(const recursive_directory_iterator& rdi) const noexcept
{ return !this->operator==(rdi); }

} // inline namespace v1
} // namespace filesystem
} // namespace experimental
} // namespace std
