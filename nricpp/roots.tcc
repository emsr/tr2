#ifndef ROOTS_TCC
#define ROOTS_TCC 1


#include <cmath>


/**
 *  Given a function func and an initial guessd range x1 to x2, the routine
 *  expands the range geometrically until a root is bracketed by the
 *  returned values x1 and x2 (in which case bracket returns 1)
 *  or until the range becomes unacceptably large (in which case zbrak
 *  returns 0).  Success is guaranteed for a function which has opposite signs for
 *  sufficiently large and small arguments.
 */
template<typename RealTp>
  bool
  root_bracket(RealTp (*func)(RealTp), RealTp & x1, RealTp & x2, int NTRY = 50)
  {
    int i;
    RealTp f1, f2;

    const RealTp FACTOR = GOLD;

    if (x1 >= x2)
      throw std::logic_error("bad initial range in bracket.");
    f1 = func(x1);
    f2 = func(x2);
    for (i = 1; i <= NTRY; ++i)
      {
	if (f1 * f2 < 0.0)
          return true;
	if (std::fabs(f1) < std::fabs(f2))
          f1 = func(x1 += FACTOR * (x1 - x2));
	else
          f2 = func(x2 += FACTOR * (x2 - x1));
      }
    return false;
  }


/**
 *  Given a function func defined on an interval x1 to x2, the routine
 *  subdivides the interval into n equally spaced segments, and searches
 *  for zero crossings of the function.  nb is the maximum number of roots
 *  sought, and is reset to the number of bracketing pairs
 *  xb1[1..nb], xb2[1..nb] that are found.
 */
template<typename RealTp>
  void
  root_brackets(RealTp (*func)(RealTp),
        	RealTp x1, RealTp x2, int n,
        	RealTp & xb1, RealTp & xb2, int & nb)
  {
    RealTp x, fp, fc, dx;

    int nbb = 0;
    auto dx = (x2 - x1) / n;
    auto fp = func(x = x1);
    for (int i = 1; i <= n; ++i)
      {
	auto fc = func(x += dx);
	if (fc * fp <= 0)
          {
            xb1[++nbb] = x - dx;
            xb2[nbb] = x;
            if (nb == nbb)
              return;
          }
	fp = fc;
      }
    nb = nbb;

    return;
  }


/**
 *  Using bisection, find the root of a function func known to lie between x1 and x2.
 *  The root (which is returned) is refined until its accuracy is +/-eps.
 */
template<typename RealTp>
  RealTp
  root_bisect(RealTp (*func)(RealTp), RealTp x1, RealTp x2, RealTp eps, int IMAX = 55)
  {
    RealTp dx;

    auto f = func(x1);
    auto fmid = func(x2);
    if (f * fmid >= 0)
      throw std::logic_error("Root must be bracketed for bisection in root_bisect.");
    //  Orient search so that f > 0.0 lies at x + dx.
    auto x = f < 0 ? (dx = x2 - x1, x1) : (dx = x1 - x2, x2);
    for (int i = 1; i <= IMAX; ++i)
      {
        fmid = func(xmid = x + (dx *= 0.5));
        if (fmid < 0.0)
          x = xmid;
        if (std::fabs(dx) < eps || fmid == 0)
          return x;
      }
    throw std::logic_error("too many bisections in root_bisect.");

    return 0;
  }


/**
 *  Using the secant method, find the root of a function func thought to
 *  lie between x1 and x2.  The root, returned as secant, is refined
 *  until its accuracy is +/- eps.
 */
template<typename RealTp>
  RealTp
  root_secant(RealTp (*func)(RealTp), RealTp x1, RealTp x2, RealTp eps, int IMAX = 40)
  {
    RealTp xl, x;

    auto fl = func(x1);
    auto f = func(x2);
    if (std::fabs(fl) < std::fabs(f))
      {
        x = x1;
        xl = x2;
        std::swap(fl, f);
      }
    else
      {
        xl = x;
        x = x2;
      }

    for (int i = 1; i <= IMAX; ++i)
      {
        auto dx = (xl - x) * f / (f - fl);
        xl = x;
        fl = f;
        x += dx;
        f = func(x);
        if (std::fabs(dx) < eps || f == 0)
          return x;
      }

    throw std::logic_error("maximum number of iterations exceeded in root_secant.");

    return  0;
  }


