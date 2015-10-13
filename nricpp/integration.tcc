#ifndef INTEGRATION_TCC
#define INTEGRATION_TCC 1


#include <cmath>
#include <stdexcept>
#include <vector>
#include <limits>


/**
 *    Trapeziod rule integration.
 *
 *    This routine implements the nth stage of refinement of an extended trapezoid rule.
 *    With n = 1, the crudest estimate of the integral is returned.
 *    With successive calls with n = 2, 3, ... (in order) accuracy will be improved
 *    by adding 2^(n-2) interior points.
 */
  double
  trapezoid(double (*func)(double), double a, double b, int n)
  {
    static thread_local double s;
    static thread_local int lastn, it;

    if (n <= 0)
      throw std::logic_error("Non-positive order in trapezoid.");
    if (n != 1 && n != lastn + 1)
      throw std::logic_error("Order out of sequence in trapezoid.");
    lastn = n;

    if (n == 1)
      {
	s = 0.5 * (b - a) * (func(a) + func(b));
	it = 1;
      }
    else
      {
	auto del = (b - a) / it;
	auto x = a + 0.5 * del;
	auto sum = 0.0;
	for (int j = 1; j <= it; ++j, x += del)
          sum += func(x);
	s = 0.5 * (s + (b - a) * sum / it);
	it *= 2;
      }

    return s;
  }


/**
 *    Modified midpoint integration.
 *
 *    This routine implements the nth stage of refinement of a modified midpoint integration.
 *    With n = 1, the crudest estimate of the integral is returned.
 *    With successive calls with n = 2, 3, ... (in order) accuracy will be improved
 *    by adding (2/3)^(n-1) interior points.
 */
  double
  midpoint(double (*func)(double), double a, double b, int n)
  {
    static thread_local int lastn;
    static thread_local double s;

    if (n <= 0)
      throw std::logic_error("Non-positive order in midpoint.");
    if (n != 1 && n != lastn + 1)
      throw std::logic_error("Order out of sequence in midpoint.");
    lastn = n;

    if (n == 1)
      {
	auto x = 0.5 * (a + b);
	return s = (b - a) * func(x);
      }
    else
      {
	auto it = 1;
	for (int j = 1; j < n - 1; ++j)
          it *= 3;
	auto tnm = it;

	//  The added points alternate in spacing between del and ddel.
	auto del = (b - a) / (3 * tnm);
	auto ddel = 2 * del;

	auto x = a + 0.5 * del;
	auto sum = 0.0;
	for (int j = 1; j <= it; ++j)
          {
            sum += func(x);
            x += ddel;
            sum += func(x);
            x += del;
          }

	//  The new sum is combined with the old integral to give a refined integral.
	return s = (s + (b - a) * sum / tnm) / 3;
    }
  }


/**
 *    This routine is an exact replacement of midpoint except that the
 *    points are evenly spaced in 1/x rather than x.  This allows the
 *    lower limit aa to be as large and negative or the upper limit
 *    bb to be as large and positive as the computer allows but not both.
 *    aa and bb must have the same sign.
 */
  double
  midpoint_inv(double (*func)(double), double aa, double bb, int n)
  {
    static thread_local int lastn;
    static thread_local double s;

    if (n <= 0)
      throw std::logic_error("Non-positive order in midpoint.");
    if (n != 1 && n != lastn + 1)
      throw std::logic_error("Order out of sequence in midpoint.");
    lastn = n;

    auto a = 1 / aa;
    auto b = 1 / bb;

    if (n == 1)
      {
	auto x = 0.5 * (a + b);
	return s = (b - a) * func(1 / x) / (x * x);
      }
    else
      {
	auto it = 1;
	for (int j = 1; j < n - 1; ++j)
          it *= 3;
	auto tnm = it;

	//  The added points alternate in spacing between del and ddel.
	auto del = (b - a) / (3 * tnm);
	auto ddel = 2 * del;

	auto x = a + 0.5 * del;
	auto sum = 0.0;
	for (int j = 1; j <= it; ++j)
          {
            sum += func(1 / x) / (x * x);
            x += ddel;
            sum += func(1 / x) / (x * x);
            x += del;
          }

	//  The new sum is combined with the old integral to give a refined integral.
	return s = (s + (b - a) * sum / tnm) / 3;
      }
  }


