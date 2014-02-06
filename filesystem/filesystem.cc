// C++ experimantal filesystem.cc -*- C++ -*-

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

#include "filesystem"

#include <memory>
#include <dirent.h>
#include <unistd.h>
#include <cerrno>
#include <sys/statvfs.h> // For statvfs
#include <sys/stat.h> // For stat
#include <fcntl.h> // For creat
#include <utime.h> // For utime.
#include <fstream>
#include <stack>  //  for recursive_directory_iterator
#include <vector>  //  for recursive_directory_iterator
#include <iomanip> // for quoted
#include <experimental/string_view>

namespace std {
namespace experimental {
namespace filesystem {

// Path implementation.

const __detail::_Path_traits::codecvt_type* path::_S_codecvt
  = &std::use_facet<__detail::_Path_traits::codecvt_type>(std::locale(""));

path::path() = default;

path::path(const path&) = default;

path::path(path&&) noexcept = default;

path::~path() = default;

//path&
//path::operator=(const path& pth) = default;

//path&
//path::operator=(path&& pth) noexcept = default;

namespace __detail
{
  int
  lex_compare(path::iterator first1, path::iterator last1,
        path::iterator first2, path::iterator last2)
  {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2)
    {
      if (first1->native() < first2->native())
	return -1;
      if (first2->native() < first1->native())
	return +1;
    }
    if (first1 == last1 && first2 == last2)
      return 0;
    return first1 == last1 ? -1 : +1;
  }
}

inline int
path::compare(const path& pth) const noexcept
{
  return __detail::lex_compare(this->begin(), this->end(),
			       pth.begin(), pth.end());
}

//path&
//path::make_absolute(const path& base)
//{
//  ;
//}

path
path::root_name() const
{
  if (this->empty())
    return path{};
  std::string first = this->begin()->string();
  if ((first.length() > 2)
	&& (first[0] == preferred_separator)
	&& (first[1] == preferred_separator))
    return *this->begin();
  else
    return path{};
}

path
path::root_directory() const
{
  const path::value_type sep_str[2]{preferred_separator, 0};
  if (this->empty())
    return path{};
  iterator piter = this->begin();
  if (!this->root_name().empty())
    ++piter;
  if (piter != this->end() && piter->string() == sep_str)
    return *piter;
  else
    return path{};
}

path
path::root_path() const
{ return root_name() / root_directory(); }

path
path::relative_path() const
{
  if (this->empty())
    return path{};
  iterator piter = this->begin();
  if (!this->root_name().empty())
    ++piter;
  if (!this->root_directory().empty())
    ++piter;
  path rel;
  for (; piter != this->end(); ++piter)
    rel /= *piter;
  return rel;
}

path
path::parent_path() const
{
  if (this->empty() || this->begin() == --this->end())
    return path{};
  else
    {
      path pp;
      for (auto piter = this->begin(); piter != --this->end(); ++piter)
	pp /= *piter;
      return pp;
    }
}

path
path::filename() const
{ return this->empty() ? path{} : *--this->end(); }

path
path::stem() const
{
  std::string filename = this->filename().string();
  std::size_t pos = filename.rfind('.');
  if (pos != string_type::npos && filename != "." && filename != "..")
    return path{filename.substr(0, pos - 1)};
  else
    return filename;
}

path
path::extension() const
{
  std::string filename = this->filename().string();
  std::size_t pos = filename.rfind('.');
  if (pos != std::string::npos && filename != "." && filename != "..")
    return path{filename.substr(pos)};
  else
    return path{};
}

path&
path::replace_extension(const path& new_extension)
{
  this->_M_pathname.erase(this->_M_pathname.size()
		        - this->extension()._M_pathname.size());

  if (!new_extension.empty())
  {
    if (new_extension._M_pathname[0] != '.')
      this->_M_pathname.push_back('.');
    this->_M_pathname.append(new_extension._M_pathname);
  }

  return *this;
}


path::iterator
path::begin() const
{ return iterator(*this, true); }

path::iterator
path::end() const
{ return iterator(*this, false); }

// Path iterator implementation

struct path::iterator::_Impl
{
  _Impl()
  : _M_pos{std::string::npos}, _M_chunk{}
  { }

  _Impl(const path& pth, bool begin = false)
  {
    const path::value_type sep_str[2]{preferred_separator, 0};
    std::experimental::string_view pathname{pth.string()};
    bool net{false};
    std::size_t pos{0};
    for (std::size_t c = 0; c < pathname.length(); ++c)
      {
	std::size_t num_slashes{0};
	while (c < pathname.length() && pathname[c] == preferred_separator)
	  {
	    ++num_slashes;
	    ++c;
	  }

	if (pos == 0 && !net && num_slashes == 2)
	  net = true;
	else if (pos == 0 && num_slashes > 0)
	  {
	    if (c - num_slashes > 0)
	      _M_chunk.push_back(pathname.substr(0, c - num_slashes));
	    else
	      _M_chunk.push_back(sep_str);
	    if (net)
	      _M_chunk.push_back(sep_str);
	    pos = c;
	  }
	else if (num_slashes > 0)
	  {
	    _M_chunk.push_back(pathname.substr(pos, c - num_slashes - pos));
	    pos = c;
	  }
      }
    if (pos < pathname.length())
      _M_chunk.push_back(pathname.substr(pos));
    else if (pathname.length() > 1)
      _M_chunk.push_back("."sv);

    if (begin && _M_chunk.size() > 0)
      _M_pos = 0;
    else
      _M_pos = std::string::npos;
  }

