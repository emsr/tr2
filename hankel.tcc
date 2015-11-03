#ifndef HANKEL_TCC
#define HANKEL_TCC 1

#include <complex>
#include <iostream>
#include <limits>
#include <vector>

#include "complex_util.h"
#include "airy.tcc"

template<typename _Tp>
  void
  hankel(std::complex<_Tp> nu, std::complex<_Tp> arg,
	 std::complex<_Tp> & h1, std::complex<_Tp> & h2,
	 std::complex<_Tp> & h1p, std::complex<_Tp> & h2p,
	 int & error);

template<typename _Tp>
  void
  region(std::complex<_Tp> alpha, int & indexr, char & aorb, int & error);

template<typename _Tp>
  void
  hankel_debye(std::complex<_Tp> nu, std::complex<_Tp> arg,
	       _Tp alpha, int & indexr, char & aorb, int & morn,
	       std::complex<_Tp> & h1dbye, std::complex<_Tp> & h2dbye,
	       std::complex<_Tp> & h1pdby, std::complex<_Tp> & h2pdby,
	       int & error);

template<typename _Tp>
  void
  hankel_uniform_olver(std::complex<_Tp> nu, std::complex<_Tp> z,
		       std::complex<_Tp> & h1, std::complex<_Tp> & h2,
		       std::complex<_Tp> & h1p, std::complex<_Tp> & h2p);

template<typename _Tp>
  void
  hankel_uniform_outer(std::complex<_Tp> nu, std::complex<_Tp> z, _Tp eps,
		       std::complex<_Tp> & zhat, std::complex<_Tp> & _1dnsq,
		       std::complex<_Tp> & nm1d3, std::complex<_Tp> & nm2d3,
		       std::complex<_Tp> & t, std::complex<_Tp> & tsq,
		       std::complex<_Tp> & etm3h, std::complex<_Tp> & etrat,
		       std::complex<_Tp> & aip, std::complex<_Tp> & o4dp,
		       std::complex<_Tp> & aim, std::complex<_Tp> & o4dm,
		       std::complex<_Tp> & od2p, std::complex<_Tp> & od0dp,
		       std::complex<_Tp> & od2m, std::complex<_Tp> & od0dm,
		       int & ier);

template<typename _Tp>
  void
  hankel_uniform_sum(std::complex<_Tp> t, std::complex<_Tp> tsq,
		     std::complex<_Tp> z1dnsq, std::complex<_Tp> zetm3h,
		     std::complex<_Tp> aip, std::complex<_Tp> zo4dp,
		     std::complex<_Tp> aim, std::complex<_Tp> zo4dm,
		     std::complex<_Tp> zod2p, std::complex<_Tp> zod0dp,
		     std::complex<_Tp> zod2m, std::complex<_Tp> zod0dm,
		     _Tp eps,
		     int nterms, std::vector<std::complex<_Tp>> & zwksp,
		     std::complex<_Tp> & h1sum, std::complex<_Tp> & h1psum,
		     std::complex<_Tp> & h2sum, std::complex<_Tp> & h2psum,
		     int & ier);

template<typename _Tp>
  bool
  safe_div(std::complex<_Tp> z1, std::complex<_Tp> z2,
	   std::complex<_Tp> & z1dz2);

template<typename _Tp>
  void
  airy_arg(std::complex<_Tp> znm2d3, std::complex<_Tp> zeta,
	   std::complex<_Tp> & zargp, std::complex<_Tp> & zargm, int & ier);

template<typename _Tp>
  void
  hankel_params(std::complex<_Tp> nu, std::complex<_Tp> zhat,
		std::complex<_Tp> & t, std::complex<_Tp> & tsq,
		std::complex<_Tp> & nusq, std::complex<_Tp> & z1dnsq,
		std::complex<_Tp> & znm1d3, std::complex<_Tp> & znm2d3,
		std::complex<_Tp> & znm4d3, std::complex<_Tp> & zeta,
		std::complex<_Tp> & zetaphf, std::complex<_Tp> & zetamhf,
		std::complex<_Tp> & zetm3h, std::complex<_Tp> & zetrat,
		int & ier);

/**
 *
 */
template<typename _Tp>
  void
  hankel(std::complex<_Tp> nu, std::complex<_Tp> z,
	 std::complex<_Tp> & h1, std::complex<_Tp> & h2,
	 std::complex<_Tp> & h1p, std::complex<_Tp> & h2p,
	 int & error)
  {
    static constexpr _Tp pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;

    int indexr, ierr;

    std::cout << " > hankel: nu = " << nu << " z = " << z << '\n';

    error = 0;

    auto test = std::abs((nu - z) / std::pow(nu, 1.0/3.0));
    if (test < 4.0)
      hankel_uniform(z, nu, h1, h2, h1p, h2p);
    else
      {
	auto sqtrm = std::sqrt((nu / z) * (nu / z) - 1);
	auto alpha = std::log((nu / z) + sqtrm);
	if (std::imag(alpha) < 0)
	  alpha = -alpha;
	auto alphar = std::real(alpha);
	auto alphai = std::imag(alpha);
	char aorb;
	if ((std::real(nu) > std::real(z))
	 && (std::abs(std::imag(nu/z)) <= 0))
	  {
	    indexr = 0;
	    aorb = ' ';
	  }
	else
	  region(alpha, indexr, aorb, ierr);
	auto morn = 0;
	if (aorb == 'A')
	  {
	    auto mfun = ((alphar * std::tanh(alphar) - 1) * std::tan(alphai) + alphai) / pi;
	    morn = int(mfun);
	    if (mfun < 0 && (std::fmod(mfun, 1) != 0))
	    --morn;
	  }
	else if (aorb == 'B')
	  {
	    auto nfun = ((1 - alphar * std::tanh(alphar)) * std::tan(alphai) - alphai) / pi;
	    morn = int(nfun) + 1;
	    if (nfun < 0 && std::fmod(nfun, 1) != 0)
	      --morn;
	  }
	hankel_debye(nu, z, alpha, indexr, aorb, morn,
		     h1, h2, h1p, h2p, error);
      }

    return;
  }

/**
 *  
 */
template<typename _Tp>
  void
  region(std::complex<_Tp> alpha, int & indexr, char & aorb, int & error)
  {
    static constexpr _Tp pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;

    error = 0;
    aorb = ' ';

    auto alphar = std::real(alpha);
    auto alphai = std::imag(alpha);

    auto f1 = _Tp{1}
	    - alphai * std::cos(alphai) / std::sin(alphai)
	    - alphar * std::sinh(alphar) / std::cosh(alphar);

    auto f2 = _Tp{1}
	    + (pi - alphai) * std::cos(alphai) / std::sin(alphai)
	    - alphar * std::sinh(alphar) / std::cosh(alphar);

    if (f1 > _Tp{0} && f2 > _Tp{0})
      indexr = 1;
    else if (f2 > _Tp{0})
      {
	if (alphar > _Tp{0})
	  indexr = 2;
	else
	  indexr = 3;
      }
    else if (f1 > _Tp{0})
      {
	if (alphar > _Tp{0})
	  indexr = 4;
	else
	  indexr = 5;
      }
    else
      {
	if (alphar > _Tp{0})
          indexr = 6;
	else
          indexr = 7;
        if (alphai <= (pi / _Tp{2}))
          aorb = 'A';
        else
          aorb = 'B';
      }
    return;
  }

