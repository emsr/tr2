#ifndef ODE_TCC
#define ODE_TCC 1


#include <cmath>
#include <vector>


/**
 *    Given values for n dependent variables y and their derivatives dydx known at x,
 *    use fourth-order Runge-Kutta method to advance the solution over an interval h and returns
 *    the incremented variables as yout, (which need not be a distinct array from y).
 */
template<typename StateVec, typename RealTp>
  StateVec
  runge_kutta_4(const StateVec & y, const StateVec & dydx,
        	RealTp x, RealTp h)
  {
    auto n = std:distance(y_beg, y_end);
    std::vector<RealTp> dym(n);
    std::vector<RealTp> dyt(n);
    std::vector<RealTp> yt(n);

    RealTp h2 = h / 2;
    RealTp h6 = h / 6;
    RealTp xh = x + h2;

    yt = y + h2 * dydx;
    dyt = yt.deriv(xh);

    yt = y + h2 * dyt;
    dym = yt.deriv(xh);

    yt = y + h * dym;
    dym += dyt;

    dyt = yt.deriv(x + h);

    return y + h6 * (dydx + dyt + 2 * dym);
  }


/**
 *  Starting from initial state vector y1 known at x1,
 *  use 4th order Runge-Kutta to advance nstep equal increments to x2.
 *  The user supplied routine deriv supplies derivatives.
 *  The results are stored in the output iterators x_tab, y_tab
 *  which must either be perallocated for nstep + 1 values or be accessed
 *  through a back inserter.
 */
template<typename StateVec, typename RealTp>
  void
  dumb_runge_kutta(const StateVec & y1, RealTp x1, RealTp x2, int nstep,
                   RealTpOutIter x_tab, StateVecOutIter y_tab)
  {
    //  Load starting values of dependant variables.
    StateVec y = y1;
    y_tab[0] = y;
    x_tab[0] = x1;
    auto x = x1;
    auto h = (x2 - x1) / nstep;

    //  Take nstep steps in the independant variable x.
    for (int k = 1; k <= nstep; ++k)
      {
	yout = runge_kutta_4(y, y.deriv(x), x, h);
	if (x + h == x)
          throw std::logic_error("step size too small in dumb_runge_kutta");
	x += h;
	//  Store intermediate results.
	x_tab[k] = x;
	y = yout;
	y_tab[k] = y;
      }
  }




/**
 *    Fifth-order Runge-Kutta step with monitoring of local truncation error to ensure
 *    accuracy and adjust stepsize.  Input are the dependent variable state vector y
 *    and its derivative dydx at the starting value of the independent variable x.
 *    Also input are the first guess for the stepsize h_try, the requred accuracy eps, and the
 *    vector yscale[1..n] against which the error is scaled independently for each dependent variable.
 *    On output, x and y are replaced by thier new values, h_final is the stepsize which was actually
 *    accomplished, and h_next is the estimated next stepsize.
 */
template<typename StateVec, typename RealTp>
void
  quad_runge_kutta(StateVec & y, StateVec & dydx, RealTp & x, RealTp h_try,
                   double eps, StateVec & yscale, RealTp & h_final, RealTp & h_next)
  {
    constexpr double POW_GROW = 10000;
    constexpr double POW_SHRINK = 1.0e-30;
    constexpr double F_CORR = 1.0 / 15.0;
    constexpr double F_SAFETY = 0.9;
    constexpr double ERR_COND = std::pow((4.0 / F_SAFETY), (1.0 / POW_GROW));

    auto dysav = dydx;
    auto ysav = y;
    auto xsav = x;

    //  Set stepsize to the initial trial value.
    auto h = h_try;
    while (true)
      {
	//  Take two half steps.
	auto h2 = h / 2;
	auto ytemp = runge_kutta_4(ysav, dysav, xsav, h2);
	x = xsav + h2;
	dydx = ytemp.deriv(x);
	y = runge_kutta_4(ytemp, dydx, x, h2);
	x = xsav + h;
	if (x == xsav)
          throw std::logic_error("step size too small in quad_runge_kutta");

	//  Take the large step.
	ytemp = runge_kutta_4(ysav, dysav, xsav, h);

	//  Evaluate accuracy.  Put the error estimate into ytemp.
	ytemp = y - ytemp;
	auto errmax = max(abs(ytemp[i] / yscale[i]));

	//  Scale relative to required tolerance.
	errmax /= eps;
	if (errmax <= 1.0)
          {
            //  Step succeeded.  Compute size of next step.
            h_final = h;
            h_next = (errmax > ERR_COND ? F_SAFETY * std::exp(POW_GROW * std::log(errmax)) : 4 * h);
            break;
          }
	//  Truncation error too large, reduce stepsize.
	h = F_SAFETY * h * std::exp(POW_SHRINK * std::log(errmax));
      }
    //  Mop up fifth order truncation error.
    y += F_CORR * ytemp;
  }



