// -*- C++ -*-
// Integration utilities for the C++ library testsuite.
//
// Copyright (C) 2015 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.
//
// Ported from GSL by Jason Dick
// Originally written by Brian Gaugh
//
//Used to interpret the error estimate of the itnegration routines
//Based upon gsl-1.9/integration/err.c

#include <cmath>
#include <limits>

#ifndef ERR_H
#define ERR_H

namespace __gnu_test
{
  template<typename _Tp>
    _Tp
    rescale_error(_Tp err, _Tp result_abs, _Tp result_asc)
    {
      err = std::abs(err);
      if (result_asc != 0 && err != 0)
	{
          double scale = std::pow((200 * err / result_asc), _Tp(1.5L));

          if (scale < 1)
            err = result_asc*scale;
          else
            err = result_asc;
	}
      if (result_abs > std::numeric_limits<_Tp>::min()
                  / (50 * std::numeric_limits<_Tp>::epsilon()))
	{
          double min_err = 50 * std::numeric_limits<_Tp>::epsilon() * result_abs;

          if (min_err > err)
            err = min_err;
	}

      return err;
    }

} // namespace __gnu_test

#endif // ERR_H