  std::size_t _M_pos;
  std::vector<path> _M_chunk;
};

path::iterator::iterator() = default;

path::iterator::iterator(const iterator& iter) = default;

path::iterator::iterator(const path& pth, bool begin)
: _M_impl{std::make_unique<_Impl>(pth, begin)}
{ }

path::iterator::~iterator() = default;

path::iterator& path::iterator::operator=(const iterator& iter) = default;

bool
path::iterator::operator==(const iterator& iter) const
{
  if (iter._M_impl.get()->_M_pos == std::string::npos
      && this->_M_impl.get()->_M_pos == std::string::npos)
    return true;
  else if (iter._M_impl.get()->_M_pos != this->_M_impl.get()->_M_pos)
    return false;
  else
    {
      auto ichunk = iter._M_impl.get()->_M_chunk;
      auto icbegin = ichunk.cbegin(), icend = ichunk.cend();
      auto tchunk = this->_M_impl.get()->_M_chunk;
      auto tcbegin = tchunk.cbegin(), tcend = tchunk.cend();
      for (; icbegin != icend, tcbegin != tcend; ++icbegin, ++tcbegin)
	if (icbegin->native() != tcbegin->native())
	  return false;
      return icbegin == icend && tcbegin == tcend;
    }
}
//------------------
//{ return (iter._M_impl.get()->_M_pos == std::string::npos
//       && this->_M_impl.get()->_M_pos == std::string::npos)
//   || (iter._M_impl.get()->_M_chunk == this->_M_impl.get()->_M_chunk
//    && iter._M_impl.get()->_M_pos == this->_M_impl.get()->_M_pos); }

bool
path::iterator::operator!=(const iterator& iter) const
{ return !this->operator==(iter); }

path::iterator::reference
path::iterator::operator*()
{ return this->_M_impl.get()->_M_chunk[this->_M_impl.get()->_M_pos]; }

path::iterator::pointer
path::iterator::operator->()
{ return &this->_M_impl.get()->_M_chunk[this->_M_impl.get()->_M_pos]; }

path::iterator&
path::iterator::operator++()
{
  if (this->_M_impl.get()->_M_pos < this->_M_impl.get()->_M_chunk.size())
    ++this->_M_impl.get()->_M_pos;
  if (this->_M_impl.get()->_M_pos == this->_M_impl.get()->_M_chunk.size())
    this->_M_impl.get()->_M_pos = std::string::npos;
  return *this;
}

path::iterator
path::iterator::operator++(int)
{
  iterator temp = *this;
  this->operator++();
  return temp;
}

path::iterator&
path::iterator::operator--()
{
  if (this->_M_impl.get()->_M_pos == std::string::npos)
    this->_M_impl.get()->_M_pos = this->_M_impl.get()->_M_chunk.size() - 1;
  else if (this->_M_impl.get()->_M_pos > 0)
    --this->_M_impl.get()->_M_pos;
  return *this;
}

path::iterator
path::iterator::operator--(int)
{
  iterator temp = *this;
  this->operator--();
  return temp;
}

// Directory_iterator implementation.

struct directory_iterator::_Impl
{
  _Impl()
  : _M_path{}, _M_dir{nullptr}, _M_dirent{}
  { }

  _Impl(_Impl&&) noexcept = default;

  _Impl(const path& pth)
  : _M_path{pth},
    _M_dir{::opendir(_M_path.c_str())},
    _M_dirent{}
  {
    std::error_code ec;
    this->_M_incr(ec);
    if (ec)
      throw filesystem_error{"directory_iterator", this->_M_path, ec};
  }

  _Impl(const path& pth, std::error_code& ec) noexcept
  : _M_path{pth},
    _M_dir{::opendir(_M_path.c_str())},
    _M_dirent{}
  { this->_M_incr(ec); }

  ~_Impl() noexcept
  {
    if (this->_M_dir)
      ::closedir(this->_M_dir);
  }

  // modifiers
  _Impl&
  operator=(_Impl&&) = default;

  void
  _M_incr(std::error_code& ec) noexcept
  {
    if (this->_M_dir)
      {
	struct dirent dirent, *res;
	int result;
	do
	  result = ::readdir_r(this->_M_dir, &dirent, &res);
	while (result == 0 && res != nullptr
	       && ((dirent.d_name[0] == '.' && (dirent.d_name[1] == '\0'
		|| (dirent.d_name[1] == '.' && dirent.d_name[2] == '\0')))));
	if (result != 0)
	  ec = std::make_error_code(static_cast<std::errc>(result));
	if (res != nullptr)
	  this->_M_dirent = directory_entry(this->_M_path
					    / std::string(dirent.d_name));
	else
	  this->_M_dirent = directory_entry();
      }
    return;
  }