/**
 *
 */
template<typename _Tp>
  void
  hankel_debye(std::complex<_Tp> nu, std::complex<_Tp> z, std::complex<_Tp> alpha,
	       int indexr, char & aorb, int & morn,
	       std::complex<_Tp> & h1dbye, std::complex<_Tp> & h2dbye,
	       std::complex<_Tp> & h1pdby, std::complex<_Tp> & h2pdby,
	       int & error)
  {
    using namespace std::literals::complex_literals;
    using cmplx = std::complex<_Tp>;

    static constexpr _Tp pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;
    static constexpr cmplx j = 1.0il;
    static constexpr _Tp toler = 1.0e-8;

    std::cout << " > hankel_debye: nu = " << nu << " z = " << z << '\n';

    cmplx jdbye;

    auto alphar = std::real(alpha);
    auto alphai = std::imag(alpha);
    auto thalpa = std::sinh(alpha) / std::cosh(alpha);
    auto snhalp = std::sinh(alpha);
    auto denom = std::sqrt(pi * z / 2) * std::sqrt(-j * std::sinh(alpha));
    if (std::abs(std::real(nu * (thalpa - alpha))) > 690.0)
      {
	error = 1;
	return;
      }
    auto s1 = std::exp(nu * (thalpa - alpha) - j * pi / 4) / denom;
    auto s2 = std::exp(-nu * (thalpa - alpha) + j * pi / 4) / denom;
    auto exparg = nu * (thalpa - alpha) - j * pi / 4;
    if (indexr == 0)
      {
	h1dbye = 0.5 * s1 - s2;
	h2dbye = 0.5 * s1 + s2;
	h1pdby = snhalp * (0.5 * s1 + s2);
	h2pdby = snhalp * (0.5 * s1 - s2);
      }
    else if (indexr == 1)
      {
	h1dbye = s1;
	h2dbye = s2;
	h1pdby = snhalp * s1;
	h2pdby = -snhalp * s2;
      }
    else if (indexr == 2)
      {
	jdbye = s1 / 2.0;
	h2dbye = s2;
	h1dbye = 2.0 * jdbye - h2dbye;
	h1pdby = snhalp * (s1 + s2);
	h2pdby = -snhalp * s2;
      }
    else if (indexr == 3)
      {
	h1dbye = s1;
	h2dbye = s2 - s1;
	h1pdby = snhalp * s1;
	h2pdby = -snhalp * (s1 + s2);
      }
    else if (indexr == 4)
      {
	h1dbye = s1;
	h2dbye = s2 - std::exp(2 * j * nu * pi) * s1;
	h1pdby = snhalp * s1;
	h2pdby = -snhalp * (s2 + std::exp(2 * j * nu * pi) * s1);
      }
    else if (indexr == 5)
      {
	h1dbye = s1 - std::exp(-2 * j * nu * pi) * s2;
	h2dbye = s2;
	h1pdby = snhalp * (s1 + std::exp(-2 * j * nu * pi) * s2);
	h2pdby = -snhalp * s2;
      }
    else if (aorb == 'A')
      {
	cmplx sinrat;
	if ((std::abs(std::imag(nu)) < toler)
	 && (std::abs(std::fmod(std::real(nu), 1)) < toler))
	  sinrat = morn;
	else
	  sinrat = std::sin(morn * nu * pi) / std::sin(nu * pi);
	if (indexr == 6)
	  {
	    h2dbye = s2 - std::exp(j * (morn + 1) * nu * pi) * sinrat * s1;
	    h1dbye = s1 - h2dbye;
	    h2pdby = -snhalp * (s2 + std::exp(j * (morn + 1) * nu * pi) * sinrat * s1);
	    h1pdby = snhalp * ((1 + std::exp(j * (morn + 1) * nu * pi) * sinrat) * s1 + s2);
	  }
	else if (indexr == 7)
	  {
	    h1dbye = s1 - std::exp(-j * (morn + 1) * nu * pi) * sinrat * s2;
	    h2dbye = s2 - h1dbye;
	    h1pdby = snhalp * (s1 + std::exp(-j * (morn + 1) * nu * pi) * sinrat * s2);
	    h2pdby = -snhalp *
		     ((1.0 + std::exp(-j * (morn + 1) * nu * pi) * sinrat) * s2 + s1);
	  }
	else
	  error = 1;
      }
    else
      {
	cmplx sinrat;
	if ((std::abs(std::imag(nu)) < toler)
	 && (std::abs(std::fmod(dreal(nu), 1)) < toler))
	  sinrat = -morn;
	else
	  sinrat = std::sin(morn * nu * pi) / std::sin(nu * pi);
	if (indexr == 6)
	  {
	    h1dbye = s1 - std::exp(j * (morn - 1) * nu * pi) * sinrat * s2;
	    h2dbye = s2 - std::exp(2 * j * nu * pi) * h2dbye;
	    h1pdby = snhalp * (s1 + std::exp(j * (morn - 1) * nu * pi) * sinrat * s2);
	    h2pdby = -snhalp * ((1 + std::exp(j * (morn + 1) * nu * pi) * sinrat) * s2
			     + std::exp(2 * j * nu * pi) * s1);
	  }
	else if (indexr == 7)
	  {
	    h2dbye = s2 - std::exp(-j * (morn - 1) * nu * pi) * sinrat * s1;
	    h1dbye = s1 - std::exp(-2 * j * nu * pi) * h2dbye;
	    h2pdby = -snhalp * (s2 + std::exp(-j * (morn - 1) * nu * pi) * sinrat * s1);
	    h1pdby = snhalp * ((1 + std::exp(-j * (morn + 1) * nu * pi) * sinrat) * s1
			     + std::exp(-2 * j * nu * pi) * s2);
	  }
	else
	  error = 1;
      }

    return;
  }

/**
 *  @brief
 *      This routine uses the old hankel_uniform subroutine of
 *      Whitaker (now called hankel_uniform_olver) to compute the
 *      uniform asymptotic approximations of the Hankel functions
 *      and their derivatives including a patch for the case when
 *      the order equals or nearly equals the argument.  At such 
 *      points, Olver's expressions have zero denominators (and
 *      numerators) resulting in numerical problems.  This routine
 *      averages four surrounding points in the complex order plane
 *      to obtain the result in such cases.
 *
 *  @param[in]  nu  The order for which the Hankel functions are to be evaluated.
 *  @param[in]  z   The argument at which the Hankel functions are to be evaluated.
 *  @param[out] h1  The computed value for the Hankel function of the first kind.
 *  @param[out] h1p The computed value for the derivative of the Hankel function of the first kind.
 *  @param[out] h2  The computed value for the Hankel function of the second kind.
 *  @param[out] h2p The computed value for the derivative of the Hankel function of the second kind.
 */
