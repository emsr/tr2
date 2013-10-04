// C++ experimantal unique_path.cc -*- C++ -*-

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

#include "filesystem"

#include <random> // For unique_path randomness.
#include <sstream> // To build paths for unique_path.
#include <iomanip> // To build paths for unique_path.

namespace std {
namespace experimental {
namespace filesystem {

path
unique_path(const path& model)
{
  std::error_code ec;
  path unique = unique_path(model, ec);
  if (ec)
    throw filesystem_error("unique_path", model, ec);
  return unique;
}

path
unique_path(const path& model, std::error_code& ec)
{
  std::mt19937_64 re;
  std::random_device rd;
  re.seed(rd());
  std::ostringstream poo;
  poo << std::hex << std::setw(16)
      << std::setfill('0') << re();
  std::string pathname = model.string();
  std::size_t i = 0;
  for (std::size_t c = 0; c < pathname.length(); ++c)
    {
      if (pathname[c] == '%')
        {
	  if (i == poo.str().length())
	    {
	      i = 0;
	      re.seed(rd());
	      poo << std::hex << std::setw(16)
		  << std::setfill('0') << re();
	    }
	  pathname[c] = poo.str()[i];
	  ++i;
	}
    }
  return path{pathname};
}

} // filesystem
} // experimental
} // std