  path _M_path;
  DIR *_M_dir;
  directory_entry _M_dirent;
};

// Create the "end" iterator.
directory_iterator::directory_iterator()
: _M_impl{std::make_unique<_Impl>()}
{ }

directory_iterator::directory_iterator(directory_iterator&& diter) = default;

directory_iterator::directory_iterator(const path& pth)
: _M_impl{std::make_unique<_Impl>(pth)}
{ }

directory_iterator::directory_iterator(const path& pth,
				       std::error_code& ec) noexcept
: _M_impl{std::make_unique<_Impl>(pth, ec)}
{ }

directory_iterator::~directory_iterator() = default;

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

bool
directory_iterator::operator==(const directory_iterator& di) const
{
  return this->_M_impl.get()->_M_dirent.path().native()
	 == di._M_impl.get()->_M_dirent.path().native();
}

bool
directory_iterator::operator!=(const directory_iterator& di) const
{ return !this->operator==(di); }

directory_iterator::reference
directory_iterator::operator*()
{ return this->_M_impl.get()->_M_dirent; }

directory_iterator::pointer
directory_iterator::operator->()
{ return &this->_M_impl.get()->_M_dirent; }

// Recursive_directory_iterator implementation.

struct recursive_directory_iterator::_Impl
{
  _Impl()
  : _M_pending_push{false}, _M_options{symlink_option::none},
    _M_path{}, _M_dir{}, _M_dirent{}
  { }

  _Impl(_Impl&&) noexcept = default;

  _Impl(const path& pth, symlink_option opt)
  : _M_pending_push{false},
    _M_options{opt},
    _M_path{pth},
    _M_dir{}, _M_dirent{}
  {
    errno = 0;
    std::error_code ec;
    this->_M_dir.push(directory_iterator{this->_M_path});
    if (this->_M_dir.top() != directory_iterator{})
      this->_M_dirent = directory_entry{this->_M_path
				      / this->_M_dir.top()->path().filename()};
    else
      ec = std::make_error_code(static_cast<std::errc>(errno));
    if (ec)
      throw filesystem_error{"recursive_directory_iterator", this->_M_path, ec};
  }

  _Impl(const path& pth, symlink_option opt, std::error_code& ec) noexcept
  : _M_pending_push{false},
    _M_options{opt},
    _M_path{pth},
    _M_dir{}, _M_dirent{}
  {
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
    _M_options{symlink_option::none},
    _M_path{pth},
    _M_dir{}, _M_dirent{}
  {
    errno = 0;
    this->_M_dir.push(directory_iterator{this->_M_path, ec});
    if (this->_M_dir.top() != directory_iterator{})
      this->_M_dirent = directory_entry{this->_M_path
				      / this->_M_dir.top()->path().filename()};
    else
      ec = std::make_error_code(static_cast<std::errc>(errno));
  }

  ~_Impl() = default;

  // observers
  int
  _M_level() const
  { return this->_M_dir.size() - 1; }

  bool
  _M_is_push_pending() const
  { return this->_M_pending_push; }

  // modifiers
  _Impl&
  operator=(_Impl&&) = default;

  _Impl&
  _M_incr(std::error_code& ec) noexcept
  {
    static const directory_iterator end{};
    if (this->_M_is_push_pending()
	&& is_directory(status(this->_M_dirent.path(), ec))
	&& (!is_symlink(symlink_status(this->_M_dirent.path(), ec))
	    || this->_M_options == symlink_option::recurse))
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
		|| this->_M_options == symlink_option::recurse))
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
  symlink_option _M_options;

  path _M_path;
  std::stack<directory_iterator, std::vector<directory_iterator>> _M_dir;
  directory_entry _M_dirent;
};

// constructors and destructor
recursive_directory_iterator::recursive_directory_iterator()
: _M_impl{std::make_unique<_Impl>()}
{ }

recursive_directory_iterator::
recursive_directory_iterator(recursive_directory_iterator&&) = default;

recursive_directory_iterator::
recursive_directory_iterator(const path& pth, symlink_option opt)
: _M_impl{std::make_unique<_Impl>(pth, opt)}
{ }

recursive_directory_iterator::
recursive_directory_iterator(const path& pth, symlink_option opt,
			     std::error_code& ec) noexcept
: _M_impl{std::make_unique<_Impl>(pth, opt, ec)}
{ }

recursive_directory_iterator::
recursive_directory_iterator(const path& pth, std::error_code& ec) noexcept
: _M_impl{std::make_unique<_Impl>(pth, ec)}
{ }

recursive_directory_iterator::
~recursive_directory_iterator() = default;

// observers
int
recursive_directory_iterator::level() const
{ return this->_M_impl.get()->_M_level(); }

bool
recursive_directory_iterator::is_push_pending() const
{ return this->_M_impl.get()->_M_is_push_pending(); }

// modifiers
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
recursive_directory_iterator::pending_push(bool value)
{ this->_M_impl.get()->pending_push(value); }

bool
recursive_directory_iterator::
operator==(const recursive_directory_iterator& rdi) const
{
  return this->_M_impl.get()->_M_dirent.path().native()
	== rdi._M_impl.get()->_M_dirent.path().native();
}

bool
recursive_directory_iterator::
operator!=(const recursive_directory_iterator& rdi) const
{ return !this->operator==(rdi); }

recursive_directory_iterator::reference
recursive_directory_iterator::operator*()
{ return this->_M_impl.get()->_M_dirent; }

recursive_directory_iterator::pointer
recursive_directory_iterator::operator->()
{ return &this->_M_impl.get()->_M_dirent; }

// Namespace functions.

bool
lexicographical_compare(path::iterator first1, path::iterator last1,
			path::iterator first2, path::iterator last2)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2)
    {
      if (first1->native() < first2->native())
	return true;
      if (first2->native() < first1->native())
	return false;
    }
  return first1 == last1 && first2 != last2;
}