/**
 *    This routine is an exact replacement of midpoint except that it allows
 *    for an inverse square root singularity at the upper limit bb.
 */
  double
  midpoint_inv_sqrt_lower(double (*func)(double), double aa, double bb, int n)
  {
    static thread_local int lastn;
    static thread_local double s;

    if (n <= 0)
      throw std::logic_error("Non-positive order in midpoint.");
    if (n != 1 && n != lastn + 1)
      throw std::logic_error("Order out of sequence in midpoint.");
    lastn = n;

    auto a = 0.0;
    auto b = std::sqrt(bb - aa);

    if (n == 1)
      {
	auto x = 0.5 * (a + b);
	return s = (b - a) * 2 * x * func(aa + x * x);
      }
    else
      {
	auto it = 1;
	for (int j = 1; j < n - 1; ++j)
          it *= 3;
	auto tnm = it;

	//  The added points alternate in spacing between del and ddel.
	auto del = (b - a) / (3 * tnm);
	auto ddel = 2 * del;

	auto x = a + 0.5 * del;
	auto sum = 0.0;
	for (int j = 1; j <= it; ++j)
          {
            sum += 2 * x * func(aa + x * x);
            x += ddel;
            sum += 2 * x * func(aa + x * x);
            x += del;
          }
	//  The new sum is combined with the old integral to give a refined integral.
	return s = (s + (b - a) * sum / tnm) / 3;
      }
  }


/**
 *    This routine is an exact replacement of midpoint except that it allows
 *    for an inverse square root singularity at the upper limit bb.
 */
  double
  midpoint_inv_sqrt_upper(double (*func)(double), double aa, double bb, int n)
  {
    static thread_local int lastn;
    static thread_local double s;

    if (n <= 0)
      throw std::logic_error("Non-positive order in midpoint.");
    if (n != 1 && n != lastn + 1)
      throw std::logic_error("Order out of sequence in midpoint.");
    lastn = n;

    auto a = 0.0;
    auto b = std::sqrt(bb - aa);

    if (n == 1)
      {
	auto x = 0.5 * (a + b);
	return s = (b - a) * 2 * x * func(bb - x*x);
      }
    else
      {
	auto it = 1;
	for (int j = 1; j < n - 1; ++j)
          it *= 3;
	auto tnm = it;

	//  The added points alternate in spacing between del and ddel.
	auto del = (b - a) / (3 * tnm);
	auto ddel = 2 * del;

	auto x = a + 0.5 * del;
	auto sum = 0.0;
	for (int j = 1; j <= it; ++j)
          {
            sum += 2 * x * func(bb - x * x);
            x += ddel;
            sum += 2 * x * func(bb - x * x);
            x += del;
          }
	//  The new sum is combined with the old integral to give a refined integral.
	return s = (s + (b - a) * sum / tnm) / 3;
      }
  }


/**
 *    This routine is an exact replacement of midpoint except that bb is assumed
 *    to be infinite (input value is not actually used).  It is assumed that the
 *    function func decreases exponentially rapidly at infinity.
 */
  double
  midpoint_exp(double (*func)(double), double aa, double bb, int n)
  {
    static thread_local int lastn;
    static thread_local double s;

    if (n <= 0)
      throw std::logic_error("Non-positive order in midpoint.");
    if (n != 1 && n != lastn + 1)
      throw std::logic_error("Order out of sequence in midpoint.");
    lastn = n;

    auto a = 0.0;
    auto b = std::exp(-aa);

    if (n == 1)
      {
	auto x = 0.5 * (a + b);
	return (s = (b - a) * func(-std::log(x)) / x);
      }
    else 
      {
	auto it = 1;
	for (int j = 1; j < n - 1; ++j)
          it *= 3;
	auto tnm = it;

	//  The added points alternate in spacing between del and ddel.
	auto del = (b - a) / (3 * tnm);
	auto ddel = 2 * del;

	auto x = a + 0.5 * del;
	auto sum = 0.0;
	for (int j = 1; j <= it; ++j)
          {
            sum += func(-std::log(x)) / x;
            x += ddel;
            sum += func(-std::log(x)) / x;
            x += del;
          }

	//  The new sum is combined with the old integral to give a refined integral.
	return s = (s + (b - a) * sum / tnm) / 3;
      }
  }


