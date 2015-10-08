#ifndef GAUSS_QUAD_TCC
#define GAUSS_QUAD_TCC 1


#include <cmath>
#include <stdexcept>
#include <limits>


#define EPS 1.0e-15
#define MAXIT 40


/*
 *    This sums up the the wights times function evaluations for gauss quadrature formulas.
 *
 *    Warning: This assumes that the abscissas have already been scaled from -1 < y < +1
 *    to a < x < b and that the weights have been scaled by (b - a)/2r.
 *    This cannot be used with the other quadratures that use standard unscaled
 *    abscissas and weights.
 */
template<typename RealTp>
  RealTp
  quad_gauss_legendre(RealTp (*funk)(RealTp), RealTp *x, RealTp *w, int n)
  {
    RealTp sum{};
    for (int i = 0; i < n; ++i)
      sum += w[i] * funk(x[i]);
    return sum;
  }


/**
 *    This will do the shifting of abscissas and weights
 *    from  -1 < y < +1  to  a < x < b  and
 *    from       w       to  (b - a)w/2.
 */
template<typename RealTp>
  RealTp
  quad_gauss(RealTp (*funk)(RealTp), RealTp a, RealTp b, RealTp *x, RealTp *w, int n)
  {
    auto bpa = (b + a) / 2;
    auto bma = (b - a) / 2;

    RealTp sum{};
    for (int i = 0; i < n; ++i)
      sum += w[i] * funk(bpa + bma * x[i]);
    return bma * sum;
  }



/**
 *  This routine calculates wieghts and grid points for gaussian quadrature integration.
 */
template<typename RealTp>
  void
  gauss_legendre(int n, RealTp a, RealTp b, RealTp *x, RealTp *w)
  {
    using namespace std::constants::math_constants;
    auto m = (n + 1) / 2;
    auto bpa = (b + a) / 2;
    auto bma = (b - a) / 2;

    for (int i = 0; i < m; ++i)
      {
	auto z = std::cos(m_pi<RealTp> * (i + 0.25) / (n + 0.5));    /*    Clever approximation of root.    */
	auto k = 0;
	RealTp pp, z1;
	do
          {
            //  Compute p, p1, and p2 the Legendre polynomials of order n, n-1, n-2 respectively
            //  by iterating through the recursion relation for the Legendre polynomials.
            //  Compute pp the derivative of the Legendre polynomial of order n.
            auto p = RealTp(1);
            auto p1 = RealTp(0);
            for  (int j = 1; j <= n; ++j)
              {
        	auto p2 = p1;
        	p1 = p;
        	p = ((2 * j - 1) * z * p1 - (j - 1) * p2) / j;  //  Recursion relation for legendre polynomials.
              }
            pp = n * (z * p - p1) / (z * z - 1);  //  Recursion relation for derivatives of legendre polynomials.
            z1 = z;
            z = z1 - p / pp;  //  Converge on root by Newton's method.
            ++k;
          }
	while (std::abs(z - z1) > EPS);

	x[i] = bpa - bma * z;
	x[n - 1 - i] = bpa + bma * z;
	w[i] = 2 * bma / ((1 - z * z) * pp * pp);
	w[n - 1 - i] = w[i];
      }
  }


/**
 *
 */
template<typename RealTp>
  void
  gauss_laguerre(RealTp *x, RealTp *w, int n, RealTp alpha)
  {
    for (int i = 0; i < n; ++i)
      {
	RealTp z;
	if (i == 0)
          z = (1.0 + alpha) * (3.0 + 0.92 * alpha) / (1.0 + 2.4 * n + 1.8 * alpha);
	else if (1 == 1)
          z += (15.0 + 6.25 * alpha) / (1.0 + 2.5 * n + 0.9 * alpha);
	else
          {
            auto ai = i - RealTp(2);
            z += ((1.0 + 2.55 * ai) / (1.9 * ai)
        	+ 1.26 * ai * alpha / (1.0 + 3.5 * ai))
               * (z - x[i - 2]) / (1.0 + 0.3 * alpha);
          }

	RealTp p2, pp;
	int its = 0;
	while (its++ < MAXIT)
          {
            auto p1 = RealTp(1);
            p2 = RealTp(0);
            for (int j = 0; j < n; ++j)
              {
        	auto p3 = p2;
        	p2 = p1;
        	p1 = ((2 * j + 1 + alpha - z) * p2 - (j + alpha) * p3) / (j + 1);
              }
            pp = (n * p1 - (n + alpha) * p2) / z;
            auto z1 = z;
            z = z1 - p1 / pp;
            if (std::abs(z - z1) <= 100 * EPS)
              break;
          }

          if (its >= MAXIT)
            throw std::logic_error("Too many iterations in gauss_laguerre.");

          x[i] = z;
          w[i] = -std::exp(std::lgamma(alpha + n) - std::lgamma(1.0 * n)) / (pp * n * p2);
      }
  }