std::ostream&
operator<<(std::ostream& os, const path& pth)
{ return os << std::quoted(pth.string(), '"', '&'); }

std::wostream&
operator<<(std::wostream& os, const path& pth)
{ return os << std::quoted(pth.wstring(), L'"', L'&'); }

std::istream&
operator>>(std::istream& is, path& pth)
{
  pth.clear();
  std::string pathname;

  is >> std::quoted(pathname, '"', '&');

  pth = path{pathname};

  return is;
}

std::wistream&
operator>>(std::wistream& is, path& pth)
{
  pth.clear();
  std::wstring pathname;

  is >> std::quoted(pathname, L'"', L'&');

  pth = path{pathname};

  return is;
}

void
copy_directory(const path& from, const path& to)
{
  std::error_code ec;
  copy_directory(from, to, ec);
  if (ec)
    throw filesystem_error{"copy_directory", from, to, ec};
}

// Operational functions

path
absolute(const path& pth, const path& base)
{
  if (pth.has_root_name())
    {
      if (pth.has_root_directory())
	return pth;
      else
	absolute(base).root_name() / pth;
    }
  else
    {
      if (pth.has_root_directory())
	return pth.root_name()
	     / absolute(base).root_directory()
	     / absolute(base).relative_path()
	     / pth.relative_path();
      else
	absolute(base) / pth;
    }
}

path
canonical(const path& pth, const path& base)
{
  std::error_code ec;
  path canon = canonical(pth, base, ec);
  if (ec)
    throw filesystem_error{"canonical", pth, ec};
  return canon;
}

path
canonical(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  char canon[PATH_MAX];
  char* test = ::realpath(pth.c_str(), canon);
  if (test == nullptr)
    ec = std::make_error_code(static_cast<std::errc>(errno));
  return path{std::string{canon}};
}

path
canonical(const path& pth, const path& base, std::error_code& ec) noexcept
{
  path abspath = absolute(pth, base);
  return canonical(abspath, ec);
}

void
copy(const path& from, const path& to, copy_options options)
{
  std::error_code ec;
  copy(from, to, options, ec);
  if (ec)
    throw filesystem_error{"copy", from, to, ec};
}

void
copy(const path& from, const path& to,
     copy_options options, std::error_code& ec) noexcept
{
  file_status from_stat;
  if ((options & copy_options::create_symlinks) == copy_options::create_symlinks
   || (options & copy_options::skip_symlinks) == copy_options::skip_symlinks)
    from_stat = symlink_status(from, ec);
  else
    from_stat = status(from, ec);
  if (ec)
    return;

  file_status to_stat;
  if ((options & copy_options::create_symlinks) == copy_options::create_symlinks
   || (options & copy_options::skip_symlinks) == copy_options::skip_symlinks)
    to_stat = symlink_status(to, ec);
  else
    to_stat = status(to, ec);
  if (ec)
    return;

  if (equivalent(from, to))
    ec = std::make_error_code(static_cast<std::errc>(111));
  else if (is_other(from) || is_other(to))
    ec = std::make_error_code(static_cast<std::errc>(666));
  else if (is_directory(from) && is_regular_file(to))
    ec = std::make_error_code(static_cast<std::errc>(ENOTDIR));
  if (ec)
    return;

  if (is_symlink(from_stat))
    {
      if ((options & copy_options::skip_symlinks)
	  == copy_options::skip_symlinks)
	return;
      else if ((options & copy_options::copy_symlinks)
	       == copy_options::copy_symlinks)
	copy_symlink(from, to, ec);
      else if ((options & copy_options::none) == copy_options::none)
	{
	  path slink = read_symlink(from, ec);
	  if (ec)
	    return;
	  copy(slink, to, options, ec);
	}
    }
  else if (is_directory(from_stat)
	   && (options & copy_options::recursive) == copy_options::recursive)
    copy_directory(from, to, ec);
  else if (is_regular_file(from_stat))
    {
      if ((options & copy_options::directories_only)
	  != copy_options::directories_only)
	{
	  if ((options & copy_options::directories_only)
	      == copy_options::directories_only)
	    return;
	  else if ((options & copy_options::create_symlinks)
		   == copy_options::create_symlinks)
	    create_symlink(from, to, ec);
	  else if ((options & copy_options::create_hard_links)
		   == copy_options::create_hard_links)
	    create_hard_link(from, to, ec);
          else if (is_directory(to_stat))
	    copy_file(from, to / from.filename(), options, ec);
          else
	    copy_file(from, to, options, ec);
	}
    }
  else
    ;
}