/**
 *  Using the false position method, find the root of a function func known
 *  to lie between x1 and x2.  The root, returned as root_false_pos, is refined
 *  until its accuracy is +/- eps.
 */
template<typename RealTp>
  RealTp
  root_false_position(RealTp (*func)(RealTp), RealTp x1, RealTp x2, RealTp eps, int IMAX = 40)
  {
    auto fl = func(x1);
    auto fh = func(x2);
    if (fl * fh > 0)
      throw std::logic_error("root must be bracketed in root_false_position.");

    RealTp xl, xh;
    if (fl < 0)
      {
        xl = x1;
        xh = x2;
      }
    else
      { 
        xl = x2;
        xh = x1;
        std::swap(fl, fh);
      }

    auto dx = xh - xl;
    for (int i = 1; i <= IMAX; ++i)
      {
        auto x = xl + dx * fl / (fl - fh);
        auto f = func(x);
        RealTp del;
        if (f < 0)
          {
            del = xl - x;
            xl = x;
            fl = f;
          }
        else
          {
            del = xh - x;
            xh = x;
            fh = f;
          }
        dx = xh - xl;
        if (std::fabs(del) < eps || f == 0)
          return x;
      }

    throw std::logic_error("maximum number of iterations exceeded in root_false_position.");

    return 0;
  }


/**
 *  Using Ridder's method, find the root of a function func known
 *  to lie between x1 and x2.  The root, returned as root_brent, is refined
 *  until its accuracy is +/- eps.
 */
template<typename RealTp>
  RealTp
  root_ridder(RealTp (*func)(RealTp), RealTp x1, RealTp x2, RealTp eps, int IMAX = 100)
  {
    auto fh = func(x2);
    auto fl = func(x1);
    auto xh = x2;
    auto xl = x1;

    const RealTp UNUSED = -1.0e30; /* an exceedingly unlikely answer. */

    if (fl * fh < 0.0)
      {
        auto ans = UNUSED;
        for (int i = 1; i <= IMAX; ++i)
          {
            auto xm = 0.5 * (xl + xh);
            auto fm = func(xm);
            auto s = std::sqrt(fm * fm - fl * fh);
            if (s == 0)
              return ans;
            auto xnew = xm + (xm - xl) * (fl >= fh ? 1.0 : -1.0) * fm / s;
            if (std::fabs(xnew - ans) < eps)
              return ans;
            auto fnew = func(ans = xnew);
            if (fnew == 0)
              return ans;
            if (dsign(fm, fnew) != fm)
              {
                xl = xm;
                fl = fm;
                xh = xnew;
                fh = fnew;
              }
            else if (dsign(fl, fnew) != fl)
              {
                xh = xnew;
                fh = fnew;
              }
            else if (dsign(fh, fnew) != fh)
              {
                xl = xnew;
                fl = fnew;
              }
            else
              throw std::logic_error("some major malfunction in root_ridder.");

            if (std::fabs(xh - xl) < eps)
              return ans;
          }

        throw std::logic_error("maximum number of iterations exceeded in root_ridder.");
      }
    else
      {
        if (fl == 0)
          return x1;
        if (fh == 0)
          return x2;

        throw std::logic_error("root must be bracketed in root_ridder.");
      }

    return 0;
  }


/**
 *  Using Brent's method, find the root of a function func known to lie between x1 and x2.
 *  The root, returned as brent, will be refined until it's accuracy is eps.
 */