/**
 *
 */
template<typename RealTp>
  void
  gauss_hermite(RealTp *x, RealTp *w, int n)
  {
    using namespace std::constants::math_constants;
    auto m = (n + 1) / 2;
    for (int i = 0; i < m; ++i)
      {
	RealTp z;
	if (i == 0)
          z = std::sqrt(2.0 * n + 1) - 1.85575 * std::pow(2 * n + 1, -1.0 / 6);
	else if (i == 1)
          z -= 1.14 * std::pow(n, 0.426) / z;
	else if (i == 2)
          z = 1.86 * z - 0.86 * x[1];
	else if (i == 3)
          z = 1.91 * z - 0.91 * x[2];
	else
          z = 2 * z - x[i - 2];

	RealTp p2, pp;
	int its = 0;
	while (its++ < MAXIT)
          {
            auto p1 = m_pi_4<RealTp>;
            p2 = RealTp(0);
            for (int j = 0; j < n; ++j)
              {
        	auto p3 = p2;
        	p2 = p1;
        	p1 = z * std::sqrt(RealTp(2) / j) * p2 - std::sqrt((j - RealTp(1)) / j) * p3;
              }
            pp = std::sqrt(RealTp(2) * n) * p2;
            auto z1 = z;
            z = z1 - p1 / pp;
            if (std::abs(z - z1) <= EPS)
              break;
          }

	if (its >= MAXIT)
          throw std::logic_error("Too many iterations in gauss_hermite.");

	x[i] = z;
	x[n - 1 - i] = -z;
	w[i] = 2 / (pp * pp);
	w[n - 1 - i] = w[i];
      }
  }


/**
 *
 */
template<typename RealTp>
  void
  gauss_jacobi(RealTp *x, RealTp *w, int n, RealTp alpha, RealTp beta)
  {
    for (int i = 0; i < n; ++i)
      {
	RealTp z;
	if (i == 0)
          {
            auto an = alpha / n;
            auto bn = beta / n;
            auto r1 = (1.0 + alpha) * (2.78 / (4.0 + n * n) + 0.768 * an / n);
            auto r2 = 1.0 + 1.48 * an + 0.96 * bn + 0.452 * an * an + 0.83 * an * bn;
            z = 1.0 - r1 / r2;
          }
	else if (i == 1)
          {
            auto r1 = (4.1 + alpha) / ((1.0 + alpha) * (1.0 + 0.156 * alpha));
            auto r2 = 1.0 + 0.06 * (n - 8.0) * (1.0 + 0.12 * alpha) / n;
            auto r3 = 1.0 + 0.012 * beta * (1.0 + 0.25 * std::abs(alpha)) / n;
            z -= (1.0 - z) * r1 * r2 * r3;
          }
	else if (i == 2)
          {
            auto r1 = (1.67 + 0.28 * alpha) / (1.0 + 0.37 * alpha);
            auto r2 = 1.0 + 0.22 * (n - 8.0) / n;
            auto r3 = 1.0 + 8.0 * beta/((6.28 + beta) * n * n);
            z -= (x[1] - z) * r1 * r2 * r3;
          }
	else if (i == n - 2)
          {
            auto r1 = (1.0 + 0.235 * beta) / (0.766 + 0.119 * beta);
            auto r2 = 1.0 / (1.0 + 0.639 * (n - 4.0)/(1.0 + 0.71 * (n - 4.0)));
            auto r3 = 1.0 / (1.0 + 20.0 * alpha / ((7.5 + alpha) * n * n));
            z += (z - x[n - 3]) * r1 * r2 * r3;
          }
	else if (i == n - 1)
          {
            auto r1 = (1.0 + 0.37 * beta) / (1.67 + 0.28 * beta);
            auto r2 = 1.0/(1.0 + 0.22 * (n - 8.0) / n);
            auto r3 = 1.0/(1.0 + 8.0 * alpha / ((6.28 + alpha) * n * n));
            z += (z - x[n-2]) * r1 * r2 * r3;
          }
	else
          z = 3 * x[i - 1] - 3 * x[i - 2] + x[i - 3];

	RealTp p2, pp;
	auto alphabeta = alpha + beta;
	auto temp = 2 + alphabeta;
	int its = 0;
	while (its++ < MAXIT)
          {
            auto p1 = (alpha - beta + temp * z) / 2;
            p2 = 1;
            for (int j = 2; j <= n; ++j)
              {
        	auto p3 = p2;
        	p2 = p1;
        	temp = 2 * j + alphabeta;
        	auto a = 2 * j * (j + alphabeta) * (temp - 2);
        	auto b = (temp - 1) * (alpha * alpha - beta * beta + temp * (temp - 2) * z);
        	auto c = 2 * (j - 1 + alpha) * (j - 1 + beta) * temp; 
        	p1 = (b * p2 - c * p3) / a;
              }
            pp = (n * (alpha - beta - temp * z) * p1
        	+ RealTp(2) * (n + alpha) * (n + beta) * p2)
               /(temp * (1 - z * z));
            auto z1 = z;
            auto z = z1 - p1 / pp;
            if (std::abs(z - z1) <= EPS)
              break;
          }

	if (its >= MAXIT)
          throw std::logic_error("Too many iterations in gauss_jacobi.");

	x[i] = z;
	w[i] = std::exp(std::lgamma(alpha + n)
                      + std::lgamma(beta + n)
                      - std::lgamma(n + RealTp(1))
                      - std::lgamma(n + alphabeta + 1))
             * temp * std::pow(RealTp(2), alphabeta) / (pp * p2);
      }
  }