void
copy_directory(const path& from, const path& to, std::error_code& ec) noexcept
{
  recursive_directory_iterator rdi(from, ec);
  if (ec)
    return;
  for (; rdi != recursive_directory_iterator(); ++rdi)
    {
      if (is_directory(rdi->path(), ec))
	{
	  
	}
    }
}

void
copy_file(const path& from, const path& to, copy_options options)
{
  std::error_code ec;
  copy_file(from, to, options, ec);
  if (ec)
    throw filesystem_error{"copy_file", from, to, ec};
}

void
copy_file(const path& from, const path& to, copy_options options,
	  std::error_code& ec) noexcept
{
  if (exists(to))
    {
      if ((options & copy_options::none) == copy_options::none)
	{
	  ec = std::make_error_code(static_cast<std::errc>(EEXIST));
	  return;
	}
      else if ((options & copy_options::skip_existing)
	       == copy_options::skip_existing)
	return;
      else if ((options & copy_options::update_existing)
	       == copy_options::update_existing)
	{
	  auto from_time = last_write_time(from, ec);
	  if (ec)
	    return;
	  auto to_time = last_write_time(to, ec);
	  if (ec)
	    return;
	  if (to_time >= from_time)
	    return;
	}
    }

  std::ifstream fromf(from.string(), std::ios::binary);
  if (fromf.fail())
    {
      ; // Error...
      return;
    }
  std::ofstream tof(to.string(), std::ios::binary);
  if (tof.fail())
    {
      ; // Error...
      return;
    }
  tof << fromf.rdbuf();
}

void
copy_symlink(const path& existing_symlink, const path& new_symlink)
{
  std::error_code ec;
  copy_symlink(existing_symlink, new_symlink, ec);
  if (ec)
    throw filesystem_error{"copy_symlink", existing_symlink, new_symlink, ec};
}

void
copy_symlink(const path& existing_symlink, const path& new_symlink,
	     std::error_code& ec) noexcept
{ create_symlink(read_symlink(existing_symlink, ec), new_symlink, ec); }

//  This is "mkdir -p" - create the directory and
//  any required intermediate directories.
bool
create_directories(const path& pth)
{
  std::error_code ec;
  bool ok = create_directories(pth, ec);
  if (ec)
    throw filesystem_error{"create_directories", pth, ec};
  return ok;
}

bool
create_directories(const path& pth, std::error_code& ec) noexcept
{
  file_status fs = status(pth, ec);
  if (ec)
    return false;
  if (pth.empty() || exists(fs))
    {
      if (!pth.empty() && !is_directory(fs))
        // ENOTEMPTY or ENOTDIR
	ec = std::make_error_code(static_cast<std::errc>(ENOTEMPTY));
      return false;
    }

  create_directories(pth.parent_path(), ec);
  if (ec)
    return false;
  create_directory(pth, ec);
  if (ec)
    return false;
  return true;
}

bool
create_directory(const path& pth)
{
  std::error_code ec;
  bool ok = create_directory(pth, ec);
  if (ec)
    throw filesystem_error{"create_directory", pth, ec};
  return ok;
}

bool
create_directory(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
  int result = ::mkdir(pth.c_str(), mode);
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
  return result == 0;
}

void
create_directory_symlink(const path& to, const path& new_symlink)
{
  std::error_code ec;
  create_directory_symlink(to, new_symlink, ec);
  if (ec)
    throw filesystem_error{"create_directory_symlink", to, new_symlink, ec};
}

void
create_directory_symlink(const path& to, const path& new_symlink,
			 std::error_code& ec) noexcept
{
  errno = 0;
  int result = ::symlink(to.c_str(), new_symlink.c_str());
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
}

void
create_hard_link(const path& to, const path& new_hard_link)
{
  std::error_code ec;
  create_hard_link(to, new_hard_link, ec);
  if (ec)
    throw filesystem_error{"create_hard_link", to, new_hard_link, ec};
}

void
create_hard_link(const path& to, const path& new_hard_link,
		 std::error_code& ec) noexcept
{
  errno = 0;
  int result = ::link(to.c_str(), new_hard_link.c_str());
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
}

// NOTE: I'm adding functions to create files.
// POSIC creat; Windows: CreateFile.
bool
create_file(const path& pth)
{
  std::error_code ec;
  create_file(pth, ec);
  if (ec)
    throw filesystem_error{"create_file", pth, ec};
  return true;
}

bool
create_file(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  mode_t mode = static_cast<mode_t>(perms::all_all);
  int result = ::creat(pth.c_str(), mode);
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
  return (result != -1);
}