/**
 * Cash-Carp Runge-Kutta algorithm.
 */
template<typename StateVec, typename RealTp>
  void
  cash_karp_rk(StateVec & y, StateVec & dydx,
               RealTp x, RealTp h, StateVec & yout, StateVec & yerr)
  {
    static constexpr RealTp
      a2 = 0.2, a3 = 0.3, a4 = 0.6, a5 = 1.0, a6 = 0.875,
      b21 = 0.2,
      b31 = 3.0/40.0, b32 = 9.0/40.0,
      b41 = 0.3, b42 = -0.9, b43 = 1.2,
      b51 = -11.0/54.0, b52 = 2.5, b53 = -70.0/27.0, b54 = 35.0/27.0,
      b61 = 1631.0/55296.0, b62 = 175.0/512.0, b63 = 575.0/13824.0, b64 = 44275.0/110592.0, b65 = 253.0/4096.0,
      c1 = 37.0/378.0, c3 = 250.0/621.0, c4 = 125.0/594.0, c6 = 512.0/1771.0,
      dc5 = -277.0/14336.0;
    static constexpr RealTp dc1 = c1 - 2825.0/27648.0, dc3 = c3 - 18575.0/48384.0,
                            dc4 = c4 - 13525.0/55296.0, dc6 = c6 - 0.25;

    ytemp = y + h * b21 * dydx;

    ak2 = ytemp.deriv(x + a2 * h);
    ytemp = y + h * (b31 * dydx + b32 * ak2);

    ak3 = ytemp.deriv(x + a3 * h);
    ytemp = y + h * (b41 * dydx + b42 * ak2 + b43 * ak3);

    ak4 = ytemp.deriv(x + a4 * h);
    ytemp = y + h * (b51 * dydx + b52 * ak2 + b53 * ak3 + b54 * ak4);

    ak5 = ytemp.deriv(x + a5 * h);
    ytemp = y + h * (b61 * dydx + b62 * ak2 + b63 * ak3 + b64 * ak4 + b65 * ak5);

    ak6 = ytemp.deriv(x + a6 * h);
    fyout = y + h * (c1 * dydx + c3 * ak3 + c4 * ak4 + c6 * ak6);

    yerr = h * (dc1 * dydx + dc3 * ak3 + dc4 * ak4 + dc5 * ak5 + dc6 * ak6);
  }


/**
 *    Fifth-order Runge-Kutta step with monitoring of local truncation error to ensure
 *    accuracy and adjust stepsize.  Input are the dependent variable vector y
 *    and its derivative dydx at the starting value of the independent variable x.
 *    Also input are the first guess for the stepsize h_try, the requred accuracy eps, and the
 *    vector yscale against which the error is scaled independently for each dependent variable.
 *    On output, x and y are replaced by thier new values, h_final is the stepsize which was actually
 *    accomplished, and h_next is the estimated next stepsize.
 */
template<typename StateVec, typename RealTp>
void
  quad_cash_karp_rk(StateVec & y, StateVec & dydx, RealTp & x, RealTp h_try,
                    RealTp eps, StateVec & yscale, RealTp & h_final, RealTp & h_next)
  {
    constexpr double POW_GROW = -0.20;
    constexpr double POW_SHRINK = -0.25;
    constexpr double F_SAFETY = 0.9;
    constexpr double ERR_COND = std::pow(5 / F_SAFETY, 1 / POW_GROW);

    auto h = h_try;
    while (true)
      {
	cash_karp_rk(y, dydx, x, h, ytemp, yerr);
	auto errmax = std::max(std::abs(yerr[i] / yscale[i]));
	errmax /= eps;
	if (errmax <= 1.0)
          break;
	auto h_temp = F_SAFETY * h * std::pow(errmax, POW_SHRINK);
	h = (h > 0 ? std::max(h_temp, 0.1 * h) : std::min(h_temp, 0.1 * h));
	auto x_new= x + h;
	if (x_new == x)
          throw std::logic_error("Stepsize underflow in quad_cash_karp_rk");
      }

    if (errmax > ERR_COND)
      h_next = F_SAFETY * h * std::pow(errmax, POW_GROW);
    else
      h_next = 5 * h;
    x += h_final = h;
    y = ytemp;
  }


