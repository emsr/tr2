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

#include <experimental/string_view>

namespace std
{
namespace experimental
{
namespace filesystem
{
inline namespace v1
{

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

int
path::compare(const path& pth) const noexcept
{
  return __detail::lex_compare(this->begin(), this->end(),
			       pth.begin(), pth.end());
}

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
  std::experimental::string_view filename{this->filename().string()};
  std::size_t len = filename.length();
  std::size_t pos = filename.rfind(_S_dot);
  if (pos != string_type::npos
   && !(len > 0 && filename[0] == _S_dot && (filename[1] == _S_nul
	|| (len > 1 && filename[1] == _S_dot && filename[2] == _S_nul))))
    return path{filename.substr(0, pos - 1)};
  else
    return filename;
}

path
path::extension() const
{
  std::experimental::string_view filename{this->filename().string()};
  std::size_t len = filename.length();
  std::size_t pos = filename.rfind(_S_dot);
  if (pos != std::string::npos
   && !(len > 0 && filename[0] == _S_dot && (filename[1] == _S_nul
	|| (len > 1 && filename[1] == _S_dot && filename[2] == _S_nul))))
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
    if (new_extension._M_pathname[0] != _S_dot)
      this->_M_pathname.push_back(_S_dot);
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

} // inline namespace v1
} // namespace filesystem
} // namespace experimental
} // namespace std