void
create_symlink(const path& to, const path& new_symlink)
{
  std::error_code ec;
  create_symlink(to, new_symlink, ec);
  if (ec)
    throw filesystem_error{"create_symlink", to, new_symlink, ec};
}

void
create_symlink(const path& to, const path& new_symlink,
	       std::error_code& ec) noexcept
{
  errno = 0;
  int result = ::symlink(to.c_str(), new_symlink.c_str());
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));;
}

path
current_path()
{
  std::error_code ec;
  path curr_path = current_path(ec);
  if (ec)
    throw filesystem_error{"current_path", ec};
  return curr_path;
}

path
current_path(std::error_code& ec) noexcept
{
  errno = 0;
  long size = ::pathconf(".", _PC_PATH_MAX);
  std::unique_ptr<char> buf{std::make_unique<char>(size + 1)};
  char * pwd = ::getcwd(buf.get(), static_cast<std::size_t>(size));
  if (!pwd)
    ec = std::make_error_code(static_cast<std::errc>(errno));
  return path{std::string(buf.get())};
}

void
current_path(const path& pth)
{
  std::error_code ec;
  current_path(pth, ec);
  if (ec)
    throw filesystem_error{"current_path", pth, ec};
}

void
current_path(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  int result = ::chdir(pth.c_str());
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
}

bool
equivalent(const path& pth1, const path& pth2)
{
  std::error_code ec;
  bool ok = equivalent(pth1, pth2, ec);
  if (ec)
    throw filesystem_error{"equivalent", pth1, pth2, ec};
  return ok;
}

bool
equivalent(const path& pth1, const path& pth2, std::error_code& ec) noexcept
{
  file_status fs1 = status(pth1, ec);
  if (ec)
    return false;
  file_status fs2 = status(pth2, ec);
  if (ec)
    return false;
  if (fs1.type() == fs2.type()) // Want s1 == s2.
    {
      errno = 0;
      struct stat buf1, buf2;
      if (::stat(pth1.c_str(), &buf1) != 0)
	ec = std::make_error_code(static_cast<std::errc>(errno));
      else if (::stat(pth2.c_str(), &buf2) != 0)
	ec = std::make_error_code(static_cast<std::errc>(errno));
      if (buf1.st_dev == buf2.st_dev && buf1.st_ino == buf2.st_ino)
	return true;
    }
  return false;
}

bool
exists(const path& pth)
{ return exists(status(pth)); }

bool
exists(const path& pth, std::error_code& ec) noexcept
{ return exists(status(pth, ec)); }

uintmax_t
file_size(const path& pth)
{
  std::error_code ec;
  uintmax_t sz = file_size(pth, ec);
  if (ec)
    throw filesystem_error{"file_size", pth, ec};
  return sz;
}

uintmax_t
file_size(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  struct stat buf;
  int result = ::stat(pth.c_str(), &buf);
  if (result == -1)
    {
      ec = std::make_error_code(static_cast<std::errc>(errno));
      return static_cast<uintmax_t>(-1);
    }
  else
    return buf.st_size == 0;
}

uintmax_t
hard_link_count(const path& pth)
{
  std::error_code ec;
  uintmax_t num_links = hard_link_count(pth, ec);
  if (ec)
    throw filesystem_error{"hard_link_count", pth, ec};
  return num_links;
}

uintmax_t
hard_link_count(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  struct stat buf;
  int result = ::stat(pth.c_str(), &buf);
  if (result == -1)
    {
      ec = std::make_error_code(static_cast<std::errc>(errno));
      return static_cast<uintmax_t>(-1);
    }
  else
    return buf.st_nlink;
}

const path&
initial_path()
{
  std::error_code ec;
  const path& init_path = initial_path(ec);
  if (ec)
    throw filesystem_error{"initial_path", ec};
  return init_path;
}

const path&
initial_path(std::error_code& ec) noexcept
{
  static bool called = false; // This is not thread safe - see n3365.html#initial_path
  static path init_path;
  if (!called)
    {
      called = true;
      init_path = current_path(ec);
    }
  return init_path;
}

bool
is_empty(const path& pth)
{
  std::error_code ec;
  bool ok = is_empty(pth, ec);
  if (ec)
    throw filesystem_error{"is_empty", pth, ec};
  return ok;
}

bool
is_empty(const path& pth, std::error_code& ec) noexcept
{
  if (is_directory(status(pth, ec)))
    {
      directory_iterator diter(pth);
      return diter == directory_iterator();
    }
  else
    return file_size(pth, ec) == 0;
}

file_time_type
last_write_time(const path& pth)
{
  std::error_code ec;
  file_time_type wt = last_write_time(pth, ec);
  if (ec)
    throw filesystem_error{"last_write_time", pth, ec};
  return wt;
}

file_time_type
last_write_time(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  struct stat buf;
  int result = ::stat(pth.c_str(), &buf);
  if (result == -1)
    {
      ec = std::make_error_code(static_cast<std::errc>(errno));
      return std::chrono::system_clock::from_time_t(-1);//static_cast<file_time_type>(-1);
    }
  else
    return std::chrono::system_clock::from_time_t(buf.st_mode);
}