/**
 *    This routine is an exact replacement of midpoint except that aa is assumed
 *    to be zero (input value is not actually used).  It is assumed that the
 *    function func has a logarithmic singularity at 0.
 */
  double
  midpoint_log(double (*func)(double), double aa, double bb, int n)
  {
    static thread_local int lastn;
    static thread_local double s;

    if (n <= 0)
      throw std::logic_error("Non-positive order in midpoint.");
    if (n != 1 && n != lastn + 1)
      throw std::logic_error("Order out of sequence in midpoint.");
    lastn = n;

    auto a = -std::log(bb);
    auto b = -std::log(0.01);

    if (n == 1)
      {
	auto x = (a + b) / 2;
	return (s = -(b - a) * x * func(std::exp(-x)));
      }
    else
      {
	auto it = 1;
	for (int j = 1; j < n - 1; ++j)
          it *= 3;
	auto tnm = it;

	//  The added points alternate in spacing between del and ddel.
	auto del = (b - a) / (3 * tnm);
	auto ddel = 2 * del;

	auto x = a + 0.5 * del;
	auto sum = 0.0;
	for (int j = 1; j <= it; ++j)
          {
            sum += x * func(std::exp(-x));
            x += ddel;
            sum += x * func(std::exp(-x));
            x += del;
          }

	//  The new sum is combined with the old integral to give a refined integral.
	return s = (s - (b - a) * sum / tnm) / 3;
      }
  }


/**
 *    Runs through n steps of the trapezoid rule integration
 *    of a function func of one real variable from a to b.
 */
  double
  dumb_trapezoid(double (*func)(double), double a, double b, int n, int JMAX = 20)
  {
    if (n <= 0)
      throw std::logic_error("Non-positive order in dumb_trapezoid.");
    if (n > JMAX)
      throw std::logic_error("Order too large in dumb_trapezoid.");

    double s;
    for (int j = 1; j <= n; ++j)
      s = trapezoid(func, a, b, j);

    return s;
  }


/**
 *    Integrates a function of one real variable over the interval a to b
 *    using trapezoid rule integration.  Integration steps are taken until
 *    the difference between successive steps is less than eps.
 */
  double
  quad_trapezoid(double (*func)(double), double a, double b, double eps, int JMAX = 20)
  {
    if (eps <= 0.0)
      throw std::logic_error("Error tolerance eps must be greater than 0 in quad_trapezoid.");

    auto olds = -std::numeric_limits<double>::max();
    for (int j = 1; j <= JMAX; ++j)
      {
	auto s = trapezoid(func, a, b, j);
	if (std::abs(s - olds) < eps * std::abs(olds))
          return s;
	if (std::abs(s) < eps && std::abs(olds) < eps && j > 6)
          return s;
	olds = s;
      }

    throw std::logic_error("Too many steps in routine quad_trapezoid.");

    return 0.0;
  }


/*
 *    Runs through n steps of the Simpson rule integration
 *    of a function func of one real variable from a to b.
 */
  double
  dumb_simpson(double (*func)(double), double a, double b, int n, int JMAX = 20)
  {
    if (n <= 0)
      throw std::logic_error("Non-positive order in dumb_simpson.");
    if (n > JMAX)
      throw std::logic_error("Order too large in dumb_simpson.");

    auto s = 0.0;
    auto ost = trapezoid(func, a, b, 1);
    for (int j = 2; j <= n; ++j)
      {
	auto st = trapezoid(func, a, b, j);
	s = (4 * st - ost) / 3;
	ost = st;
      }

    return s;
  }


