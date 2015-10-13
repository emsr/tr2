#ifndef INTERPOLATION_TCC
#define INTERPOLATION_TCC 1


#include <vector>
#include <limits>
#include <cmath>
#include <experimental/optional>


/**
 *  
 */
template<typename RealTp>
  void
  polynomial_interp(RealTp *xa, RealTp *ya, int n, RealTp x, RealTp & y, RealTp & dy)
  {
    int ns = 1;

    std::vector<RealTp> c(n);
    std::vector<RealTp> d(n);

    auto dif = std::fabs(x - xa[0]);

    //  Here we find the index of the closest table entry,
    for (i = 0; i < n; ++i)
      {
        auto dift = std::fabs(x - xa[i]);
        if (dift < dif)
          {
            ns = i;
            dif = dift;
          }
        //  And initialize the tableau of c's and d's.
        d[i] = c[i] = ya[i];
      }

    //  This is the initial approximation to y.
    y = ya[ns--];

    //  For each column of the tableau, loop over the current c's and d's and update them.
    for (int m = 0; m < n - 1; ++m)
      {
        for (int i = 0; i < n - m; ++i)
          {
            auto ho = xa[i] - x;
            auto hp = xa[i + m] - x;
            auto w = c[i + 1] - d[i];
            //  This error can occur if two input xa's are identical within roundoff error.
            auto den = ho - hp;
            if (den == 0)
              throw std::logic_error("error in polynomial_interp");
            den = w / den;
            //  Here, the c's and d's are updated.
            d[i] = hp * den;
            c[i] = ho * den;
        }
        y += (dy = (2 * ns < (n - m) ? c[ns + 1] : d[ns--]));
      }

    return;
  }



/**
 *    Given arrays xa[1..n] and ya[1..n], and given a value x, this routine returns
 *    a value of y and an accuracy estimate dy.  The value returned is that of the 
 *    diagonal rational function, evaluated at x, which passes through the
 *    n points (xa[i], ya[i]), i = 1..n.
 */
template<typename RealTp>
  void
  rational_interp(RealTp *xa, RealTp *ya, int n, RealTp x, RealTp & y, RealTp & dy)
  {

    int ns = 1;
    const RealTp TINY = 1.0e-25;

    std::vector<RealTp> c(n);
    std::vector<RealTp> d(n);

    auto hh = std::fabs(x - xa[1]);
    for (i = 1; i <= n; ++i)
      {
        h = std::fabs(x - xa[i]);
        if (h == 0)
          {
            y = ya[i];
            dy = 0;
            return;
          }
        else if (h < hh)
          {
            ns = i;
            hh = h;
          }
        c[i] = ya[i];
        //  The tiny part is needed to prevent a rare zero over zero condition.
        d[i] = ya[i] + TINY;
      }
    y = ya[ns--];
    for (int m = 1; m < n; ++m)
      {
        for (int i = 1; i <= n - m; ++i)
          {
            auto w = c[i + 1] - d[i];
            auto h = xa[i + m] - x;
            auto t = (xa[i] - x) * d[i] / h;
            auto dd = t - c[i + 1];
            //  This error condition indicated that the interpolating function has a pole 
            //  at the requested value of x.
            if (dd == 0)
              throw std::logic_error("error in rational_interp");
            dd = w / dd;
            d[i] = c[i + 1] * dd;
            c[i] = t * dd;
          }
        y += (dy = (2 * ns < (n - m) ? c[ns + 1] : d[ns--]));
      }

    return;
}


/**
 *  Given arrays x[1..n] and y[1..n] containing a tabulated function, i.e. y[i] = f(x[i]), with monatonically increasing
 *  values of x[i], and given the values yp1, and ypn for the first derivatives of the function at points 1 and n 
 *  respectively, this routine returns the an array ypp[1..n] that contains the second derivatives of the interpolating 
 *  function at the tabulated points x[i].  If yp1 and/or ypn are equal to 1.0e30 or larger, the routine is signalled to 
 *  set the corresponding boundary condition for a natural spline, with zero second derivative at that boundary.
 */