template<typename _Tp>
  void
  hankel_uniform(std::complex<_Tp> nu, std::complex<_Tp> z,
		 std::complex<_Tp> & h1, std::complex<_Tp> & h2,
		 std::complex<_Tp> & h1p, std::complex<_Tp> & h2p)
  {
    using cmplx = std::complex<_Tp>;
    _Tp test = std::pow(std::abs(nu), _Tp(1) / _Tp(3)) / _Tp(5);

    std::cout << " > hankel_uniform: nu = " << nu << " z = " << z << '\n';

    if (std::abs(z - nu) > test)
      hankel_uniform_olver(nu, z, h1, h2, h1p, h2p);
    else
      {
	_Tp r = 2 * test;
	std::complex<_Tp> anu[4]{nu + cmplx{r, 0.0L},
				 nu + cmplx{0.0L, r},
				 nu - cmplx{r, 0.0L},
				 nu - cmplx{0.0L, r}};

	h1  = cmplx{};
	h2  = cmplx{};
	h1p = cmplx{};
	h2p = cmplx{};
	for (auto tnu : anu)
	  {
	    std::complex<_Tp> th1, th2, th1p, th2p;
	    hankel_uniform_olver(tnu, z, th1, th2, th1p, th2p);
	    h1  += th1;
	    h2  += th2;
	    h1p += th1p;
	    h2p += th2p;
	  }
	h1  /= _Tp(4);
	h2  /= _Tp(4);
	h1p /= _Tp(4);
	h2p /= _Tp(4);
      }

    return;
  }


/**
 *  @brief Compute approximate values for the Hankel functions
 *         of the first and second kinds using Olver's uniform asymptotic
 *         expansion to of order @c nu along with their derivatives.
 *
 *  @param[in]  nu  The order for which the Hankel functions are to be evaluated.
 *  @param[in]  z   The argument at which the Hankel functions are to be evaluated.
 *  @param[out] h1  The value of the Hankel function of the first kind.
 *  @param[out] h1p The value of the derivative of the Hankel function of the first kind.
 *  @param[out] h2  The value of the Hankel function of the second kind.
 *  @param[out] h2p The value of the derivative of the Hankel function of the second kind.
 */
template<typename _Tp>
  void
  hankel_uniform_olver(std::complex<_Tp> nu, std::complex<_Tp> z,
		       std::complex<_Tp> & h1, std::complex<_Tp> & h2,
		       std::complex<_Tp> & h1p, std::complex<_Tp> & h2p)
  {
    using namespace std::literals::complex_literals;
    using cmplx = std::complex<_Tp>;


    std::cout << " > hankel_uniform_olver: nu = " << nu << " z = " << z << '\n';

    static constexpr _Tp pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;
    //static constexpr _Tp pi_3 = 1.0471'97551'19659'77461'54214'46109'31676'28063e+0L;
    static constexpr cmplx con1p{ 1.0, 1.732050807568877}; // 2*exp( pi*j/3)
    static constexpr cmplx con1m{ 1.0,-1.732050807568877}; // 2*exp(-pi*j/3)
    static constexpr cmplx con2p{-2.0, 3.464101615137755}; // 4*exp( 2*pi*j/3)
    static constexpr cmplx con2m{-2.0,-3.464101615137755}; // 4*exp(-2*pi*j/3)
    static constexpr cmplx j{1il};
    static constexpr _Tp eps   = 1.0e-06;
    static constexpr _Tp epsai = 1.0e-12;
    int nterms = 4;

    //  Extended to accommodate negative real orders.
    bool nuswitch = false;
    if (std::real(nu) < _Tp(0))
      {
	nuswitch = true;
	nu = -nu;
      }

    // Compute outer factors in the uniform asymptotic expansions
    // for the Hankel functions and their derivatives along with
    // other important functions of nu and z.
    cmplx t, tsq,
	  _1dnsq, etm3h, aip, o4dp, aim, o4dm,
	  od2p, od0dp, od0dm, tmp, zhat, nm1d3,
	  nm2d3, etrat, od2m, r_factor;
    static constexpr int nwksp = 100; // > some function of nterms
    std::vector<cmplx> wksp(nwksp);
    int ier;
    hankel_uniform_outer(nu, z, epsai, zhat, _1dnsq, nm1d3, nm2d3, t, tsq,
			 etm3h, etrat, aip, o4dp, aim, o4dm, od2p,
			 od0dp, od2m, od0dm, ier);

    // Check for successful completion
    if (ier == 0)
      {
	// Compute further terms in the expansions in their appropriate linear combinations.

	hankel_uniform_sum(t, tsq, _1dnsq, etm3h, aip, o4dp, aim, o4dm,
			   od2p, od0dp, od2m, od0dm, eps, nterms, wksp,
			   h1, h1p, h2, h2p, ier);

	// Assemble approximations.
	tmp = etrat * nm1d3;
	h1 = con1m * tmp * h1;
	h2 = con1p * tmp * h2;
	tmp = nm2d3 / (zhat * etrat);
	h1p = con2p * tmp * h1p;
	h2p = con2m * tmp * h2p;

	// Check for successful completion
	if (ier != 0)
	  {
	    std::cerr << "  hankel_uniform_sum: "
		      << "  ier = " << ier << '\n'
		      << "  nu  = " << nu << '\n'
		      << "  z   = " << z << '\n';
	  }
      }
    else
      std::cerr << "  hankel_uniform_outer: "
		<< "  ier = " << ier << '\n'
		<< "  nu  = " << nu << '\n'
		<< "  z   = " << z << '\n';

    if (nuswitch)
      {
	r_factor = std::exp(j * nu * pi);
	h1  = h1  * r_factor;
	h1p = h1p * r_factor;
	h2  = h2  / r_factor;
	h2p = h2p / r_factor;
	nu  = -nu;
      }

    return;
  }


/**
 *  @brief Compute outer factors and associated functions of @c z and @c nu
 *         appearing in Olver's uniform asymptotic expansions of the
 *         Hankel functions of the first and second kinds and their derivatives.
 *         The various functions of z and nu returned by @c hankel_uniform_outer
 *         are available for use in computing further terms in the expansions.
 */