/**
 *    Global variables for the storage of intermediate results from ode_integrate.
 */
int ode_max = 0;
int ode_count = 0;
double *ode_xp = 0;
double **ode_yp = 0;
double ode_dxsave = 0;


/**
 *    ODE driver with adaptive stepsize control.  Integrate starting with values
 *    y1[1..nvar] from x1 to x2 with accuracy eps, storing intermediate results
 *    in global variables ode_xp, ode_yp, ode_max, ode_count, ode_dxsave.  If ode_max == 0 no intermediate results
 *    will be stored and the pointers ode_xp and ode_yp can be set to zero.
 *    h1 should be set as a first guess initial stepsize, hmin is the minimum stepsize (can be zero).
 *    On output nok and nbad are the numbers of good and bad (but retried and fixed) steps taken.
 *    y1 is replaced by stepped values at the end of the integration interval.
 *    stepper is the name of the integration stepper to be used (e.g. quad_runge_kutta or bulirsch_stoer).
 */
template<typename StateVec, typename RealTp>
  void
  ode_integrate(double *y1, RealTp x1, RealTp x2,
        	RealTp eps, RealTp h1, RealTp hmin,
        	int *nok, int *nbad,
        	void (*stepper)(StateVec &, StateVec &, RealTp &, RealTp,
                        	RealTp, RealTp &, RealTp &, RealTp &))
  {
    double xsave, h_next, h_final;

    constexpr int MAXSTEP = 10000;
    constexpr double TINY = 1.0e-30;

    std::vector<double> yscale(nvar);
    std::vector<double> y(nvar);
    std::vector<double> dydx(nvar);

    auto x = x1;
    auto h = (x2 > x1) ? std::fabs(h1) : -std::fabs(h1);
    nok = nbad = ode_count = 0;
    y = y1;
    if (ode_max > 0)
      xsave = x - 2 * ode_dxsave;
    for (int nstep = 0; nstep < MAXSTEP; ++nstep)
     {
	dydx = y.deriv(x);
	yscale = std::abs(y) + std::abs(dydx) + TINY;
	if (ode_max)
          if (std::abs(x - xsave) > std::abs(ode_dxsave))
            if (ode_count < ode_max - 1)
              {
        	ode_xp[++ode_count] = x;
        	ode_yp][ode_count] = y;
        	xsave = x;
              }
	if ((x + h - x2) * (x + h - x1) > 0.0)
          h = x2 - x;
	stepper(y, dydx, x, h, eps, yscale, h_final, h_next);
	if (h_final == h)
          ++(*nok);
	else
          ++(*nbad);
	if ((x - x2) * (x2 - x1) >= 0.0)
          {
            y1 = y; 
            if (ode_max)
              {
        	ode_xp[++ode_count] = x;
        	ode_yp[ode_count] = y;
              }
            return;
          }
	if (std::fabs(h_next) <= hmin)
          throw std::logic_error("Step size to small in ode_integrate");
	h = h_next;
    }
    throw std::logic_error("Too many steps in ode_integrate");
  }


/**
 *   Modified midpoint step.  At xs, input the dependent variable vector y,
 *   and its derivative dydx.  Also input is htot, the total step to be made,
 *   and nstep, the number of interior steps to be used.  The output is returned as 
 *   yout, which need not be distinct from y[1..nvar]; if it is distinct
 *   however, then y and dydx will be returned undamaged.  Derivs is the user-supplied
 *   routine for calculating the right-hand side derivative.
 */
template<typename StateVec, typename RealTp>
  void
  modified_midpoint(double *y, double *dydx, double xs,
                    double htot, int nstep, double *yout)
  {
    auto h = htot / nstep;
    auto ym = y;
    auto yn = y + h * dydx;
    auto x = xs + h;
    auto yout = yn.deriv(x);
    auto h2 = 2.0 * h;
    for (int n = 1; n < nstep; ++n)
      {
	auto swap = ym + h2 * yout;
	ym = yn;
	yn = swap;
	x += h;
	yout = yn.deriv(x);
      }
    yout = (ym + yn + h * yout) / 2;

    return yout;
  }


/**
 *   Stoermer's rule for integrating second order conservative systems of the form
 *   y'' = f(x,y) for a system of n = nv/2 equations.  On input y[1..nv] contains
 *   y in the first n elements and y' in the second n elements all evaluated at xs.
 *   d2y[1..nv] contains the right hand side function f (also evaluated at xs) in
 *   its first n elements (the second n elements are not referenced).  Also input
 *   is htot, the total step to be taken and nstep, the number of substeps to be used.
 *   The output is returned as yout[1..nv], with the same storage arrangement as y.
 *   derivvs is the user-supplied routine that calculates f.
 *
 *   This routine can replace modified_midpoint above.
 */