template<typename RealTp>
  void
  cubic_spline(RealTp *x, RealTp *y, int n,
         std::experimental::optional<RealTp> yp1,
         std::experimental::optional<RealTp> ypn, RealTp *ypp)
  {
    std::vector<RealTp> u(n - 2);

    //  The lower boundary condition is set to either the natural one or
    //  to match a specified first derivative.
    if (yp1)
      ypp[0] = u[0] = 0;
    else
      {
        ypp[0] = -0.5;
        u[0] = (3 / (x[1] - x[0])) * ((y[1] - y[0]) / (x[1] - x[0]) - yp1);
      }

    //  Decomposition of the tridiagonal system.
    for (int i = 1; i < n - 1; ++i)
      {
        auto sigma = (x[i] - x[i - 1]) / (x[i + 1] - x[i - 1]);
        auto p = sigma * ypp[i - 1] + 2;
        ypp[i] = (sigma - 1) / p;
        u[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]) - (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
        u[i] = (6 * u[i] / (x[i + 1] - x[i - 1]) - sigma * u[i - 1]) / p;
      }

    //  The upper boundary condition is set to either the natural one or
    //  to match a specified first derivative.
    RealTp qn, un;
    if (ypn)
      ypp[n - 1] = 0;
    else
      {
        auto qn = 0.5;
        auto un = (3 / (x[n - 1] - x[n - 2]))
                * (ypn - (y[n - 1] - y[n - 2]) / (x[n - 1] - x[n - 2]));
        ypp[n - 1] = (un - qn * u[n - 2]) / (qn * ypp[n - 2] + 1);
      }

    //  This is the backsubstitution loop of the tridiagonal algorithm.
    for (int j = n - 2; j >= 0; --j)
      ypp[j] = ypp[j] * ypp[j + 1] + u[j];
  }


/**
 *  Given the arrays xa[1..na], ya[1..na] which tabulate a function (with the x[i]'s in order), and given the array 
 *  yapp[1..na] which is the output from cubic_spline above, and given a value of x, this routine returns a cubic-spline 
 *  interpolated value y.
 */
template<typename RealTp>
  RealTp
  cubic_spline_interp(RealTp *xa, RealTp *ya, RealTp *yapp, int na, RealTp x)
  {
    //  Store the last values of klo and khi to see if they still work on subsequent calls.
    //  Also store the last number of points to watch out for new spline data.
    //  If the old points don't work reset them to the endpoints.
    //  In either case, refine klo and khi with bisection.
    static int naold = 0;
    static int kloold;
    static int khiold;

    //  If the number of points has changed then spline was rerun or a new set of points
    //  is being provided.  To prevent the arrays being indexed to an illegal value and
    // to reflect the arrival of new data reset the values of kloold and khiold and naold.
    if (naold != na)
      {
        naold = na;
        kloold = 1;
        khiold = na;
      }

    int klo = (x > xa[kloold] ? kloold : 1);
    int khi = (x < xa[khiold] ? khiold : na);
    while (khi - klo > 1)
      {

        auto k = (khi + klo) >> 1;
        if (xa[k] > x)
          khi = k;
        else
          klo = k;
      }
    kloold = klo;
    khiold = khi;

    auto h = xa[khi] - xa[klo];
    if (h == 0)
      throw std::logic_error("bad xa input to cubic_spline_interp");
    auto a = (xa[khi] - x) / h;
    auto b = (x - xa[klo]) / h;
    return a * ya[klo]
         + b * ya[khi]
         + (a * (a * a - 1) * yapp[klo]
          + b * (b * b - 1) * yapp[khi]) * (h * h) / 6;
  }




/**
 *  Given the arrays xa[1..na], ya[1..na] which tabulate a function (with the xa's in order), 
 *  and given a value of x, this routine returns a simple linear interpolated value y.
 */
template<typename RealTp>
  RealTp
  linear_interp(RealTp *xa, RealTp *ya, int na, RealTp x)
  {
    int k, klo, khi;
    RealTp h, b, a;

    //  Store the last values of klo and khi to see if they still work.
    //  Also store the last number of points to watch out for new spline data.
    //  If the old points don't work reset them to the endpoints.
    //  In either case, refine klo and khi with bisection.
    static int naold = 0;
    static int kloold;
    static int khiold;

    //  If the number of points has changed then spline was rerun or a new set of points
    //  is being provided.  To prevent the arrays being indexed to an illegal value and
    //  to reflect the arrival of new data reset the values of kloold and khiold and naold.
    if (naold != na)
      {
        naold = na;
        kloold = 1;
        khiold = na;
      }

    klo = (x > xa[kloold] ? kloold : 1);
    khi = (x < xa[khiold] ? khiold : na);
    while (khi - klo > 1)
      {

        k = (khi + klo) >> 1;
        if (xa[k] > x)
          khi = k;
        else
          klo = k;
      }
    kloold = klo;
    khiold = khi;

    h = xa[khi] - xa[klo];

    if (h == 0)
      throw std::logic_error("bad xa input to linear_interp");

    a = (xa[khi] - x) / h;
    b = (x - xa[klo]) / h;
    return a * ya[klo] + b * ya[khi];
  }


#endif // INTERPOLATION_TCC