template<typename _Tp>
  void
  hankel_uniform_outer(std::complex<_Tp> nu, std::complex<_Tp> z, _Tp eps,
		       std::complex<_Tp> & zhat, std::complex<_Tp> & _1dnsq,
		       std::complex<_Tp> & nm1d3, std::complex<_Tp> & nm2d3,
		       std::complex<_Tp> & t, std::complex<_Tp> & tsq,
		       std::complex<_Tp> & etm3h, std::complex<_Tp> & etrat,
		       std::complex<_Tp> & aip, std::complex<_Tp> & o4dp,
		       std::complex<_Tp> & aim, std::complex<_Tp> & o4dm,
		       std::complex<_Tp> & od2p, std::complex<_Tp> & od0dp,
		       std::complex<_Tp> & od2m, std::complex<_Tp> & od0dm,
		       int & error)
  {
    using cmplx = std::complex<_Tp>;

    static constexpr cmplx e2pd3{-0.5L,  0.8660254037844386L};
    static constexpr cmplx d2pd3{-0.5L, -0.8660254037844386L};

    std::cout << " > hankel_uniform_outer: nu = " << nu << " z = " << z << '\n';


    error = 0;

    if (safe_div(z, nu, zhat))
      {
	//  Try to compute other nu and z dependent parameters except args to Airy functions
	cmplx nm4d3, nusq, zeta, etphf, etmhf;
	hankel_params(nu, zhat, t, tsq, nusq, _1dnsq, nm1d3, nm2d3, nm4d3,
		      zeta, etphf, etmhf, etm3h, etrat, error);

        std::cout.precision(std::numeric_limits<double>::max_digits10);
        std::cout << " > > t      = " << t << '\n';
        std::cout << " > > tsq    = " << tsq << '\n';
        std::cout << " > > nusq   = " << nusq << '\n';
        std::cout << " > > _1dnsq = " << _1dnsq << '\n';
        std::cout << " > > nm1d3  = " << nm1d3 << '\n';
        std::cout << " > > nm2d3  = " << nm2d3 << '\n';
        std::cout << " > > nm4d3  = " << nm4d3 << '\n';
        std::cout << " > > zeta   = " << zeta << '\n';
        std::cout << " > > etphf  = " << etphf << '\n';
        std::cout << " > > etmhf  = " << etmhf << '\n';
        std::cout << " > > etm3h  = " << etm3h << '\n';
        std::cout << " > > etrat  = " << etrat << '\n';
        std::cout << " > > error  = " << error << '\n';

	if (error == 0)
	  {
	    //  Try to compute Airy function arguments
	    cmplx argp, argm;
	    airy_arg(nm2d3, zeta, argp, argm, error);
	    std::cout << " > > nm2d3 = " << nm2d3 << '\n';
	    std::cout << " > > zeta  = " << zeta << '\n';
	    std::cout << " > > argp  = " << argp << '\n';
	    std::cout << " > > argm  = " << argm << '\n';
	    std::cout << " > > error = " << error << '\n';

	    if (error == 0)
	      {
		//  Compute Airy functions and derivatives
		cmplx aipp, aimp;
		int errorp, errorm;
		airy(argp, eps, aip, aipp, errorp);
		std::cout << " > > argp   = " << argp << '\n';
		std::cout << " > > aip    = " << aip << '\n';
		std::cout << " > > aipp   = " << aipp << '\n';
		std::cout << " > > errorp = " << errorp << '\n';
		airy(argm, eps, aim, aimp, errorm);
		std::cout << " > > argm   = " << argm << '\n';
		std::cout << " > > aim    = " << aim << '\n';
		std::cout << " > > aimp   = " << aimp << '\n';
		std::cout << " > > errorm = " << errorm << '\n';
		if (errorp == 0 && errorm == 0)
		  {
		    //  Compute partial outer terms in expansions
		    o4dp = -etmhf * nm4d3 * e2pd3 * aipp;
		    o4dm = -etmhf * nm4d3 * d2pd3 * aimp;
		    od2p = -etphf * nm2d3 * aip;
		    od0dp = e2pd3 * aipp;
		    od2m = -etphf * nm2d3 * aim;
		    od0dm = d2pd3 * aimp;
		  }
		else  //  Error in evaluation of Airy functions
		  error = 134;
	      }
	    else  //  Airy function args not computable
	      error = 133;
	  }
	else  //  Factors not successfully computed
	  error = 135;
      }
    else //  z/nu not successfully computed
      error = 130;

    return;
  }


/**
 *  @brief
 *    Compute the sums in appropriate linear combinations appearing in
 *    Olver's uniform asymptotic expansions for the Hankel functions
 *    of the first and second kinds and their derivatives, using up to
 *    nterms (less than 5) to achieve relative error eps.
 */
