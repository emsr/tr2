
// $HOME/bin/bin/g++ -o test_integration test_integration.cpp 


#include <cstdio>
#include <cmath>
#include <vector>
#define PI        3.1415926535897931


#include "integration.h"


double sin2(double x) { double s = std::sin(x); return s * s; }
double cos2(double x) { double c = std::cos(x); return c * c; }
double foo(double x) { return (1.0 - x) * std::exp(-x / 2.0); }
double foonum(double x) { return (1.0 - x); }
double funk1(double x) { return std::cos(x) / std::sqrt(x * (PI - x)); }
double funk1num(double x) { return cos(x); }
double funk2(double x) { return (2.0 + std::sin(x)) / std::sqrt(x * (PI - x)); }
double funk2num(double x) { return 2.0 + std::sin(x); }
double one(double x) { return 1.0; }
double ex(double x) { return x; }


main()
{
  double ans, exact, err;

  int m;

  double eps = 1.0e-6;
  double a = 0.0;
  double b = PI;

  int n_jac = 12;
  std::vector<double> x_jac(n_jac);
  std::vector<double> w_jac(n_jac);
  gauss_jacobi(x_jac, w_jac, n_jac, 1.5, -0.5);
  printf("\n\n  Abscissas and wights from gauss_jacobi with alpha = 1.5, beta = -0.5.\n");
  for (int i = 0; i < n_jac; ++i)
    printf("\n  x[%d] = %16.12f    w[%d] = %16.12f",
           i, x_jac[i], i, w_jac[i]);


  int n_leg = 12;
  std::vector<double> x_leg(n_leg);
  std::vector<double> w_leg(n_leg);
  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  printf("\n\n  Abscissas and wights from gauss_legendre.\n");
  for (int i = 0; i < n_leg; ++i)
    printf("\n  x[%d] = %16.12f    w[%d] = %16.12f",
           i, x_leg[i] / PIO2 - 1.0, i, w_leg[i] / PIO2);


  int n_jac2 = 12;
  std::vector<double> x_jac2(n_jac2);
  std::vector<double> w_jac2(n_jac2);
  gauss_jacobi(x_jac2, w_jac2, n_jac2, 0.0, 0.0);
  printf("\n\n  Abscissas and wights from gauss_jacobi with alpha = beta = 0.0.\n");
  for (int i = 0; i < n_jac2; ++i)
    printf("\n  x[%d] = %16.12f    w[%d] = %16.12f",
           i, x_jac2[i], i, w_jac2[i]);


  int n_cheb = 12;
  std::vector<double> x_cheb(n_cheb);
  std::vector<double> w_cheb(n_cheb);
  gauss_chebyshev(x_cheb, w_cheb, n_cheb);
  printf("\n\n  Abscissas and wights from gauss_chebyshev.\n");
  for (int i = 0; i < (n_cheb + 1) / 2; ++i)
    printf("\n  x[%d] = -x[%d] = %16.12f    w[%d] = w[%d] = %16.12f",
           i, n_cheb + 1 - i , x_cheb[i], i, n_cheb + 1 - i, w_cheb[i]);


  int n_jac3 = 12;
  std::vector<double> x_jac3(n_jac3);
  std::vector<double> w_jac3(n_jac3);
  gauss_jacobi(x_jac3, w_jac3, n_jac3, -0.5, -0.5);
  printf("\n\n  Abscissas and wights from gauss_jacobi with alpha = beta = -0.5.\n");
  for (int i = 0; i < n_jac3; ++i)
    printf("\n  x[%d] = %16.12f    w[%d] = %16.12f",
           i, x_jac3[i], i, w_jac3[i]);


  int n_herm = 12;
  std::vector<double> x_herm(n_herm);
  std::vector<double> w_herm(n_herm);
  gauss_hermite(x_herm, w_herm, n_herm);
  printf("\n\n  Abscissas and wights from gauss_hermite.\n");
  for (int i = 0; i < (n_herm + 1) / 2; ++i)
    printf("\n  x[%d] = -x[%d] = %16.12f    w[%d] = w[%d] = %16.12f",
           i, n_herm + 1 - i , x_herm[i], i, n_herm + 1 - i, w_herm[i]);


  n_lag = 12;
  std::vector<double> x_lag(n_lag);
  std::vector<double> w_lag(n_lag);
  gauss_laguerre(x_lag, w_lag, n_lag, 1.0);
  printf("\n\n  Abscissas and wights from gauss_laguerre.\n");
  for (int i = 0; i < n_lag; ++i)
    printf("\n  x[%d] = %16.12f    w[%d] = %16.12f",
           i , x_lag[i], i, w_lag[i]);



  m = 40;
  std::vector<double> c(m);
  std::vector<double> cint(m);
  std::vector<double> cder(m);


  printf("\n\n\n\n Test of integration routines...");
  printf("\n\n");
  printf("\n\n %-40s  %g", "Input requested error", eps);
  printf("\n\n %-40s  %d", "Input order of Gaussian quadrature", n_leg);
  printf("\n\n %-40s  %d", "Input order of Chebyshev fit", m);
  printf("\n\n");


  a = 0.0;
  b = PI;
  printf("\n\n Integrate cos(x) from  a = %f  to  b = %f . . .", a, PI);
  exact = 0.0;
  printf("\n %-40s    %16.12f", "Exact answer", exact);

  ans = quad_trapezoid(std::cos, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Trapezoid rule", ans, err);

  ans = quad_simpson(std::cos, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Simpson's rule", ans, err);

  ans = quad_romberg(std::cos, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Romberg integration", ans, err);

  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  ans = quad_gauss_legendre(std::cos, x_leg, w_leg, n_leg);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Legendre quadrature", ans, err);

  chebyshev_fit(a, b, c, m, cos);
  chebyshev_integ(a, b, c, cint, m);

  ans = chebyshev_eval(a, b, cint, m, b);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Chebyshev evaluation of integral", ans, err);

  ans = clenshaw_curtis_quad(a, b, c, m, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Clenshaw - Curtis quadrature", ans, err);



  a = 0.0;
  b = PI;
  printf("\n\n Integrate sin(x) from  a = %f  to  b = %f . . .", a, b);
  exact = 2.0;
  printf("\n %-40s    %16.12f", "Exact answer", exact);

  ans = quad_trapezoid(sin, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Trapezoid rule", ans, err);

  ans = quad_simpson(sin, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Simpson's rule", ans, err);

  ans = quad_romberg(sin, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Romberg integration", ans, err);

  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  ans = quad_gauss_legendre(sin, x_leg, w_leg, n_leg);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Legendre quadrature", ans, err);

  chebyshev_fit(a, b, c, m, sin);
  chebyshev_integ(a, b, c, cint, m);

  ans = chebyshev_eval(a, b, cint, m, b);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Chebyshev evaluation of integral", ans, err);

  ans = clenshaw_curtis_quad(a, b, c, m, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Clenshaw - Curtis quadrature", ans, err);



  a = 0.0;
  b = PI;
  printf("\n\n Integrate cos^2(x) from  a = %f  to  b = %f . . .", a, b);
  exact = PI/2.0;
  printf("\n %-40s    %16.12f", "Exact answer", exact);

  ans = quad_trapezoid(cos2, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Trapezoid rule", ans, err);

  ans = quad_simpson(cos2, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Simpson's rule", ans, err);

  ans = quad_romberg(cos2, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Romberg integration", ans, err);

  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  ans = quad_gauss_legendre(cos2, x_leg, w_leg, n_leg);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Legendre quadrature", ans, err);

  chebyshev_fit(a, b, c, m, cos2);
  chebyshev_integ(a, b, c, cint, m);

  ans = chebyshev_eval(a, b, cint, m, b);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Chebyshev evaluation of integral", ans, err);

  ans = clenshaw_curtis_quad(a, b, c, m, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Clenshaw - Curtis quadrature", ans, err);

  a = 0.0;
  b = PI;
  printf("\n\n Integrate sin^2(x) from  a = %f  to  b = %f . . .", a, b);
  exact = PI/2.0;
  printf("\n %-40s    %16.12f", "Exact answer", exact);

  ans = quad_trapezoid(sin2, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Trapezoid rule", ans, err);

  ans = quad_simpson(sin2, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Simpson's integral", ans, err);

  ans = quad_romberg(sin2, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Romberg integration", ans, err);

  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  ans = quad_gauss_legendre(sin2, x_leg, w_leg, n_leg);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Legendre quadrature", ans, err);

  chebyshev_fit(a, b, c, m, sin2);
  chebyshev_integ(a, b, c, cint, m);

  ans = chebyshev_eval(a, b, cint, m, b);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Chebyshev evaluation of integral", ans, err);

  ans = clenshaw_curtis_quad(a, b, c, m, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Clenshaw - Curtis quadrature", ans, err);



  a = 0.0;
  b = PI;
  printf("\n\n Integrate J_1(x) from  a = %f  to  b = %f . . .", a, b);
  exact = bessel_j0(0.0) - bessel_j0(PI);
  printf("\n %-40s    %16.12f", "Exact answer", exact);

  ans = quad_trapezoid(bessel_j1, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Trapezoid rule", ans, err);

  ans = quad_simpson(bessel_j1, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Simpson's integral", ans, err);

  ans = quad_romberg(bessel_j1, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Romberg integration", ans, err);

  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  ans = quad_gauss_legendre(bessel_j1, x_leg, w_leg, n_leg);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Legendre quadrature", ans, err);

  chebyshev_fit(a, b, c, m, bessel_j1);
  chebyshev_integ(a, b, c, cint, m);
  chebyshev_deriv(a, b, c, cder, m);

  ans = chebyshev_eval(a, b, cint, m, b);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Chebyshev evaluation of integral", ans, err);

  ans = clenshaw_curtis_quad(a, b, c, m, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Clenshaw - Curtis quadrature", ans, err);



  a = 0.0;
  b = 10.0*PI;
  printf("\n\n Integrate foo(x) = (1 - x)exp(-x/2)  from  a = %f  to  b = %f . . .", a, b);
  exact = 2.0*(1.0 + b)*exp(-b/2.0) - 2.0*(1.0 + a)*exp(-a/2.0);
  printf("\n %-40s    %16.12f", "Exact answer", exact);

  ans = quad_trapezoid(foo, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Trapezoid rule", ans, err);

  ans = quad_simpson(foo, a, b, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Simpson's rule", ans, err);

  ans = quad_romberg_open(foo, a, b, eps, midpoint_exp);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Open Romberg integration", ans, err);

  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  ans = quad_gauss_legendre(foo, x_leg, w_leg, n_leg);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Legendre quadrature", ans, err);

  gauss_laguerre(x_lag, w_lag, n_lag, 0.0);
  ans = 0.0;
  for (int i = 0; i < n_lag; ++i)
    ans += 2 * w_lag[i] * foonum(2.0 * x_lag[i]);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Laguerre quadrature", ans, err);

  chebyshev_fit(a, b, c, m, foo);
  chebyshev_integ(a, b, c, cint, m);
  chebyshev_deriv(a, b, c, cder, m);

  ans = chebyshev_eval(a, b, cint, m, b);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Chebyshev evaluation of integral", ans, err);

  ans = clenshaw_curtis_quad(a, b, c, m, eps);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Clenshaw - Curtis quadrature", ans, err);




  a = 0.0;
  b = PI;
  printf("\n\n Integrate funk1(x) = cos(x)/sqrt(x(PI - x))  from  a = %f  to  b = %f . . .", a, b);
  exact = 0.0;
  printf("\n %-40s    %16.12f", "Exact answer", exact);

  ans = quad_romberg_open(funk1, a, b, eps, midpoint);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Open Romberg quadrature with midpoint", ans, err);

  ans = quad_romberg_open(funk1, a, (a+b)/2, eps, midpoint_inv_sqrt_lower)
      + quad_romberg_open(funk1, (a+b)/2, b, eps, midpoint_inv_sqrt_upper);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Open Romberg with inverse sqrt step", ans, err);

  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  ans = quad_gauss_legendre(funk1, x_leg, w_leg, n_leg);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Legendre quadrature", ans, err);

  ans = quad_gauss(funk1num, a, b, x_cheb, w_cheb, n_cheb);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Chebyshev quadrature", ans, err);

  ans = quad_gauss(funk1num, a, b, x_jac, w_jac, n_jac);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Jacobi quadrature", ans, err);

  chebyshev_fit(a, b, c, m, funk1);
  chebyshev_integ(a, b, c, cint, m);

  ans = chebyshev_eval(a, b, cint, m, b);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Chebyshev evaluation of integral", ans, err);

  ans = dumb_gauss_crap(funk1num, a, b, 8);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Chebyshev quadrature", ans, err);

  printf("\n\n");
  plot_func(funk1, a+0.1, b-0.1, "", "", "", "");




  a = 0.0;
  b = PI;
  printf("\n\n Integrate funk2(x) = (2.0+sin(x))/sqrt(x(PI - x))  from  a = %f  to  b = %f . . .", a, b);
  exact = 0.0;
  printf("\n %-40s    %16.12f", "Exact answer", exact);

  ans = quad_romberg_open(funk2, a, b, eps, midpoint);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Open Romberg quadrature with midpoint", ans, err);

  ans = quad_romberg_open(funk2, a, (a+b)/2, eps, midpoint_inv_sqrt_lower)
      + quad_romberg_open(funk2, (a+b)/2, b, eps, midpoint_inv_sqrt_upper);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Open Romberg with inverse sqrt step", ans, err);

  gauss_legendre(n_leg, a, b, x_leg, w_leg);
  ans = quad_gauss_legendre(funk2, x_leg, w_leg, n_leg);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Legendre quadrature", ans, err);

  ans = quad_gauss(funk2num, a, b, x_cheb, w_cheb, n_cheb);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Chebyshev quadrature", ans, err);

  ans = quad_gauss(funk2num, a, b, x_jac, w_jac, n_jac);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Gauss - Jacobi quadrature", ans, err);

  chebyshev_fit(a, b, c, m, funk2);
  chebyshev_integ(a, b, c, cint, m);

  ans = chebyshev_eval(a, b, cint, m, b);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Chebyshev evaluation of integral", ans, err);

  ans = dumb_gauss_crap(funk2num, a, b, 8);
  err = ans - exact;
  printf("\n %-40s    %16.12f    %16.12f", "Adaptive Gauss - Chebyshev quadrature", ans, err);

  printf("\n\n");
  plot_func(funk2, a+0.1, b-0.1, "", "", "", "");



  printf("\n\n");
}