/*
 *    Integrates a function of one real variable over the interval a to b
 *    using Simpson rule integration.  Integration steps are taken until
 *    the difference between successive steps is less than eps.
 */
  double
  quad_simpson(double (*func)(double), double a, double b, double eps)
  {
    const int JMAX = 20;

    if (eps <= 0.0)
      throw std::logic_error("Error tolerance eps must be greater than 0 in quad_simpson.");

    auto oldst = -1.0e30;
    auto olds = -1.0e30;
    for (int j = 1; j <= JMAX; ++j)
      {
	auto st = trapezoid(func, a, b, j);
	auto s = (4 * st - oldst) / 3;
	if (std::abs(s - olds) < eps * std::abs(olds))
          return s;
	if (std::abs(s) < eps && std::abs(olds) < eps && j > 6)
          return s;
	olds = s;
	oldst = st;
      }

    throw std::logic_error("Too many steps in routine quad_simpson.");

    return 0.0;
  }


/**
 *    Runs through n steps of the Romberg integration
 *    of a function func of one real variable from a to b.
 */
  double
  dumb_romberg(double (*func)(double), double a, double b, int n)
  {
    const int K = 5;
    const int JMAX = 20;

    if (n <= 0)
      throw std::logic_error("Non-positive order in dumb_romberg.");
    if (n > JMAX)
      throw std::logic_error("Order too large in dumb_romberg.");

    std::vector<double> s(JMAX);
    std::vector<double> h(JMAX + 1);

    h[0] = 1.0;
    double ss, dss;
    for (int j = 0; j < n; ++j)
      {
	s[j] = trapezoid(func, a, b, j);
	if (j >= K)
          {
            poly_interp(&h[j - K], &s[j - K], K, 0.0, &ss, &dss);
          }
	h[j + 1] = h[j] / 4;
      }

    return ss;
  }


/**
 *    Integrates a function of one real variable over the interval a to b
 *    using Romberg integration.  Integration steps are taken until the
 *    difference between successive steps is less than eps.
 */
  double
  quad_romberg(double (*func)(double), double a, double b, double eps, int JMAX = 20)
  {
    const int K = 5;

    if (eps <= 0.0)
      throw std::logic_error("Error tolerance eps must be greater than 0 in quad_romberg.");

    std::vector<double> s(JMAX);
    std::vector<double> h(JMAX + 1);

    h[0] = 1.0;
    for (int j = 0; j < JMAX; ++j)
      {
	s[j] = trapezoid(func, a, b, j);
	if (j >= K)
          {
            double ss, dss;
            poly_interp(h.begin() + j - K, s.begin() + j - K, K, 0.0, ss, dss);
            if (std::abs(dss) < eps * std::abs(ss))
              return ss;
            if (std::abs(dss) < eps && std::abs(ss) < eps && j > 6)
              return ss;
          }
	h[j + 1] = h[j] / 4;
      }

    throw std::logic_error("Too many steps in routine quad_romberg.");

    return 0.0;
  }


/**
 *    Romberg integration on an open interval.  Returns the integral of a function
 *    func from a to b using any specified integration routine choose and Romberg's method.
 *    Normally, choose will be an open formula, not evaluating the function at the endpoints.
 *    It is assumed that choose triples the number of steps on each call, and that
 *    it's error series contains only even powers of the steps.  The routines
 *    midpoint, midpoint_inv, midpoint_inv_sqrt_lower, midpoint_inv_sqrt_upper, and
 *    midpoint_exp are possible choices for choose.
 */
  double
  quad_romberg_open(double (*func)(double), double a, double b, double eps, int JMAX = 14,
                    double (*choose)(double (*)(double), double, double, int))
  {
    const int K = 5;

    if (eps <= 0.0)
      throw std::logic_error("Error tolerance eps must be greater than 0 in quad_romberg_open.");

    std::vector<double> s(JMAX);
    std::vector<double> h(JMAX + 1);

    h[0] = 1.0;
    for (int j = 0; j < JMAX; ++j)
      {
	s[j] = (*choose)(func, a, b, j);
	if (j >= K)
          {
            double ss, dss;
            poly_interp(h.begin() + j - K, s.begin() + j - K, K, 0.0, ss, dss);
            if (std::abs(dss) < eps * std::abs(ss))
              return ss;
            if (std::abs(dss) < eps && std::abs(ss) < eps && j > K + 1)
              return ss;
          }
	h[j + 1] = h[j] / 9;
      }

    throw std::logic_error("Too many steps in routine quad_romberg_open.");

    return 0.0;
  }


#endif  //  INTEGRATION_TCC