template<typename _Tp>
  void
  hankel_uniform_sum(std::complex<_Tp> t, std::complex<_Tp> tsq,
		     std::complex<_Tp> z1dnsq, std::complex<_Tp> zetm3h,
		     std::complex<_Tp> aip, std::complex<_Tp> zo4dp,
		     std::complex<_Tp> aim, std::complex<_Tp> zo4dm,
		     std::complex<_Tp> zod2p, std::complex<_Tp> zod0dp,
		     std::complex<_Tp> zod2m, std::complex<_Tp> zod0dm,
		     _Tp eps,
		     int nterms, std::vector<std::complex<_Tp>> & zwksp,
		     std::complex<_Tp> & h1sum, std::complex<_Tp> & h1psum,
		     std::complex<_Tp> & h2sum, std::complex<_Tp> & h2psum,
		     int & ier)
  {
    using cmplx = std::complex<_Tp>;

    _Tp dukta, dvkta, duktb, dvktb, dukpta,
     	dvkpta, dukptb, dvkptb, dsdata;

    int index, indexp, nduv, indexend, i2k,
	i2kp1, i2km1, indexv, indexvpl, i2kl;

    bool coverged;

    static constexpr auto zone = cmplx{1, 0};

    std::cout << " > hankel_uniform_sum:\n";
    std::cout << " > t     = " << t << '\n';
    std::cout << " > tsq   = " << tsq << '\n';
    std::cout << " > z1dnsq = " << z1dnsq << '\n';
    std::cout << " > zetm3h = " << zetm3h << '\n';
    std::cout << " > aip    = " << aip << '\n';
    std::cout << " > zo4dp  = " << zo4dp << '\n';
    std::cout << " > aim    = " << aim << '\n';
    std::cout << " > zo4dm  = " << zo4dm << '\n';
    std::cout << " > zod2p  = " << zod2p << '\n';
    std::cout << " > zod0dp = " << zod0dp << '\n';
    std::cout << " > zod2m  = " << zod2m << '\n';
    std::cout << " > zod0dm = " << zod0dm << '\n';

    //  Coefficients for u and v polynomials appearing in Olver's
    //  uniform asymptotic expansions for the Hankel functions
    //  and their derivatives

    static constexpr _Tp
    a[66]
    {
       0.1000000000000000e+01,
      -0.2083333333333333e+00,
       0.1250000000000000e+00,
       0.3342013888888889e+00,
      -0.4010416666666667e+00,
       0.7031250000000000e-01,
      -0.1025812596450617e+01,
       0.1846462673611111e+01,
      -0.8912109136581421e+00,
       0.7324218750000000e-01,
       0.4669584423426247e+01,
      -0.1120700261622299e+02,
       0.8789123535156250e+01,
      -0.2364086866378784e+01,
       0.1121520996093750e+00,
      -0.2821207255820024e+02,
       0.8463621767460073e+02,
      -0.9181824154324002e+02,
       0.4253499984741211e+02,
      -0.7368794441223145e+01,
       0.2271080017089844e+00,
       0.2125701300392171e+03,
      -0.7652524681411816e+03,
       0.1059990452528000e+04,
      -0.6995796273761325e+03,
       0.2181905059814453e+03,
      -0.2649143028259277e+02,
       0.5725014209747314e+00,
      -0.1919457662318407e+04,
       0.8061722181737309e+04,
      -0.1358655000643414e+05,
       0.1165539333686453e+05,
      -0.5305646972656250e+04,
       0.1200902954101563e+04,
      -0.1080909194946289e+03,
       0.1727727532386780e+01,
       0.2020429133096615e+05,
      -0.9698059838863751e+05,
       0.1925470012325315e+06,
      -0.2034001772804155e+06,
       0.1222004649830175e+06,
      -0.4119265625000000e+05,
       0.7109514160156250e+04,
      -0.4939153137207031e+03,
       0.6074041843414307e+01,
      -0.2429191879005513e+06,
       0.1311763614662977e+07,
      -0.2998015918538107e+07,
       0.3763271297656404e+07,
      -0.2813563226586534e+07,
       0.1268365250000000e+07,
      -0.3316451875000000e+06,
       0.4521876953125000e+05,
      -0.2499830566406250e+04,
       0.2438052940368652e+02,
       0.3284469853072038e+07,
      -0.1970681911843223e+08,
       0.5095260249266464e+08,
      -0.7410514821153266e+08,
       0.6634451227472903e+08,
      -0.3756717666076335e+08,
       0.1328876700000000e+08,
      -0.2785618250000000e+07,
       0.3081864062500000e+06,
      -0.1388608984375000e+05,
       0.1100171432495117e+03
    };

    static constexpr _Tp
    b[66]
    {  0.1000000000000000e+01,
       0.2916666666666667e+00,
      -0.3750000000000000e+00,
      -0.3949652777777778e+00,
       0.5156250000000000e+00,
      -0.1171875000000000e+00,
       0.1146496431327160e+01,
      -0.2130533854166667e+01,
       0.1089257836341858e+01,
      -0.1025390625000000e+00,
      -0.5075635242854617e+01,
       0.1238668710214120e+02,
      -0.9961006673177083e+01,
       0.2793920993804932e+01,
      -0.1441955566406250e+00,
       0.3015773273462785e+02,
      -0.9140711508856879e+02,
       0.1005628359759295e+03,
      -0.4753911590576172e+02,
       0.8502454757690430e+01,
      -0.2775764465332031e+00,
      -0.2247169946128867e+03,
       0.8146235951180321e+03,
      -0.1138508263826370e+04,
       0.7604126384523180e+03,
      -0.2411579284667969e+03,
       0.3002362060546875e+02,
      -0.6765925884246826e+00,
       0.2013089743407110e+04,
      -0.8497490948317704e+04,
       0.1440997727955136e+05,
      -0.1245921356699312e+05,
       0.5730098632812500e+04,
      -0.1315274658203125e+04,
       0.1208074951171875e+03,
      -0.1993531703948975e+01,
      -0.2106404840887960e+05,
       0.1014913238950858e+06,
      -0.2024212064239434e+06,
       0.2150230445535821e+06,
      -0.1300843659496637e+06,
       0.4424396093750000e+05,
      -0.7727732910156250e+04,
       0.5459063720703125e+03,
      -0.6883914470672607e+01,
       0.2520859497081193e+06,
      -0.1365304986690037e+07,
       0.3131261070473134e+07,
      -0.3946845507298180e+07,
       0.2965647725320941e+07,
      -0.1345235875000000e+07,
       0.3545172500000000e+06,
      -0.4883626953125000e+05,
       0.2737909667968750e+04,
      -0.2724882698059082e+02,
      -0.3395807814193124e+07,
       0.2042343072273885e+08,
      -0.5295074376688679e+08,
       0.7725855877372554e+08,
      -0.6943030354332107e+08,
       0.3949369854080250e+08,
      -0.1404812500000000e+08,
       0.2965335500000000e+07,
      -0.3310150312500000e+06,
       0.1509357617187500e+05,
      -0.1215978927612305e+03
    };

    //  lambda and mu coefficients appearing in the expansions
    static constexpr _Tp
    lambda[21]
    {
       0.1041666666666667e+00,
       0.8355034722222222e-01,
       0.1282265745563272e+00,
       0.2918490264641405e+00,
       0.8816272674437577e+00,
       0.3321408281862768e+01,
       0.1499576298686255e+02,
       0.7892301301158652e+02,
       0.4744515388682643e+03,
       0.3207490090890662e+04,
       0.2408654964087401e+05,
       0.1989231191695098e+06,
       0.1791902007775344e+07,
       0.1748437718003412e+08,
       0.1837073796763307e+09,
       0.2067904032945155e+10,
       0.2482751937593589e+11,
       0.3166945498173489e+12,
       0.4277112686513472e+13,
       0.6097113241139256e+14,
       0.9148694223435640e+15
    };

    static constexpr _Tp
    mu[21]
    {
      -0.1458333333333333e+00,
      -0.9874131944444445e-01,
      -0.1433120539158951e+00,
      -0.3172272026784136e+00,
      -0.9424291479571203e+00,
      -0.3511203040826354e+01,
      -0.1572726362036805e+02,
      -0.8228143909718595e+02,
      -0.4923553705236705e+03,
      -0.3316218568547973e+04,
      -0.2482767424520859e+05,
      -0.2045265873151298e+06,
      -0.1838444917068210e+07,
      -0.1790568747352892e+08,
      -0.1878356353993943e+09,
      -0.2111438854691369e+10,
      -0.2531915342298413e+11,
      -0.3226140741130003e+12,
      -0.4352813796009286e+13,
      -0.6199585732586975e+14,
      -0.9295073331010611e+15
    };

    ier = 0;
    //  Initialize for modified Horner's rule evaluation of u_k and v_k polynomials
    auto dxtsq = std::real(tsq);
    auto dytsq = std::imag(tsq);
    auto dr = 2 * dxtsq;
    //  Compute square of magnitudes
    auto ds = std::norm(tsq);

    //  Compute u_0,1,2 and v_0,1,2 and store for later use
    indexv = 2 * nterms;
    std::cout << " > > indexv = " << indexv << '\n';
    auto tk = t;
    zwksp[0] = tk * (a[1] * tsq + a[2]);
    zwksp[indexv] = tk * (b[1] * tsq + b[2]);
    std::cout << " > > tk = " << tk << '\n';
    std::cout << " > > zwksp[0] = " << zwksp[0] << '\n';
    std::cout << " > > zwksp[indexv] = " << zwksp[indexv] << '\n';
    auto dytsq2 = dytsq * dytsq;
    tk *= t;
    zwksp[1] = tk * cmplx((a[3] * dxtsq + a[4]) * dxtsq + a[5] - a[3] * dytsq2,
			       (2 * a[3] * dxtsq + a[4]) * dytsq);
    zwksp[indexv + 1] = tk
		    * cmplx((b[3] * dxtsq + b[4]) * dxtsq + b[5] - b[3] * dytsq2,
			     (2 * b[3] * dxtsq + b[4]) * dytsq);
    std::cout << " > > tk = " << tk << '\n';
    std::cout << " > > zwksp[1] = " << zwksp[1] << '\n';
    std::cout << " > > zwksp[indexv + 1] = " << zwksp[indexv + 1] << '\n';
    tk *= t;
    zwksp[2] = tk
	     * cmplx(((a[6] * dxtsq + a[7]) * dxtsq + a[8]) * dxtsq
     		      + a[9] - (3 * a[6] * dxtsq + a[7]) * dytsq2,
     		      ((3 * a[6] * dxtsq + 2 * a[7]) * dxtsq + a[8]
     		      - a[6] * dytsq2) * dytsq);
    zwksp[indexv + 2] = tk
		    * cmplx(((b[6] * dxtsq + b[7]) * dxtsq + b[8])
     		      * dxtsq + b[9] - (3 * b[6] * dxtsq + b[7]) * dytsq2,
     		      ((3 * b[6] * dxtsq + 2 * b[7]) * dxtsq + b[8]
     		      - b[6] * dytsq2) * dytsq);
    std::cout << " > > tk = " << tk << '\n';
    std::cout << " > > zwksp[2] = " << zwksp[2] << '\n';
    std::cout << " > > zwksp[indexv + 2] = " << zwksp[indexv + 2] << '\n';

    //  Compute a_0,1, b_0,1, c_0,1, d_0,1 ... note that
    //  b_k and c_k are computed up to -zeta**(-1/2)
    //  -zeta**(1/2) factors, respectively.  These recurring factors
    //  are included as appropriate in the outer factors, thus saving
    //  repeated multiplications by them.
    auto a0 = zone;
    auto a1 = zwksp[1]
	    + zetm3h * (mu[1] * zetm3h
		     +  mu[0] * zwksp[0]);
    auto b0 = zwksp[0] + lambda[0] * zetm3h;
    auto b1 = zwksp[2]
	    + zetm3h * (zetm3h * (lambda[2] * zetm3h
				+ lambda[1] * zwksp[0])
		      + lambda[0] * zwksp[1]);
    auto c0 = zwksp[indexv] + mu[0] * zetm3h;
    auto c1 = zwksp[indexv + 2]
	    + zetm3h * (zetm3h * (mu[2] * zetm3h
				+ mu[1] * zwksp[indexv])
		      + mu[0] * zwksp[indexv + 1]);
    auto d0 = zone;
    auto d1 = zwksp[indexv + 1]
	    + zetm3h * (lambda[1] * zetm3h
		      + lambda[0] * zwksp[indexv]);
    std::cout << " > > a1 = " << a1 << '\n';
    std::cout << " > > b0 = " << b0 << '\n';
    std::cout << " > > b1 = " << b1 << '\n';
    std::cout << " > > c0 = " << c0 << '\n';
    std::cout << " > > c1 = " << c1 << '\n';
    std::cout << " > > d1 = " << d1 << '\n';

    //  Compute terms
    auto aterm = a1 * z1dnsq;
    auto bterm = b1 * z1dnsq;
    auto cterm = c1 * z1dnsq;
    auto dterm = d1 * z1dnsq;
    //  Compute sum of first two terms, thus initializing Kahan summing scheme
    auto asum = a0 + aterm;
    auto atemp = aterm - (asum - zone);
    auto bsum = b0 + bterm;
    auto btemp = bterm - (bsum - b0);
    auto csum = c0 + cterm;
    auto ctemp = cterm - (csum - c0);
    auto dsum = d0 + dterm;
    auto dtemp = dterm - (dsum - zone);
    std::cout << " > > asum = " << asum << '\n';
    std::cout << " > > bsum = " << bsum << '\n';
    std::cout << " > > csum = " << csum << '\n';
    std::cout << " > > dsum = " << dsum << '\n';

    //  Set convergence flag to no convergence indication
    coverged = false;

    //  Combine sums in form appearing in expansions
    h1sum = aip * asum + zo4dp * bsum;
    h2sum = aim * asum + zo4dm * bsum;
    h1psum = zod2p * csum + zod0dp * dsum;
    h2psum = zod2m * csum + zod0dm * dsum;
    auto h1save = aip + zo4dp * b0;
    auto h2save = aim + zo4dm * b0;
    auto h1psave = zod2p * c0 + zod0dp;
    auto h2psave = zod2m * c0 + zod0dm;
    std::cout << " > > > h1sum = " << h1sum << '\n';
    std::cout << " > > > h2sum = " << h2sum << '\n';
    std::cout << " > > > h1psum = " << h1psum << '\n';
    std::cout << " > > > h2psum = " << h2psum << '\n';
    std::cout << " > > > h1save = " << h1save << '\n';
    std::cout << " > > > h2save = " << h2save << '\n';
    std::cout << " > > > h1psave = " << h1psave << '\n';
    std::cout << " > > > h2psave = " << h2psave << '\n';

    //  If convergence criteria now satisfied
    if (norm1(h1sum - h1save) < eps * norm1(h1sum)
     && norm1(h2sum - h2save) < eps * norm1(h2sum)
     && norm1(h1psum - h1psave) < eps * norm1(h1psum)
     && norm1(h2psum - h2psave) < eps * norm1(h2psum))
      coverged = true;

    //  Save current sums combined as in expansion for next convergence test
    h1save = h1sum;
    h2save = h2sum;
    h1psave = h1psum;
    h2psave = h2psum;

    //  Update index into u_k and v_k coefficients
    index = 9;//10;
    indexp = 14;//15;
    //  Update index into storage for u and v polynomials
    nduv = 3;//4;
    //  Update power of nu**(-2)
    auto z1dn2k = z1dnsq;

    //  Loop until convergence criteria satisfied or maximum number of terms reached
    for (auto k = 2; k <= nterms; ++k)
      {
	//  Initialize for evaluation of two new u and v polynomials
	//  via Horner's rule modified for complex arguments and real coefficients
	indexend = indexp;
	++index;
	dukta = a[index];
	dvkta = b[index];
	++index;
	duktb = a[index];
	dvktb = b[index];
	++indexp;
	dukpta = a[indexp];
	dvkpta = b[indexp];
	++indexp;
	dukptb = a[indexp];
	dvkptb = b[indexp];
	//  Update indices into coefficients to reflect initialization
	++index;
	++indexp;

	//  Loop until quantities to evaluate lowest order u and v 
	//  polynomials and partial quantities to evaluate
	//  next highest order polynomials computed
	for (auto l = index; l < indexend; ++l)
	  {
	    dsdata = ds * dukta;
	    dukta = duktb + dr * dukta;
	    duktb = a[l] - dsdata;
	    dsdata = ds * dvkta;
	    dvkta = dvktb + dr * dvkta;
	    dvktb = b[l] - dsdata;
	    dsdata = ds * dukpta;
	    dukpta = dukptb + dr * dukpta;
	    dukptb = a[indexp] - dsdata;
	    dsdata = ds * dvkpta;
	    dvkpta = dvkptb + dr * dvkpta;
	    dvkptb = b[indexp] - dsdata;
	    ++indexp;
	  }

	//  One more iteration for highest order polynomials
	dsdata = ds * dukpta;
	dukpta = dukptb + dr * dukpta;
	dukptb = a[indexp] - dsdata;
	dsdata = ds * dvkpta;
	dvkpta = dvkptb + dr * dvkpta;
	dvkptb = b[indexp] - dsdata;

	//  Post multiply and form new polynomials
	tk *= t;
	zwksp[nduv] = tk * (dukta * tsq + duktb);
	zwksp[indexv + nduv] = tk * (dvkta * tsq + dvktb);
	std::cout << " > > > nduv = " << nduv << '\n';
	std::cout << " > > > zwksp[nduv] = " << zwksp[nduv] << '\n';
	std::cout << " > > > zwksp[indexv + nduv] = " << zwksp[indexv + nduv] << '\n';
	tk *= t;
	++nduv = nduv;
	zwksp[nduv] = tk * (dukpta * tsq + dukptb);
	zwksp[indexv + nduv] = tk * (dvkpta * tsq + dvkptb);
	std::cout << " > > > nduv = " << nduv << '\n';
	std::cout << " > > > zwksp[nduv] = " << zwksp[nduv] << '\n';
	std::cout << " > > > zwksp[indexv + nduv] = " << zwksp[indexv + nduv] << '\n';

	//  Update indices in preparation for next iteration
	++nduv = nduv;
	index = indexp;
	i2k = 2 * k - 1;
	i2km1 = i2k - 1 - 1;
	i2kp1 = i2k + 1 - 1;
	indexp = indexp + i2kp1 + 2;

	//  Initialize for evaluation of a, b, c, and d polynomials via Horner's rule.
	a1 = mu[i2k] * zetm3h + mu[i2km1] * zwksp[0];
	b1 = lambda[i2kp1] * zetm3h + lambda[i2k] * zwksp[0];
	c1 = mu[i2kp1] * zetm3h + mu[i2k] * zwksp[indexv];
	d1 = lambda[i2k] * zetm3h + lambda[i2km1] * zwksp[indexv];
	std::cout << " > > > a1 = " << a1 << '\n';
	std::cout << " > > > b1 = " << b1 << '\n';
	std::cout << " > > > c1 = " << c1 << '\n';
	std::cout << " > > > d1 = " << d1 << '\n';

	//  loop until partial a, b, c, and d evaluations done via Horner's rule
	for(auto l = 1; l < i2km1; ++l)
	  {
	    indexvpl = indexv + l;
	    i2kl = i2k - l;
	    a1 = a1 * zetm3h + mu[i2kl] * zwksp[l];
	    d1 = d1 * zetm3h + lambda[i2kl] * zwksp[indexvpl];
	    i2kl = i2kp1 - l;
	    b1 = b1 * zetm3h + lambda[i2kl] * zwksp[l];
	    c1 = c1 * zetm3h + mu[i2kl] * zwksp[indexvpl];
	  }

	//  Complete the evaluations
	a1 = a1 * zetm3h + zwksp[i2k];
	d1 = d1 * zetm3h + zwksp[indexv + i2k];
	b1 = zetm3h
     	    * (b1 * zetm3h + lambda[0] * zwksp[i2k])
	    + zwksp[i2kp1];
	c1 = zetm3h
     	    * (c1 * zetm3h + mu[0] * zwksp[indexv + i2k])
     	    + zwksp[indexv + i2kp1];
	std::cout << " > > > a1 = " << a1 << '\n';
	std::cout << " > > > b1 = " << b1 << '\n';
	std::cout << " > > > c1 = " << c1 << '\n';
	std::cout << " > > > d1 = " << d1 << '\n';

	//  Evaluate new terms for sums
	z1dn2k *= z1dnsq;
	aterm = a1 * z1dn2k + atemp;
	bterm = b1 * z1dn2k + btemp;
	cterm = c1 * z1dn2k + ctemp;
	dterm = d1 * z1dn2k + dtemp;
	std::cout << " > > > aterm = " << aterm << '\n';
	std::cout << " > > > bterm = " << bterm << '\n';
	std::cout << " > > > cterm = " << cterm << '\n';
	std::cout << " > > > dterm = " << dterm << '\n';

	//  Update sums via Kahan summing scheme
	atemp = asum;
	asum += aterm;
	atemp = aterm - (asum - atemp);
	btemp = bsum;
	bsum += bterm;
	btemp = bterm - (bsum - btemp);
	ctemp = csum;
	csum += cterm;
	ctemp = cterm - (csum - ctemp);
	dtemp = dsum;
	dsum += dterm;
	dtemp = dterm - (dsum - dtemp);
	std::cout << " > > > asum = " << asum << '\n';
	std::cout << " > > > bsum = " << bsum << '\n';
	std::cout << " > > > csum = " << csum << '\n';
	std::cout << " > > > dsum = " << dsum << '\n';

	//  Combine sums in form appearing in expansions
	h1sum  = aip  * asum  + zo4dp * bsum;
	h2sum  = aim  * asum  + zo4dm * bsum;
	h1psum = zod2p * csum + zod0dp * dsum;
	h2psum = zod2m * csum + zod0dm * dsum;
	std::cout << " > > > h1sum = " << h1sum << '\n';
	std::cout << " > > > h2sum = " << h2sum << '\n';
	std::cout << " > > > h1psum = " << h1psum << '\n';
	std::cout << " > > > h2psum = " << h2psum << '\n';
	std::cout << " > > > h1save = " << h1save << '\n';
	std::cout << " > > > h2save = " << h2save << '\n';
	std::cout << " > > > h1psave = " << h1psave << '\n';
	std::cout << " > > > h2psave = " << h2psave << '\n';

	//  If convergence criteria met this term, see if it was before
	if (norm1(h1sum - h1save) < eps * norm1(h1sum)
	 && norm1(h2sum - h2save) < eps * norm1(h2sum)
	 && norm1(h1psum - h1psave) < eps * norm1(h1psum)
	 && norm1(h2psum - h2psave) < eps * norm1(h2psum)) 
	  {
	    if (coverged) // Convergence - relative error criteria met twice in a row
	      return;
	    else  //  Converged
	      coverged = true;
	  }
	else  //  Reset relative error criteria flag
	  coverged = false;

	//  Save combined sums for comparison next iteration
	h1save = h1sum;
	h2save = h2sum;
	h1psave = h1psum;
	h2psave = h2psum;
      }

    //  All allowable terms used - set completion code
    ier = 177;

    return;
  }