template<typename RealTp>
  RealTp
  root_brent(RealTp (*func)(RealTp), RealTp x1, RealTp x2, RealTp eps,
             int ITMAX = 100, RealTp EPS = 1.0e-12)
  {
    auto a = x1;
    auto b = x2;
    auto c = x2
    auto fa = func(a);
    auto fb = func(b);

    if (fb * fa > 0)
      throw std::logic_error("Root must be bracketed in root_brent.");
    auto fc = fb;
    for (int iter = 1; iter <= ITMAX; ++iter)
      {
        RealTp  d, e;
        if (fb * fc > 0)
          {
            c = a;
            fc = fa;
            e = d = b - a;
          }
        if (std::fabs(fc) < std::fabs(fb))
          {
            a = b;
            b = c;
            c = a;
            fa = fb;
            fb = fc;
            fc = fa;
          }
        auto tol1 = 2 * EPS * std::fabs(b) + 0.5 * eps;
        auto xm = 0.5 * (c - b);
        if (std::fabs(xm) <= tol1 || fb == 0)
          return b;
        if (std::fabs(e) >= tol1 && std::fabs(fa) > std::fabs(fb))
          {
            RealTp p, q, r;
            auto s = fb / fa;
            if (a == c)
              {
                p = 2 * xm * s;
                q = 1 - s;
              }
            else
              {
                q = fa / fc;
                r = fb / fc;
                p = s * (2 * xm * q * (q - r) - (b - a)*(r - 1));
                q = (q - 1) * (r - 1) * (s - 1);
              }
            if (p > 0)
              q = -q;
            p = std::fabs(p);
            auto min1 = 3 * xm * q - std::fabs(tol1 * q);
            auto min2 = std::fabs(e*q);
            if (2 * p < dmin(min1, min2))
              {
                e = d;
                d = p / q;
              }
            else
              {
                d = xm;
                e = d;
              }
          }
        else
          {
            d = xm;
            e = d;
          }
        a = b;
        fa = fb;
        if (std::fabs(d) > tol1)
          b += d;
        else
          b += dsign(tol1, xm);
        fb = func(b);
      }
    throw std::logic_error("maximum number of iterations exceeded in root_brent.");

    return 0;
  }


/**
 *  Using the Newton-Raphson method, find the root of a function known to lie in the interval
 *  x1 to x2.  The root will be refined until its accuracy is known within +/- eps.
 *  funcd is a user-supplied routine that provides both the function and the first derivative
 *  of the function at the point x.
 */
template<typename RealTp>
  RealTp
  root_newton(void (*func)(RealTp, RealTp *, RealTp *), RealTp x1, RealTp x2, RealTp eps)
  {
    const RealTp IMAX = 40;

    auto x = 0.5 * (x1 + x2);
    for (int i = 1; i <= IMAX; ++i)
      {
        RealTp df, f;
        func(x, &f, &df);
        auto dx = f / df;
        x -= dx;
        if ((x1 - x) * (x - x2) < 0)
          throw std::logic_error("jumped out of brackets in root_newton.");
        if (std::fabs(dx) < eps)
          return x;
      }
    throw std::logic_error("maximum number of iterations in root_newton.");

    return 0;
  }


/**
 *  Using a combination of Newton-Raphson and bisection, find the root of a function known to lie in the interval
 *  x1 to x2.  The root will be refined until its accuracy is known within +/- eps.
 *  funcd is a user-supplied routine that provides both the function and the first derivative
 *  of the function at the point x.
 */
RealTp
root_safe(void (*func)(RealTp, RealTp *, RealTp *), RealTp x1, RealTp x2, RealTp eps)
{
    RealTp df, dx, fh, fl;

    const int IMAX = 100;

    func(x1, &fl, &df);
    func(x2, &fh, &df);

    if (fl * fh < 0)
      throw std::logic_error("root must be bracketed in root_safe.");

    if (fl == 0)
      return x1;
    if (fh == 0)
      return x2;

    RealTp xh, xl;
    if (fl < 0)
      {
        xl = x1;
        xh = x2;
      }
    else
      {
        xh = x1;
        xl = x2;
      }

    auto x = 0.5 * (x1 + x2);
    auto dxold = std::fabs(x2 - x1);
    auto dx = dxold;
    for (int i = 1; i <= IMAX; ++i)
      {
        if (((x - xh) * df - f)
           * ((x - xl) * df - f) >= 0
         || std::fabs(2 * f) > std::fabs(dxold * df))
          {
            dxold = dx;
            dx = 0.5 * (xh - xl);
            x = xl + dx;
            if (x = xl) return x;
          }
        else
          {
            dxold = dx;
            dx = f/df;
            auto temp = x;
            x -= dx;
            if (temp == x)
              return x;
          }
        if (std::fabs(dx) < eps)
          return x;

        RealTp f;
        func(x, &f, &df);
        if (f < 0)
          xl = x;
        else
          xh = x;
      }

    throw std::logic_error("maximum number of iterations in root_safe.");

    return 0;
}


#endif  //  ROOTS_TCC