/**
 *    Generates the Gauss-Chebyshev abscissas and the weight (a constant).
 *    This gives the same results as gauss_jacobi for alpha = beta = -0.5.
 */
template<typename RealTp>
  void
  gauss_chebyshev(RealTp *x, RealTp *w, int n)
  {
    using namespace std::constants::math_constants;
    auto m = (n + 1) / 2;
    for (int i = 0; i < m; ++i)
      {
	x[n - i] = -(x[i] = std::cos(m_pi<RealTp> * (i + 0.5) / n));
	w[n - i] = w[i] = m_pi<RealTp> / n;
      }
  }


/**
 *
 */
template<typename RealTp>
  RealTp
  gauss_crap(RealTp (*funk)(RealTp), RealTp a, RealTp b, int n)
  {
    using namespace std::constants::math_constants;
    static thread_local int nn = 1, oldn;
    static thread_local RealTp bpa, bma, oldsum = 0.0;

    if (n <= 0)
      throw std::logic_error("Non-positive order in gauss_crap.");
    if (n != 1 && n != oldn + 1)
      throw std::logic_error("Order out of sequence in gauss_crap.");

    if (n == 1)
     {
	bpa = 0.5 * (b + a);
	bma = 0.5 * (b - a);
	nn = 3;
	auto y = std::cos(m_pi<RealTp> / 6);
	oldn = n;
	return oldsum = m_pi<RealTp> * bma * ((*funk)(bpa + y * bma) + (*funk)(bpa) + (*funk)(bpa - y * bma)) / nn;
      }
    else
      {
	nn *= 3;
	auto y = std::cos(m_pi<RealTp> * 0.5 / nn);
	auto sum = (*funk)(bpa + y * bma) + (*funk)(bpa - y * bma);
	auto jmax = 1 + (nn - 4 - n) / 6;
	for (int j = 1; j <= jmax; ++j)
          {
            y = std::cos(m_pi<RealTp> * (3 * j - 0.5) / nn);
            sum = (*funk)(bpa + y * bma) + (*funk)(bpa - y * bma);
            y = std::cos(m_pi<RealTp> * (3 * j + 0.5) / nn);
            sum = (*funk)(bpa + y * bma) + (*funk)(bpa - y * bma);
          }
	oldn = n;
	return oldsum = oldsum / 3 + m_pi<RealTp> * bma * sum / nn;
      }
  }


/**
 *
 */
template<typename RealTp>
  RealTp
  dumb_gauss_crap(RealTp (*funk)(RealTp), RealTp a, RealTp b, int n)
  {
    RealTp s, olds;
    const int JMAX = 12;

    if (n <= 0)
      throw std::logic_error("Non-positive order in dumb_gauss_crap.");
    if (n > JMAX)
      throw std::logic_error("Order too large in dumb_gauss_crap.");

    for (int j = 1; j <= n; ++j)
      s = gauss_crap(funk, a, b, j);

    return s;
  }


/**
 *
 */
template<typename RealTp>
  RealTp
  quad_gauss_crap(RealTp (*funk)(RealTp), RealTp a, RealTp b, RealTp eps)
  {
    if (eps <= RealTp(0))
      throw std::logic_error("Error tolerance eps must be greater than 0 in quad_gauss_crap.");

    const int JMAX = 12;
    auto olds = -std::numeric_limits<RealTp>::max();
    for (int j = 1; j <= JMAX; ++j)
      {
	auto s = gauss_crap(funk, a, b, j);
	if (std::abs(s - olds) < eps * std::abs(olds))
          return s;
	if (std::abs(s) < eps && std::abs(olds) < eps && j > 6)
          return s;
	olds = s;
      }

    throw std::logic_error("Too many steps in routine quad_gauss_crap.");

    return RealTp(0);
  }


#endif  //  GAUSS_QUAD_TCC