/**
 *    Compute parameters depending on z and nu that appear in the
 *    uniform asymptotic expansions of the Hankel functions and
 *    their derivatives, except the arguments to the Airy functions.
 */
template<typename _Tp>
  void
  hankel_params(std::complex<_Tp> nu, std::complex<_Tp> zhat,
		std::complex<_Tp> & t, std::complex<_Tp> & tsq,
		std::complex<_Tp> & nusq, std::complex<_Tp> & z1dnsq,
		std::complex<_Tp> & znm1d3, std::complex<_Tp> & znm2d3,
		std::complex<_Tp> & znm4d3, std::complex<_Tp> & zeta,
		std::complex<_Tp> & zetaphf, std::complex<_Tp> & zetamhf,
		std::complex<_Tp> & zetm3h, std::complex<_Tp> & zetrat,
		int & ier)
  {
    using cmplx = std::complex<_Tp>;

    //  data statements defining constants used in this subroutine
    //  note that dinf and dinfsr are machine floating-point dependent
    //  constants equal to the largest available floating-point number and
    //  its square root, respectively.

    static constexpr auto dinf   = std::numeric_limits<_Tp>::max();
    static constexpr auto dinfsr = std::sqrt(dinf);

    static constexpr auto dzero  = _Tp(0.0L);
    static constexpr auto d1d4   = _Tp(0.25L);
    static constexpr auto d1d3   = _Tp(0.33333333333333333333L);
    static constexpr auto dhalf  = _Tp(0.5L);
    static constexpr auto d2d3   = _Tp(0.66666666666666633337L);
    static constexpr auto d2pi   = _Tp(6.283185307179586L);
    static constexpr auto dlncon = _Tp(0.2703100720721096L);
    static constexpr auto dsqr2  = _Tp(1.4142135623730950L);
    static constexpr auto d4d3   = _Tp(1.33333333333333333333L);

    static constexpr cmplx zone{1.0L, 0.0L};
    static constexpr cmplx j{0.0L, 1.0L};

    //  Separate real and imaginary parts of zhat
    auto dx = std::real(zhat);
    auto dy = std::imag(zhat);
    auto dxabs = std::abs(dx);
    auto dyabs = std::abs(dy);

    //  If 1 - zhat**2 can be computed without overflow
    if (dxabs <= dinfsr && dyabs <= (dinfsr - 1))
      {
	//  find max and min of abs(dx) and abs(dy)
	auto du = dxabs;
	auto dv = dyabs;
	if (du < dv)
	  std::swap(du, dv);
	if (du >= dhalf && dv > dinf / (2 * du))
	  {
	    //  set completion code - unable to compute 1-zhat**2 and exit
	    ier = 131;
	    return;
	  }
      }
    else
      {
	//  set completion code - unable to compute 1-zhat**2 and exit
	ier = 131;
	return;
      }

    //  compute 1 - zhat**2 and related constants
    auto ztemp = cmplx{1 - (dx - dy) * (dx + dy), -2 * dx * dy};
    ztemp = std::sqrt(ztemp);
    t = _Tp(1) / ztemp;
    tsq = t * t;

    //  if nu**2 can be computed without overflow
    if (std::abs(nu) <= dinfsr)
      {
	nusq = nu * nu;
	z1dnsq = _Tp(1) / nusq;
	//  compute nu**(-2/3), nu**(-4/3), nu**(-1/3)
	znm4d3 = -std::log(nu);
	znm1d3 = std::exp(d1d3 * znm4d3);
	znm2d3 = std::exp(d2d3 * znm4d3);
	znm4d3 = std::exp(d4d3 * znm4d3);
      }
    else
      {
	//  set completion code - unable to compute nu**2
	ier = 132;
	return;
      }

    //  compute xi = ln(1+(1-zhat**2)**(1/2)) - ln(zhat) - (1-zhat**2)**(1/2)
    //  using default branch of logarithm and square root
    auto xi = std::log(zone + ztemp) - std::log(zhat) - ztemp;
    zetm3h = d2d3 / xi;

    //  Compute principal value of ln(xi) and then adjust imaginary part
    auto lnxi = std::log(xi);

    //  Prepare to adjust logarithm of xi to appropriate Riemann sheet
    auto dtemp = dzero;

    //  Find adjustment necessary to get on proper Riemann sheet
    if (dy == dzero)  //  zhat is real
      {
	if (dx > 1)
	  dtemp = d2pi;
      }
    else  //  zhat is not real
      {
	//  if zhat is in upper half-plane
	if (dy > dzero)
	  {
	    //  if xi lies in upper half-plane
	    if (std::imag(xi) > dzero)
	      dtemp = -d2pi;
	    else
	      dtemp = d2pi;
	  }
      }
    std::cout << " > > > dtemp = " << dtemp << '\n';

    //  Adjust logarithm of xi.
    lnxi += dtemp * j;
    std::cout << " > > > lnxi = " << lnxi << '\n';

    //  compute ln(zeta), zeta, zeta^(1/2), zeta^(-1/2)
    auto lnzeta = d2d3 * lnxi + dlncon;
    zeta = std::exp(lnzeta);
    zetaphf = std::sqrt(zeta);
    zetamhf = _Tp(1) / zetaphf;

    //  compute (4 * zeta / (1 - zhat**2))^(1/4)
    ztemp = std::log(ztemp);
    zetrat = dsqr2 * std::exp(d1d4 * lnzeta - dhalf * ztemp);

    return;
  }