void
last_write_time(const path& pth,
		const file_time_type new_time)
{
  std::error_code ec;
  last_write_time(pth, new_time, ec);
  if (ec)
    throw filesystem_error{"last_write_time", pth, ec};
}

void
last_write_time(const path& pth,
		const file_time_type new_time,
		std::error_code& ec) noexcept
{
  errno = 0;
  struct utimbuf times;
  times.actime = std::chrono::system_clock::to_time_t(new_time);
  times.modtime = std::chrono::system_clock::to_time_t(new_time);
  int result = ::utime(pth.c_str(), &times);
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
}

void
permissions(const path& pth, perms prms)
{
  std::error_code ec;
  permissions(pth, prms, ec);
  if (ec)
    throw filesystem_error{"permissions", pth, ec};
}

void
permissions(const path& pth, perms prms, std::error_code& ec) noexcept
{
  file_status current_status(((prms & perms::symlink_perms) != perms::no_perms)
			     ? symlink_status(pth, ec)
			     : status(pth, ec));
  if (ec)
    return;

  if ((prms & perms::add_perms) != perms::no_perms)
    prms |= current_status.permissions();
  else if ((prms & perms::remove_perms) != perms::no_perms)
    prms = current_status.permissions() & ~prms;

  errno = 0;
  ////int result = ::fchmodat(, pth.c_str(), static_cast<mode_t>(prms), 0);
  int result = ::chmod(pth.c_str(), static_cast<mode_t>(prms));
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
}

path
read_symlink(const path& pth)
{
  std::error_code ec;
  path link = read_symlink(pth, ec);
  if (ec)
    throw filesystem_error{"read_symlink", pth, ec};
  return link;
}

path
read_symlink(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  path link;
  if (is_symlink(pth))
    {
      std::size_t size = 128;
      while (true)
	{
	  std::unique_ptr<char> buffer{std::make_unique<char>(size)};
	  int nchars = ::readlink(pth.c_str(), buffer.get(), size);
	  if (nchars == -1)
	    {
	      ec = std::make_error_code(static_cast<std::errc>(errno));
	      return link;
	    }
	  if (nchars < size)
	    {
	      // The buffer is *not* null terminated in ::readlink!
	      link = std::string(buffer.get(), nchars);
	      return link;
	    }
	  size *= 2;
	}
    }
  return link;
}

bool
remove(const path& pth)
{
  std::error_code ec;
  bool ok = remove(pth, ec);
  if (ec)
    throw filesystem_error{"remove", pth, ec};
  return ok;
}

bool
remove(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  int result = ::remove(pth.c_str());
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
  return result == 0;
}

uintmax_t
remove_all(const path& pth)
{
  std::error_code ec;
  uintmax_t count = remove_all(pth, ec);
  if (ec)
    throw filesystem_error{"remove_all", pth, ec};
  return count;
}

// TODO: Check this!!!!
uintmax_t
remove_all(const path& pth, std::error_code& ec) noexcept
{
  static const directory_iterator end;
  uintmax_t count = 0;
  file_status fs = status(pth, ec);
  if (ec)
    return static_cast<uintmax_t>(-1);
  if (!is_symlink(fs) && is_directory(fs))
    for (directory_iterator diter(pth); diter != end; ++diter)
      {
	count += remove_all(diter->path(), ec);
	if (ec)
          return static_cast<uintmax_t>(-1);
      }
  if (remove(pth, ec))
    ++count;

  return count;
}

void
rename(const path& from, const path& to)
{
  std::error_code ec;
  rename(from, to, ec);
  if (ec)
    throw filesystem_error{"rename", from, to, ec};
}

void
rename(const path& from, const path& to, std::error_code& ec) noexcept
{
  errno = 0;
  int result = ::rename(from.c_str(), to.c_str());
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
}

void
resize_file(const path& pth, uintmax_t size)
{
  std::error_code ec;
  resize_file(pth, size, ec);
  if (ec)
    throw filesystem_error{"resize", pth, ec};
}

void
resize_file(const path& pth, uintmax_t size, std::error_code& ec) noexcept
{
  errno = 0;
  int result = ::truncate(pth.c_str(), size);
  if (result == -1)
    ec = std::make_error_code(static_cast<std::errc>(errno));
}

space_info
space(const path& pth)
{
  std::error_code ec;
  space_info si = space(pth, ec);
  if (ec)
    throw filesystem_error{"space", pth, ec};
  return si;
}

space_info
space(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  struct statvfs buf;
  int result = ::statvfs(pth.c_str(), &buf);
  space_info si;
  if (result == -1)
    {
      si.capacity = static_cast<uintmax_t>(-1);
      si.free = static_cast<uintmax_t>(-1);
      si.available = static_cast<uintmax_t>(-1);
      ec = std::make_error_code(static_cast<std::errc>(errno));
    }
  else
    {
      si.capacity = buf.f_blocks * buf.f_frsize;
      si.free = buf.f_bfree * buf.f_frsize;
      si.available = buf.f_bavail * buf.f_frsize;
    }
  return si;
}

