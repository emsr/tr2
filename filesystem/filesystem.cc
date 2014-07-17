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

#include <memory> // For unique_ptr
#include <dirent.h> // For PATH_MAX
#include <unistd.h>
#include <cerrno>
#include <sys/statvfs.h> // For statvfs
#include <sys/stat.h> // For stat
#include <fcntl.h> // For creat
#include <utime.h> // For utime.
#include <fstream>

namespace std
{
namespace experimental
{
namespace filesystem
{
inline namespace v1
{

// Operational functions

path
absolute(const path& pth, const path& base)
{
  path abase = base;
  if (!abase.is_absolute())
    abase = absolute(abase);
  if (pth.has_root_name())
    {
      if (pth.has_root_directory())
	return pth;
      else
	{
	  return pth.root_name()
	       / abase.root_directory()
	       / abase.relative_path()
	       / pth.relative_path();
	}
    }
  else
    {
      if (pth.has_root_directory())
	abase.root_name() / pth;
      else
	abase / pth;
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
  if ((options & copy_options::create_symlinks) != copy_options::none
   || (options & copy_options::skip_symlinks) != copy_options::none)
    from_stat = symlink_status(from, ec);
  else
    from_stat = status(from, ec);
  if (ec)
    return;

  file_status to_stat;
  if ((options & copy_options::create_symlinks) != copy_options::none
   || (options & copy_options::skip_symlinks) != copy_options::none)
    to_stat = symlink_status(to, ec);
  else
    to_stat = status(to, ec);
  if (ec)
    return;

  if (!exists(from_stat))
    ec = std::make_error_code(std::errc::no_such_file_or_directory);
  else if (equivalent(from, to))
    ec = std::make_error_code(static_cast<std::errc>(111)); // ???
  else if (is_other(from_stat) || is_other(to_stat))
    ec = std::make_error_code(std::errc::not_supported);
  else if (is_directory(from_stat) && is_regular_file(to_stat))
    ec = std::make_error_code(std::errc::not_a_directory); // is_a_directory?
  if (ec)
    return;

  if (is_symlink(from_stat))
    {
      if ((options & copy_options::skip_symlinks)
	   != copy_options::none)
	return;
      else if (!exists(to_stat) && (options & copy_options::copy_symlinks)
				   != copy_options::none)
	copy_symlink(from, to); // , options, ec
      else
	{
	  if (!exists(to_stat))
	    ec = std::make_error_code(std::errc::no_such_file_or_directory);
	  else if ((options & copy_options::copy_symlinks)
		   != copy_options::none)
	    ec = std::make_error_code(std::errc::function_not_supported); // ???
	}
    }
  else if (is_regular_file(from_stat))
    {
      if ((options & copy_options::directories_only)
	  != copy_options::directories_only)
	{
	  if ((options & copy_options::directories_only)
	       != copy_options::none)
	    return;
	  else if ((options & copy_options::create_symlinks)
		    != copy_options::none)
	    create_symlink(from, to); // , ec
	  else if ((options & copy_options::create_hard_links)
		    != copy_options::none)
	    create_hard_link(from, to); // , ec
          else if (is_directory(to_stat))
	    copy_file(from, to / from.filename(), options); // , ec
          else
	    copy_file(from, to, options, ec);
	}
    }
  else if (is_directory(from_stat)
	   && ((options & copy_options::recursive) != copy_options::none
	       || options == copy_options::none))
    {
      if (!exists(to_stat))
	{
          create_directory(to, from); // , ec
//TODO//	  for (auto tmp : directory_iterator{from})
//TODO//	    copy(tmp.path(), to / tmp.path().filename(),
//TODO//		 options | copy_options::none); // copy_options::unspecified!!!
	}
    }
  else
    return;
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
	  ec = std::make_error_code(std::errc::file_exists);
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
      ec = std::make_error_code(std::errc::io_error); // ???
      return;
    }
  std::ofstream tof(to.string(), std::ios::binary);
  if (tof.fail())
    {
      ec = std::make_error_code(std::errc::io_error); // ???
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
      if (!is_directory(fs))
	ec = std::make_error_code(std::errc::not_a_directory);
      else if (!pth.empty())
	ec = std::make_error_code(std::errc::directory_not_empty);
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

bool
create_directory(const path& pth, const path& attributes)
{
  std::error_code ec;
  bool ok = create_directory(pth, attributes, ec);
  if (ec)
    throw filesystem_error{"create_directory", pth, ec};
  return ok;
}

bool
create_directory(const path& pth, const path& attributes,
		 std::error_code& ec) noexcept
{
  errno = 0;
  struct stat buf;
  int result = ::stat(attributes.c_str(), &buf);
  if (result == -1)
    {
      ec = std::make_error_code(static_cast<std::errc>(errno));
      return false;
    }
  else
    {
      result = ::mkdir(pth.c_str(), buf.st_mode);
      if (result == -1)
	ec = std::make_error_code(static_cast<std::errc>(errno));
      return result == 0;
    }
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
exists(const path& pth)
{ return exists(status(pth)); }

bool
exists(const path& pth, std::error_code& ec) noexcept
{ return exists(status(pth, ec)); }

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
      return file_time_type::min();
    }
  else
    return __detail::file_clock_type::from_time_t(buf.st_mode);
}

void
last_write_time(const path& pth,
		file_time_type new_time)
{
  std::error_code ec;
  last_write_time(pth, new_time, ec);
  if (ec)
    throw filesystem_error{"last_write_time", pth, ec};
}

void
last_write_time(const path& pth,
		file_time_type new_time,
		std::error_code& ec) noexcept
{
  errno = 0;
  struct utimbuf times;
  times.actime = __detail::file_clock_type::to_time_t(new_time);
  times.modtime = __detail::file_clock_type::to_time_t(new_time);
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
  file_status current_status{status(pth, ec)};
  if (ec)
    return;

  bool add = (prms & perms::add_perms) != perms::none;
  bool remove = (prms & perms::remove_perms) != perms::none;
  if (add)
    prms = current_status.permissions() | (prms & perms::mask);
  if (remove)
    prms = current_status.permissions() & ~(prms & perms::mask);
  if (!(add || remove))
    prms = prms & perms::mask;
  // if (add && remove) noop

  errno = 0;
  path apath = absolute(pth);
  int result = ::fchmodat(0, apath.c_str(), static_cast<mode_t>(prms), 0);
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
	  auto buffer = std::make_unique<char>(size);
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
  if (fs.type() == file_type::none)
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
      std::errc err = static_cast<std::errc>(err);
      if (err == std::errc::no_such_file_or_directory
       || err == std::errc::not_a_directory)
	{
	  fs.type(file_type::not_found);
	  fs.permissions(perms::unknown);
	}
      else
	{
	  fs.type(file_type::none);
	  fs.permissions(perms::unknown);
	  ec = std::make_error_code(err);
	}
    }
  else
    {
      if (S_ISREG(buf.st_mode))
	fs.type(file_type::regular);
      else if (S_ISDIR(buf.st_mode))
	fs.type(file_type::directory);
      else if (S_ISLNK(buf.st_mode))
	fs.type(file_type::symlink);
      else if (S_ISBLK(buf.st_mode))
	fs.type(file_type::block);
      else if (S_ISCHR(buf.st_mode))
	fs.type(file_type::character);
      else if (S_ISFIFO(buf.st_mode))
	fs.type(file_type::fifo);
      else if (S_ISSOCK(buf.st_mode))
	fs.type(file_type::socket);
      else
	fs.type(file_type::unknown);

      fs.permissions(static_cast<perms>(buf.st_mode | 0777));
    }

  return fs;
}

file_status
symlink_status(const path& pth)
{
  std::error_code ec;
  file_status fs = symlink_status(pth, ec);
  if (fs.type() == file_type::none)
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
      std::errc err = static_cast<std::errc>(err);
      if (err == std::errc::no_such_file_or_directory
       || err == std::errc::not_a_directory)
	{
	  fs.type(file_type::not_found);
	  fs.permissions(perms::unknown);
	}
      else
	{
	  fs.type(file_type::none);
	  fs.permissions(perms::unknown);
	  ec = std::make_error_code(err);
	}
    }
  else
    {
      if (S_ISREG(buf.st_mode))
	fs.type(file_type::regular);
      else if (S_ISDIR(buf.st_mode))
	fs.type(file_type::directory);
      else if (S_ISLNK(buf.st_mode))
	fs.type(file_type::symlink);
      else if (S_ISBLK(buf.st_mode))
	fs.type(file_type::block);
      else if (S_ISCHR(buf.st_mode))
	fs.type(file_type::character);
      else if (S_ISFIFO(buf.st_mode))
	fs.type(file_type::fifo);
      else if (S_ISSOCK(buf.st_mode))
	fs.type(file_type::socket);
      else
	fs.type(file_type::unknown);

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

// 32. [PDTS] system_complete() example needs clarification
// For POSIX based operating systems, system_complete(pth) has the same semantics
// as absolute(pth, current_path()).
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
  const char* tmp = nullptr;
  const char* env = nullptr;
  if (env = ::getenv("TMPDIR"))
    tmp = env;
  else if (env = ::getenv("TMP"))
    tmp = env;
  else if (env = ::getenv("TEMP"))
    tmp = env;
  else if (env = ::getenv("TEMPDIR"))
    tmp = env;
  else
    tmp = "/tmp";
  tmpdir = tmp;
  if (!exists(tmpdir, ec))
    ec = std::make_error_code(std::errc::no_such_file_or_directory);
  else if (!is_directory(tmpdir, ec))
    ec = std::make_error_code(std::errc::not_a_directory);
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

} // inline namespace v1
} // namespace filesystem
} // namespace experimental
} // namespace std