/**
    Purpose
      Compute the arguments for the Airy function evaluations
      carefully to prevent premature overflow.  Note that the
      major work here is in safe_div.  A faster, but less safe
      implementation can be obtained without use of safe_div.

    Arguments
    @param[in]  znm2d3  nu**(-2/3).  in our implementation, zmn2d3 is
			output from hankel_params.
    @param[in]  zeta    zeta in the uniform asymptotic expansions.
			In our implementation, zeta is output from dparms.
    @param[out]  zargp  exp(2*pi*i/3) * nu(2/3) * zeta.
    @param[out]  zargm  exp(-2*pi*i/3) * nu(2/3) * zeta.
    @param[out]  ier    A completion code.
			ier = 0 indicates normal completion.
			ier = 133 indicates failure in computing nu(2/3)*zeta
			from the input zmn2d3 and zeta.
 */
template<typename _Tp>
  void
  airy_arg(std::complex<_Tp> nm2d3, std::complex<_Tp> zeta,
	   std::complex<_Tp> & argp, std::complex<_Tp> & argm, int & ier)
  {
    using cmplx = std::complex<_Tp>;

    //  zexpp and zexpm are exp(2*pi*i/3) and its reciprocal, respectively.
    static constexpr auto expp = cmplx{-0.5L,  0.8660254037844386L};
    static constexpr auto expm = cmplx{-0.5L, -0.8660254037844386L};

    std::cout << " > > > airy_arg\n";
    std::cout << " > > > > nm2d3 = " << nm2d3 << '\n';
    std::cout << " > > > > zeta  = " << zeta << '\n';

    ier = 0;

    if (safe_div(zeta, nm2d3, argm))
      {
	std::cout << " > > > > argm = " << argm << '\n';
	argp = expp * argm;
	argm = expm * argm;
      }
    else
      ier = 133;
    std::cout << " > > > > argp = " << argp << '\n';
    std::cout << " > > > > argm = " << argm << '\n';
  }

#endif // HANKEL_TCC