inline file_status
status(const path& pth)
{
  std::error_code ec;
  file_status fs = status(pth, ec);
  if (fs.type() == file_type::status_error)
    throw filesystem_error{"status", pth, ec};
  return fs;
}

inline file_status
status(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  file_status fs;
  struct stat buf;
  int result = ::stat(pth.c_str(), &buf);
  if (result == -1)
    {
      if (errno == ENOENT || errno == ENOTDIR)
	{
	  fs.type(file_type::file_not_found);
	  fs.permissions(perms::perms_not_known);
	}
      else
	{
	  fs.type(file_type::status_error);
	  fs.permissions(perms::perms_not_known);
	  ec = std::make_error_code(static_cast<std::errc>(errno));
	}
    }
  else
    {
      if (S_ISREG(buf.st_mode))
	fs.type(file_type::regular_file);
      else if (S_ISDIR(buf.st_mode))
	fs.type(file_type::directory_file);
      else if (S_ISLNK(buf.st_mode))
	fs.type(file_type::symlink_file);
      else if (S_ISBLK(buf.st_mode))
	fs.type(file_type::block_file);
      else if (S_ISCHR(buf.st_mode))
	fs.type(file_type::character_file);
      else if (S_ISFIFO(buf.st_mode))
	fs.type(file_type::fifo_file);
      else if (S_ISSOCK(buf.st_mode))
	fs.type(file_type::socket_file);
      else
	fs.type(file_type::type_unknown);

      fs.permissions(static_cast<perms>(buf.st_mode | 0777));
    }

  return fs;
}

file_status
symlink_status(const path& pth)
{
  std::error_code ec;
  file_status fs = symlink_status(pth, ec);
  if (fs.type() == file_type::status_error)
    throw filesystem_error{"symlink_status", pth, ec};
  return fs;
}

file_status
symlink_status(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  file_status fs;
  struct stat buf;
  int result = ::lstat(pth.c_str(), &buf);
  if (result == -1)
    {
      if (errno == ENOENT || errno == ENOTDIR)
	{
	  fs.type(file_type::file_not_found);
	  fs.permissions(perms::perms_not_known);
	}
      else
	{
	  fs.type(file_type::status_error);
	  fs.permissions(perms::perms_not_known);
	  ec = std::make_error_code(static_cast<std::errc>(errno));
	}
    }
  else
    {
      if (S_ISREG(buf.st_mode))
	fs.type(file_type::regular_file);
      else if (S_ISDIR(buf.st_mode))
	fs.type(file_type::directory_file);
      else if (S_ISLNK(buf.st_mode))
	fs.type(file_type::symlink_file);
      else if (S_ISBLK(buf.st_mode))
	fs.type(file_type::block_file);
      else if (S_ISCHR(buf.st_mode))
	fs.type(file_type::character_file);
      else if (S_ISFIFO(buf.st_mode))
	fs.type(file_type::fifo_file);
      else if (S_ISSOCK(buf.st_mode))
	fs.type(file_type::socket_file);
      else
	fs.type(file_type::type_unknown);

      fs.permissions(static_cast<perms>(buf.st_mode | 0777));
    }

  return fs;
}

path
system_complete(const path& pth)
{
  std::error_code ec;
  path comp_path = system_complete(pth, ec);
  if (ec)
    throw filesystem_error{"system_complete", pth, ec};
  return comp_path;
}

path
system_complete(const path& pth, std::error_code& ec) noexcept
{
  errno = 0;
  char real[PATH_MAX];
  char * ptr = ::realpath(pth.c_str(), real);
  if (!ptr)
    ec = std::make_error_code(static_cast<std::errc>(errno));
  return path{std::string{real}};
}

path
temp_directory_path()
{
  std::error_code ec;
  path tmpdir = temp_directory_path(ec);
  if (ec)
    throw filesystem_error{"temp_directory_path", ec};
  return tmpdir;
}

path
temp_directory_path(std::error_code& ec) noexcept
{
  path tmpdir;
  const char * tmp = nullptr;
  const char * env = nullptr;
  if (env = getenv("TMPDIR"))
    tmp = env;
  else if (env = getenv("TMP"))
    tmp = env;
  else if (env = getenv("TEMP"))
    tmp = env;
  else if (env = getenv("TEMPDIR"))
    tmp = env;
  else
    tmp = "/tmp";
  tmpdir = tmp; // TODO: Make this better.
  if (!exists(tmpdir, ec) || !is_directory(tmpdir, ec))
    ;
  return tmpdir;
}

namespace __detail
{
namespace _Path_traits
{
  //  from_end is 0 for null terminated MBCS
  void
  convert(const char* from, const char* from_end,
	  std::wstring& to, const codecvt_type& cvt)
  {
  }

  //  from_end is 0 for null terminated MBCS
  void
  convert(const wchar_t* from, const wchar_t* from_end,
	  std::string& to, const codecvt_type& cvt)
  {
  }
} // _Path_traits
} // __detail

} // filesystem
} // experimental
} // std