void
stoermer(double *y, double *d2y, int nv, double xs,
         double htot, int nstep, double *yout)
{
  std::vector<double> ytemp(nv);

  auto h = htot / nstep;
  auto hh = h / 2;
  auto neqns = nv / 2;
  for (int i = 0; i < neqns; ++i)
    {
      n = neqns + i;
      ytemp[i] = y[i] + (ytemp[n] = h * (y[n] + hh * d2y[i]));
    }
  auto x = xs + h;
  yout = ytemp.deriv(x);
  auto h2 = 2 * h;
  for (int nn = 1; nn < nstep; ++nn)
    {
      for (int i = 0; i < neqns; ++i)
        ytemp[i] += (ytemp[(n = neqns + i)] += h2 * yout[i]);
      x += h;
      yout = ytemp.deriv(x);
    }
  for (int i = 0; i < neqns; ++i)
    { 
      auto n = neqns + i;
      yout[n] = ytemp[n] / h + hh * yout[i];
      yout[i] = ytemp[i];
    }
}


/*
 *    Global variables for the storage of extrapolation results for bulirsch_stoer.
 */
static double *bs_xx, **bs_yy;

static void bs_rat_extrap(int iest, double xest, double *yest,
                          double *yz, double *dy, int nv);

static void bs_poly_extrap(int iest, double xest, double *yest,
                           double *yz, double *dy, int nv);


constexpr int KMAXX = 9;
constexpr int IMAXX = KMAXX + 1;
constexpr double SAFE1 = 0.25;
constexpr double SAFE2 = 0.7;
constexpr double REDMAX = 1.0e-5;
constexpr double REDMIN = 0.7;
constexpr double TINY = 1.0e-30;
constexpr double SCALEMAX = 0.1;

/**
 *  Bulirsch-Stoer step with monitoring of local truncation error to ensure accuracy
 *  and adjust stepsize.  Input are the dependent variables y and the derivatives dydx
 *  at the starting value of the independent variable x.
 */
template<typename StateVec, typename RealTp>
  void
  bulirsch_stoer(const StateVec & y, const StateVec & dydx, RealTp & xx,
        	 RealTp h_try, RealTp eps, StateVec & yscale,
        	 RealTp & h_final, RealTp & h_next)
  {
    int kk, km;
    bool exitflag = false;
    double errmax, fact, red, scale, work, workmin, xest, *err, *yerr, *ysav, *yseq;

    static bool m_first = true;
    static int m_kmax;
    static int m_kopt;
    static int m_nseq[IMAXX + 1] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };

    static double m_epsold = -1.0;
    static double m_xnew;
    static double m_a[IMAXX + 1];
    static double m_alf[KMAXX + 1][KMAXX + 1];

    std::vector<double> bs_xx(KMAXX);
    std::vector<double> bs_yy = dmatrix(1, nv, 1, KMAXX);
    std::vector<double> err(KMAXX);
    std::vector<double> yerr(nv);
    std::vector<double> ysav(nv);
    std::vector<double> yseq(nv);

    if (eps != m_epsold)
      {
	h_next = m_xnew = -1.0e29;
	auto eps1 = SAFE1 * eps;
	m_a[0] = m_nseq[0] + 1;
	for (int k = 0; k < KMAXX; ++k)
          m_a[k + 1] = m_a[k] + m_nseq[k + 1];
	for (int iq = 1; iq < KMAXX; ++iq)
          for (int k = 1; k < iq; ++k) // Didn't touch!
            m_alf[k][iq] = std::pow(eps1, (m_a[k + 1] - m_a[iq + 1])
                                       / ((m_a[iq + 1] - m_a[1]) * (2 * k + 1)));
	m_epsold = eps;
	for (int m_kopt = 2; m_kopt < KMAXX; ++m_kopt)
          if (m_a[m_kopt + 1] > m_a[m_kopt] * m_alf[m_kopt - 1][m_kopt])
            break;
	m_kmax = m_kopt;
      }
    auto h = h_try;
    ysav = y;
    if (xx != m_xnew || h != h_next)
      {
	m_first = true;
	m_kopt = m_kmax;
      }
    bool reduct = false;
    while (true)
      {
	for (int k = 0; k < m_kmax; ++k)
          {
            m_xnew = xx + h;
            if (m_xnew == xx)
              throw std::logic_error("step size underflow in bulirsch_stoer");
            yseq = modified_midpoint(ysav, dydx, xx, h, m_nseq[k]);
            xest = dsqr(h / m_nseq[k]);
            bs_poly_extrap(k, xest, yseq, y, yerr);
            if (k != 0)
              {
        	errmax = TINY;
        	for (i = 0; i < nv; ++i)
                  errmax = std::max(errmax, std::fabs(yerr[i] / yscale[i]));
        	errmax /= eps;
        	auto km = kk - 1;
        	err[km] = std::pow(errmax / SAFE1, 1.0 / (2 * k + 1));
        	if (k >= m_kopt - 1 || m_first)
        	  {
        	    if (errmax < 1.0)
                      {
                	exitflag = true;
                	break;
                      }
        	    if (k == m_kmax || k == m_kopt + 1)
                      {
                	red = SAFE2 / err[km];
                	break;
                      }
        	    else if (k == m_kopt && m_alf[m_kopt - 1][m_kopt] < err[km])
                      {
                	red = 1.0 / err[km];
                	break;
                      }
        	    else if (k == m_kopt && m_alf[km][m_kmax - 1] < err[km])
                      {
                	red = m_alf[km][m_kopt - 1] / err[km];
                	break;
                      }
        	  }
              }
          }
	if (exitflag)
          break;
	red = std::min(red, REDMIN);
	red = std::max(red, REDMAX);
	h *= red;
	reduct = true;
      }
    xx = m_xnew;
    h_final = h;
    m_first = false;
    workmin = 1.0e35;
    for (int kk = 0; kk < km; ++kk)
      {
	fact = dmax(err[kk], SCALEMAX);
	work = fact * m_a[kk + 1];
	if (work < workmin)
          {
            scale = fact;
            workmin = work;
            m_kopt = kk + 1;
          }
      }
    h_next = h / scale;
    if (m_kopt >= k && m_kopt != m_kmax && !reduct)
      {
	fact = std::max(scale / alf[m_kopt - 1][m_kopt], SCALEMAX);
	if (fact * m_a[m_kopt + 1] <= workmin)
          {
            h_next = h / fact;
            ++m_kopt;
          }
      }
  }




/**
 *  Routine used by bulirsch_stoer to perform rational function extrapolation.
 */
template<typename StateVec, typename RealTp>
  static void
  bs_rat_extrap(int iest, double xest, double *yest, 
        	double *yz, double *dy)
  {
    std::vector<double> fx(iest);

    bs_xx[iest] = xest;
    if (iest == 0)
      dy = bs_yy[1] = yz = yest;
    else
      {
	//  Evaluate next diagonal in the tableau.
	for (int k = 0; k < iest - 1; ++k)
          fx[k + 1] = bs_xx[iest - k] / xest;
	for (int j = 0; j < nv; ++j)
          {
            double v = bs_yy[1][j];
            double yy, c;
            bs_yy[1] = yest;
            c = yy = yest[j];
            for (int k = 1; k < iest; ++k)
              {
        	double b1 = fx[k] * v;
        	double b = b1 - c;
        	//  Watch division by zero.
        	double ddy;
        	if (b)
        	  {
                    b = (c - v) / b;
                    ddy = c * b;
                    c = b1 * b;
        	  }
        	else
        	  ddy = v;
        	if (k != iest)
        	  v = bs_yy[k][j];
        	bs_yy[k][j] = ddy;
        	yy += ddy;
              }
            dy[j] = ddy;
            yz[j] = yy;
          }
      }
  }


/**
 *  Routine used by bulirsch_stoer to perform polynomial function extrapolation.
 */
template<typename StateVec, typename RealTp>
  static void
  bs_poly_extrap(int iest, double xest, double *yest, 
        	 double *yz, double *dy, int nv)
  {
    std::vector<double> c(nv);

    bs_xx[iest] = xest;
    dy = yz = yest;
    if (iest == 1)
      bs_yy[1][j] = yest[j];
    else
      {
	for (int j = 0; j < nv; ++j)
          c[j] = yest[j];
	for (int k = 0; k < nv; ++k)
          {
            auto delta = 1 / (bs_xx[iest-k] - xest);
            auto f1 = delta * xest;
            auto f2 = delta * bs_xx[iest - k];
            for (int j = 0; j < nv; ++j)
              {
        	auto q = bs_yy[k][j];
        	bs_yy[k][j] = dy[j];
        	delta = c[j] - q;
        	dy[j] = delta * f1;
        	c[j] = delta * f2;
        	yz[j] += dy[j];
              }
          }
	bs_yy[iest] = dy;
      }
  }


#endif  //  ODE_TCC
