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
  __hankel(std::complex<_Tp> __nu, std::complex<_Tp> __arg,
	   std::complex<_Tp>& __h1, std::complex<_Tp>& __h2,
	   std::complex<_Tp>& __h1p, std::complex<_Tp>& __h2p,
	   int& __status);

template<typename _Tp>
  void
  __region(std::complex<_Tp> __alpha,
	   int& __indexr, char& __aorb, int& __status);

template<typename _Tp>
  void
  __hankel_debye(std::complex<_Tp> __nu, std::complex<_Tp> __arg,
		 _Tp __alpha, int& __indexr, char& __aorb, int& __morn,
		 std::complex<_Tp>& __h1, std::complex<_Tp>& __h2,
		 std::complex<_Tp>& __h1p, std::complex<_Tp>& __h2p,
		 int& __status);

template<typename _Tp>
  void
  __hankel_uniform_olver(std::complex<_Tp> __nu, std::complex<_Tp> __z,
			 std::complex<_Tp>& __h1, std::complex<_Tp>& __h2,
			 std::complex<_Tp>& __h1p, std::complex<_Tp>& __h2p);

template<typename _Tp>
  void
  __hankel_uniform_outer(std::complex<_Tp> __nu, std::complex<_Tp> __z, _Tp __eps,
			 std::complex<_Tp>& __zhat, std::complex<_Tp>& __1dnsq,
			 std::complex<_Tp>& __nm1d3, std::complex<_Tp>& __nm2d3,
			 std::complex<_Tp>& __t, std::complex<_Tp>& __tsq,
			 std::complex<_Tp>& __etm3h, std::complex<_Tp>& __etrat,
			 std::complex<_Tp>& __aip, std::complex<_Tp>& __o4dp,
			 std::complex<_Tp>& __aim, std::complex<_Tp>& __o4dm,
			 std::complex<_Tp>& __od2p, std::complex<_Tp>& __od0dp,
			 std::complex<_Tp>& __od2m, std::complex<_Tp>& __od0dm,
			 int& __status);

template<typename _Tp>
  void
  __hankel_uniform_sum(std::complex<_Tp> __t, std::complex<_Tp> __tsq,
		       std::complex<_Tp> __1dnusq, std::complex<_Tp> __zetm3h,
		       std::complex<_Tp> __aip, std::complex<_Tp> __zo4dp,
		       std::complex<_Tp> __aim, std::complex<_Tp> __zo4dm,
		       std::complex<_Tp> __zod2p, std::complex<_Tp> __zod0dp,
		       std::complex<_Tp> __zod2m, std::complex<_Tp> __zod0dm,
		       _Tp __eps,
		       std::complex<_Tp>& __h1sum, std::complex<_Tp>& __h1psum,
		       std::complex<_Tp>& __h2sum, std::complex<_Tp>& __h2psum,
		       int& __status);

template<typename _Tp>
  bool
  __safe_div(std::complex<_Tp> __z1, std::complex<_Tp> __z2,
	   std::complex<_Tp>& __z1dz2);

template<typename _Tp>
  void
  __airy_arg(std::complex<_Tp> __num2d3, std::complex<_Tp> __zeta,
	     std::complex<_Tp>& __zargp, std::complex<_Tp>& __zargm,
	     int& __status);

template<typename _Tp>
  void
  __hankel_params(std::complex<_Tp> __nu, std::complex<_Tp> __zhat,
		  std::complex<_Tp>& __t, std::complex<_Tp>& __tsq,
		  std::complex<_Tp>& __nusq, std::complex<_Tp>& __1dnusq,
		  std::complex<_Tp>& __num1d3, std::complex<_Tp>& __num2d3,
		  std::complex<_Tp>& __num4d3, std::complex<_Tp>& __zeta,
		  std::complex<_Tp>& __zetaphf, std::complex<_Tp>& __zetamhf,
		  std::complex<_Tp>& __zetm3h, std::complex<_Tp>& __zetrat,
		  int& __status);

/**
 *
 */
template<typename _Tp>
  void
  __hankel(std::complex<_Tp> __nu, std::complex<_Tp> __z,
	   std::complex<_Tp>& __h1, std::complex<_Tp>& __h2,
	   std::complex<_Tp>& __h1p, std::complex<_Tp>& __h2p,
	   int& __status)
  {
    static constexpr _Tp _S_pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;

    int __indexr, __statusr;
    std::cout << " > hankel:\n";
    std::cout << " > > nu = " << __nu << '\n';
    std::cout << " > > z  = " << __z << '\n';

    __status = 0;

    auto __test = std::abs((__nu - __z) / std::pow(__nu, 1.0/3.0));
    if (__test < 4.0)
      __hankel_uniform(__z, __nu, __h1, __h2, __h1p, __h2p);
    else
      {
	auto __sqtrm = std::sqrt((__nu / __z) * (__nu / __z) - 1);
	auto __alpha = std::log((__nu / __z) + __sqtrm);
	if (std::imag(__alpha) < 0)
	  __alpha = -__alpha;
	auto __alphar = std::real(__alpha);
	auto __alphai = std::imag(__alpha);
	char __aorb;
	if ((std::real(__nu) > std::real(__z))
	 && (std::abs(std::imag(__nu/__z)) <= 0))
	  {
	    __indexr = 0;
	    __aorb = ' ';
	  }
	else
	  __region(__alpha, __indexr, __aorb, __statusr);
	auto __morn = 0;
	if (__aorb == 'A')
	  {
	    auto __mfun = ((__alphar * std::tanh(__alphar) - 1)
			* std::tan(__alphai) + __alphai) / _S_pi;
	    __morn = int(__mfun);
	    if (__mfun < 0 && (std::fmod(__mfun, 1) != 0))
	      --__morn;
	  }
	else if (__aorb == 'B')
	  {
	    auto __nfun = ((1 - __alphar * std::tanh(__alphar))
			* std::tan(__alphai) - __alphai) / _S_pi;
	    __morn = int(__nfun) + 1;
	    if (__nfun < 0 && std::fmod(__nfun, 1) != 0)
	      --__morn;
	  }
	__hankel_debye(__nu, __z, __alpha, __indexr, __aorb, __morn,
		       __h1, __h2, __h1p, __h2p, __status);
      }

    return;
  }

/**
 *  
 */
template<typename _Tp>
  void
  __region(std::complex<_Tp> __alpha, int& __indexr, char& __aorb, int& __status)
  {
    static constexpr _Tp _S_pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;

    __status = 0;
    __aorb = ' ';

    auto __alphar = std::real(__alpha);
    auto __alphai = std::imag(__alpha);

    auto __f1 = _Tp{1}
	      - __alphai * std::cos(__alphai) / std::sin(__alphai)
	      - __alphar * std::sinh(__alphar) / std::cosh(__alphar);

    auto __f2 = _Tp{1}
	      + (_S_pi - __alphai) * std::cos(__alphai) / std::sin(__alphai)
	      - __alphar * std::sinh(__alphar) / std::cosh(__alphar);

    if (__f1 > _Tp{0} && __f2 > _Tp{0})
      __indexr = 1;
    else if (__f2 > _Tp{0})
      {
	if (__alphar > _Tp{0})
	  __indexr = 2;
	else
	  __indexr = 3;
      }
    else if (__f1 > _Tp{0})
      {
	if (__alphar > _Tp{0})
	  __indexr = 4;
	else
	  __indexr = 5;
      }
    else
      {
	if (__alphar > _Tp{0})
          __indexr = 6;
	else
          __indexr = 7;
        if (__alphai <= (_S_pi / _Tp{2}))
          __aorb = 'A';
        else
          __aorb = 'B';
      }
    return;
  }

/**
 *
 */
template<typename _Tp>
  void
  hankel_debye(std::complex<_Tp> __nu, std::complex<_Tp> __z,
	       std::complex<_Tp> __alpha,
	       int __indexr, char& __aorb, int& __morn,
	       std::complex<_Tp>& __h1, std::complex<_Tp>& __h2,
	       std::complex<_Tp>& __h1p, std::complex<_Tp>& __h2p,
	       int& __status)
  {
    using namespace std::literals::complex_literals;
    using __cmplx = std::complex<_Tp>;

    static constexpr _Tp
      _S_pi  = 3.141592653589793238462643383279502884195e+0L;
    static constexpr __cmplx _S_j = 1.0il;
    static constexpr _Tp _S_toler = 1.0e-8;
    const auto __maxexp
      = std::floor(std::numeric_limits<_Tp>::max_exponent
		 * std::log(std::numeric_limits<_Tp>::radix));
    std::cout << " > hankel_debye:\n";
    std::cout << " > > nu = " << __nu << '\n';
    std::cout << " > > z  = " << __z << '\n';

    auto __alphar = std::real(__alpha);
    auto __alphai = std::imag(__alpha);
    auto __thalpa = std::sinh(__alpha) / std::cosh(__alpha);
    auto __snhalp = std::sinh(__alpha);
    auto __denom = std::sqrt(_S_pi * __z / 2)
		 * std::sqrt(-_S_j * std::sinh(__alpha));
    if (std::abs(std::real(__nu * (__thalpa - __alpha))) > __maxexp)
      {
	__status = 1;
	return;
      }
    auto __s1 = std::exp(+__nu * (__thalpa - __alpha) - _S_j * _S_pi / _Tp(4))
	      / __denom;
    auto __s2 = std::exp(-__nu * (__thalpa - __alpha) + _S_j * _S_pi / _Tp(4))
	      / __denom;
    auto __exparg = __nu * (__thalpa - __alpha) - _S_j * _S_pi / _Tp(4);
    if (__indexr == 0)
      {
	__h1 = 0.5 * __s1 - __s2;
	__h2 = 0.5 * __s1 + __s2;
	__h1p = __snhalp * (0.5 * __s1 + __s2);
	__h2p = __snhalp * (0.5 * __s1 - __s2);
      }
    else if (__indexr == 1)
      {
	__h1 = __s1;
	__h2 = __s2;
	__h1p = +__snhalp * __s1;
	__h2p = -__snhalp * __s2;
      }
    else if (__indexr == 2)
      {
	auto __jdbye = __s1 / 2;
	__h2 = __s2;
	__h1 = 2 * __jdbye - __h2;
	__h1p = +__snhalp * (__s1 + __s2);
	__h2p = -__snhalp * __s2;
      }
    else if (__indexr == 3)
      {
	__h1 = __s1;
	__h2 = __s2 - __s1;
	__h1p = +__snhalp * __s1;
	__h2p = -__snhalp * (__s1 + __s2);
      }
    else if (__indexr == 4)
      {
	__h1 = __s1;
	__h2 = __s2 - std::exp(+_Tp(2) * _S_j * __nu * _S_pi) * __s1;
	__h1p = +__snhalp * __s1;
	__h2p = -__snhalp
	      * (__s2 + std::exp(+_Tp(2) * _S_j * __nu * _S_pi) * __s1);
      }
    else if (__indexr == 5)
      {
	__h1 = __s1 - std::exp(-_Tp(2) * _S_j * __nu * _S_pi) * __s2;
	__h2 = __s2;
	__h1p = +__snhalp
	      * (__s1 + std::exp(-_Tp(2) * _S_j * __nu * _S_pi) * __s2);
	__h2p = -__snhalp * __s2;
      }
    else if (__aorb == 'A')
      {
	__cmplx __sinrat;
	if ((std::abs(std::imag(__nu)) < _S_toler)
	 && (std::abs(std::fmod(std::real(__nu), 1)) < _S_toler))
	  __sinrat = __morn;
	else
	  __sinrat = std::sin(__morn * __nu * _S_pi)
		  / std::sin(__nu * _S_pi);
	if (__indexr == 6)
	  {
	    __h2 = __s2
		 - std::exp(_S_j * (__morn + 1) * __nu * _S_pi)
		 * __sinrat * __s1;
	    __h1 = __s1 - __h2;
	    __h2p = -__snhalp
		  * (__s2 + std::exp(_S_j * (__morn + 1) * __nu * _S_pi)
			   * __sinrat * __s1);
	    __h1p = +__snhalp
		  * ((1 + std::exp(_S_j * (__morn + 1) * __nu * _S_pi)
			* __sinrat) * __s1 + __s2);
	  }
	else if (__indexr == 7)
	  {
	    __h1 = __s1
		 - std::exp(-_S_j * (__morn + 1) * __nu * _S_pi)
		  * __sinrat * __s2;
	    __h2 = __s2 - __h1;
	    __h1p = +__snhalp
		  * (__s1 + std::exp(-_S_j * (__morn + 1) * __nu * _S_pi)
			   * __sinrat * __s2);
	    __h2p = -__snhalp
		   * ((1 + std::exp(-_S_j * (__morn + 1) * __nu * _S_pi)
			 * __sinrat) * __s2 + __s1);
	  }
	else
	  __status = 1;
      }
    else
      {
	__cmplx __sinrat;
	if ((std::abs(std::imag(__nu)) < _S_toler)
	 && (std::abs(std::fmod(dreal(__nu), 1)) < _S_toler))
	  __sinrat = -__morn;
	else
	  __sinrat = std::sin(__morn * __nu * _S_pi) / std::sin(__nu * _S_pi);
	if (__indexr == 6)
	  {
	    __h1 = __s1 - std::exp(_S_j * (__morn - 1) * __nu * _S_pi)
		 * __sinrat * __s2;
	    __h2 = __s2 - std::exp(2 * _S_j * __nu * _S_pi) * __h2;
	    __h1p = +__snhalp
		  * (__s1 + std::exp(_S_j * (__morn - 1) * __nu * _S_pi)
			  * __sinrat * __s2);
	    __h2p = -__snhalp
		  * ((1 + std::exp(_S_j * (__morn + 1) * __nu * _S_pi)
			* __sinrat) * __s2
		    + std::exp(2 * _S_j * __nu * _S_pi) * __s1);
	  }
	else if (__indexr == 7)
	  {
	    __h2 = __s2
		 - std::exp(-_S_j * (__morn - 1) * __nu * _S_pi)
		 * __sinrat * __s1;
	    __h1 = __s1 - std::exp(-2 * _S_j * __nu * _S_pi) * __h2;
	    __h2p = -__snhalp
		  * (__s2 + std::exp(-_S_j * (__morn - 1) * __nu * _S_pi)
			  * __sinrat * __s1);
	    __h1p = +__snhalp
		  * ((1 + std::exp(-_S_j * (__morn + 1) * __nu * _S_pi)
				  * __sinrat) * __s1
			      + std::exp(-2 * _S_j * __nu * _S_pi) * __s2);
	  }
	else
	  __status = 1;
      }

    return;
  }

/**
 *  @brief
 *      This routine computes the uniform asymptotic approximations
 *      of the Hankel functions and their derivatives including a patch
 *      for the case when the order equals or nearly equals the argument.
 *      At such points, Olver's expressions have zero denominators (and
 *      numerators) resulting in numerical problems.  This routine
 *      averages results from four surrounding points in the complex plane
 *      to obtain the result in such cases.
 *
 *  @param[in]  nu  The order for which the Hankel functions are evaluated.
 *  @param[in]  z   The argument at which the Hankel functions are evaluated.
 *  @param[out] h1  The Hankel function of the first kind.
 *  @param[out] h1p The derivative of the Hankel function of the first kind.
 *  @param[out] h2  The Hankel function of the second kind.
 *  @param[out] h2p The derivative of the Hankel function of the second kind.
 */
template<typename _Tp>
  void
  __hankel_uniform(std::complex<_Tp> __nu, std::complex<_Tp> __z,
		   std::complex<_Tp>& __h1, std::complex<_Tp>& __h2,
		   std::complex<_Tp>& __h1p, std::complex<_Tp>& __h2p)
  {
    using __cmplx = std::complex<_Tp>;
    _Tp __test = std::pow(std::abs(__nu), _Tp(1) / _Tp(3)) / _Tp(5);
    std::cout << " > hankel_uniform:\n";
    std::cout << " > > nu = " << __nu << '\n';
    std::cout << " > > z  = " << __z << '\n';

    if (std::abs(__z - __nu) > __test)
      __hankel_uniform_olver(__nu, __z, __h1, __h2, __h1p, __h2p);
    else
      {
	_Tp __r = 2 * __test;
	std::complex<_Tp> _S_anu[4]{__nu + __cmplx{__r, _Tp()},
				   __nu + __cmplx{_Tp(), __r},
				   __nu - __cmplx{__r, _Tp()},
				   __nu - __cmplx{_Tp(), __r}};

	__h1  = __cmplx{};
	__h2  = __cmplx{};
	__h1p = __cmplx{};
	__h2p = __cmplx{};
	for (auto __tnu : _S_anu)
	  {
	    std::complex<_Tp> __th1, __th2, __th1p, __th2p;
	    __hankel_uniform_olver(__tnu, __z, __th1, __th2, __th1p, __th2p);
	    __h1  += __th1;
	    __h2  += __th2;
	    __h1p += __th1p;
	    __h2p += __th2p;
	  }
	__h1  /= _Tp(4);
	__h2  /= _Tp(4);
	__h1p /= _Tp(4);
	__h2p /= _Tp(4);
      }

    return;
  }


/**
 *  @brief Compute approximate values for the Hankel functions
 *         of the first and second kinds using Olver's uniform asymptotic
 *         expansion to of order @c nu along with their derivatives.
 *
 *  @param[in]  nu  The order for which the Hankel functions are evaluated.
 *  @param[in]  z   The argument at which the Hankel functions are evaluated.
 *  @param[out] h1  The Hankel function of the first kind.
 *  @param[out] h1p The derivative of the Hankel function of the first kind.
 *  @param[out] h2  The Hankel function of the second kind.
 *  @param[out] h2p The derivative of the Hankel function of the second kind.
 */
template<typename _Tp>
  void
  __hankel_uniform_olver(std::complex<_Tp> __nu, std::complex<_Tp> __z,
			 std::complex<_Tp>& __h1, std::complex<_Tp>& __h2,
			 std::complex<_Tp>& __h1p, std::complex<_Tp>& __h2p)
  {
    using namespace std::literals::complex_literals;
    using __cmplx = std::complex<_Tp>;
    std::cout << " > hankel_uniform_olver:\n";
    std::cout << " > > nu = " << __nu << '\n';
    std::cout << " > > z  = " << __z << '\n';

    static constexpr _Tp _S_pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;
    //static constexpr _Tp _S_pi_3 = 1.0471'97551'19659'77461'54214'46109'31676'28063e+0L;
    static constexpr __cmplx __con1p{ 1.0, 1.732050807568877}; // 2*exp( pi*j/3)
    static constexpr __cmplx __con1m{ 1.0,-1.732050807568877}; // 2*exp(-pi*j/3)
    static constexpr __cmplx __con2p{-2.0, 3.464101615137755}; // 4*exp( 2*pi*j/3)
    static constexpr __cmplx __con2m{-2.0,-3.464101615137755}; // 4*exp(-2*pi*j/3)
    static constexpr __cmplx _S_j{1il};
    static constexpr _Tp __eps   = 1.0e-06;
    static constexpr _Tp __epsai = 1.0e-12;

    //  Extended to accommodate negative real orders.
    bool __nuswitch = false;
    if (std::real(__nu) < _Tp(0))
      {
	__nuswitch = true;
	__nu = -__nu;
      }

    // Compute outer factors in the uniform asymptotic expansions
    // for the Hankel functions and their derivatives along with
    // other important functions of nu and z.
    __cmplx __t, __tsq,
	  __1dnsq, __etm3h, __aip, __o4dp, __aim, __o4dm,
	  __od2p, __od0dp, __od0dm, __tmp, __zhat, __nm1d3,
	  __nm2d3, __etrat, __od2m, __r_factor;
    int __status;
    __hankel_uniform_outer(__nu, __z, __epsai, __zhat, __1dnsq, __nm1d3,
			   __nm2d3, __t, __tsq, __etm3h, __etrat,
			   __aip, __o4dp, __aim, __o4dm, __od2p,
			   __od0dp, __od2m, __od0dm, __status);

    // Check for successful completion
    if (__status == 0)
      {
	// Compute further terms in the expansions in their appropriate linear combinations.

	__hankel_uniform_sum(__t, __tsq, __1dnsq, __etm3h,
			     __aip, __o4dp, __aim, __o4dm,
			     __od2p, __od0dp, __od2m, __od0dm, __eps,
			     __h1, __h1p, __h2, __h2p, __status);

	// Assemble approximations.
	__tmp = __etrat * __nm1d3;
	__h1 = __con1m * __tmp * __h1;
	__h2 = __con1p * __tmp * __h2;
	__tmp = __nm2d3 / (__zhat * __etrat);
	__h1p = __con2p * __tmp * __h1p;
	__h2p = __con2m * __tmp * __h2p;

	// Check for successful completion
	if (__status != 0)
	  {
	    std::cerr << "__hankel_uniform_sum: "
		      << "  status = " << __status << '\n'
		      << "  nu  = " << __nu << '\n'
		      << "  z   = " << __z << '\n';
	  }
      }
    else
      std::cerr << "__hankel_uniform_outer: "
		<< "  status = " << __status << '\n'
		<< "  nu  = " << __nu << '\n'
		<< "  z   = " << __z << '\n';

    if (__nuswitch)
      {
	__r_factor = std::exp(_S_j * __nu * _S_pi);
	__h1  *= __r_factor;
	__h1p *= __r_factor;
	__h2  /= __r_factor;
	__h2p /= __r_factor;
	__nu  = -__nu;
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
  __hankel_uniform_outer(std::complex<_Tp> __nu, std::complex<_Tp> __z, _Tp __eps,
			 std::complex<_Tp>& __zhat, std::complex<_Tp>& __1dnsq,
			 std::complex<_Tp>& __nm1d3, std::complex<_Tp>& __nm2d3,
			 std::complex<_Tp>& __t, std::complex<_Tp>& __tsq,
			 std::complex<_Tp>& __etm3h, std::complex<_Tp>& __etrat,
			 std::complex<_Tp>& __aip, std::complex<_Tp>& __o4dp,
			 std::complex<_Tp>& __aim, std::complex<_Tp>& __o4dm,
			 std::complex<_Tp>& __od2p, std::complex<_Tp>& __od0dp,
			 std::complex<_Tp>& __od2m, std::complex<_Tp>& __od0dm,
			 int& __status)
  {
    using __cmplx = std::complex<_Tp>;

    static constexpr __cmplx __e2pd3{-0.5L,  0.8660254037844386L};
    static constexpr __cmplx __d2pd3{-0.5L, -0.8660254037844386L};
    std::cout << " > hankel_uniform_outer:\n";
    std::cout << " > > nu = " << __nu << '\n';
    std::cout << " > > z  = " << __z << '\n';

    __status = 0;

    if (__safe_div(__z, __nu, __zhat))
      {
	//  Try to compute other nu and z dependent parameters except args to Airy functions
	__cmplx __nm4d3, __nusq, __zeta, __etphf, __etmhf;
	__hankel_params(__nu, __zhat, __t, __tsq, __nusq, __1dnsq, __nm1d3, __nm2d3, __nm4d3,
			__zeta, __etphf, __etmhf, __etm3h, __etrat, __status);

        std::cout.precision(std::numeric_limits<double>::max_digits10);
        //std::cout << std::scientific;
        std::cout << " > > t      = " << __t << '\n';
        std::cout << " > > tsq    = " << __tsq << '\n';
        std::cout << " > > nusq   = " << __nusq << '\n';
        std::cout << " > > _1dnsq = " << __1dnsq << '\n';
        std::cout << " > > nm1d3  = " << __nm1d3 << '\n';
        std::cout << " > > nm2d3  = " << __nm2d3 << '\n';
        std::cout << " > > nm4d3  = " << __nm4d3 << '\n';
        std::cout << " > > zeta   = " << __zeta << '\n';
        std::cout << " > > etphf  = " << __etphf << '\n';
        std::cout << " > > etmhf  = " << __etmhf << '\n';
        std::cout << " > > etm3h  = " << __etm3h << '\n';
        std::cout << " > > etrat  = " << __etrat << '\n';
        std::cout << " > > status = " << __status << '\n';

	if (__status == 0)
	  {
	    //  Try to compute Airy function arguments
	    __cmplx __argp, __argm;
	    __airy_arg(__nm2d3, __zeta, __argp, __argm, __status);
	    std::cout << " > > nm2d3 = " << __nm2d3 << '\n';
	    std::cout << " > > zeta  = " << __zeta << '\n';
	    std::cout << " > > argp  = " << __argp << '\n';
	    std::cout << " > > argm  = " << __argm << '\n';
	    std::cout << " > > status = " << __status << '\n';

	    if (__status == 0)
	      {
		//  Compute Airy functions and derivatives
		__cmplx __aipp, __aimp;
		int __statusp, __statusm;
		__airy(__argp, __eps, __aip, __aipp, __statusp);
		std::cout << " > > argp    = " << __argp << '\n';
		std::cout << " > > aip     = " << __aip << '\n';
		std::cout << " > > aipp    = " << __aipp << '\n';
		std::cout << " > > statusp = " << __statusp << '\n';
		__airy(__argm, __eps, __aim, __aimp, __statusm);
		std::cout << " > > argm    = " << __argm << '\n';
		std::cout << " > > aim     = " << __aim << '\n';
		std::cout << " > > aimp    = " << __aimp << '\n';
		std::cout << " > > statusm = " << __statusm << '\n';
		if (__statusp == 0 && __statusm == 0)
		  {
		    //  Compute partial outer terms in expansions
		    __o4dp = -__etmhf * __nm4d3 * __e2pd3 * __aipp;
		    __o4dm = -__etmhf * __nm4d3 * __d2pd3 * __aimp;
		    __od2p = -__etphf * __nm2d3 * __aip;
		    __od0dp = __e2pd3 * __aipp;
		    __od2m = -__etphf * __nm2d3 * __aim;
		    __od0dm = __d2pd3 * __aimp;
		  }
		else  //  Error in evaluation of Airy functions
		  __status = 134;
	      }
	    else  //  Airy function args not computable
	      __status = 133;
	  }
	else  //  Factors not successfully computed
	  __status = 135;
      }
    else //  z/nu not successfully computed
      __status = 130;

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
  __hankel_uniform_sum(std::complex<_Tp> __t, std::complex<_Tp> __tsq,
		     std::complex<_Tp> __1dnusq, std::complex<_Tp> __zetm3h,
		     std::complex<_Tp> __aip, std::complex<_Tp> __zo4dp,
		     std::complex<_Tp> __aim, std::complex<_Tp> __zo4dm,
		     std::complex<_Tp> __zod2p, std::complex<_Tp> __zod0dp,
		     std::complex<_Tp> __zod2m, std::complex<_Tp> __zod0dm,
		     _Tp __eps,
		     std::complex<_Tp>& __h1sum, std::complex<_Tp>& __h1psum,
		     std::complex<_Tp>& __h2sum, std::complex<_Tp>& __h2psum,
		     int& __status)
  {
    using __cmplx = std::complex<_Tp>;

    bool __coverged;
    int __nterms = 4;

    static constexpr auto __zone = __cmplx{1, 0};
    std::cout << " > hankel_uniform_sum:\n";
    std::cout << " > t     = " << __t << '\n';
    std::cout << " > tsq   = " << __tsq << '\n';
    std::cout << " > z1dnsq = " << __1dnusq << '\n';
    std::cout << " > zetm3h = " << __zetm3h << '\n';
    std::cout << " > aip    = " << __aip << '\n';
    std::cout << " > zo4dp  = " << __zo4dp << '\n';
    std::cout << " > aim    = " << __aim << '\n';
    std::cout << " > zo4dm  = " << __zo4dm << '\n';
    std::cout << " > zod2p  = " << __zod2p << '\n';
    std::cout << " > zod0dp = " << __zod0dp << '\n';
    std::cout << " > zod2m  = " << __zod2m << '\n';
    std::cout << " > zod0dm = " << __zod0dm << '\n';

    //  Coefficients for u and v polynomials appearing in Olver's
    //  uniform asymptotic expansions for the Hankel functions
    //  and their derivatives

    static constexpr _Tp
    _S_a[66]
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
    _S_b[66]
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
    _S_lambda[21]
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
    _S_mu[21]
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

    std::vector<__cmplx> __u;
    __u.reserve(100);
    std::vector<__cmplx> __v;
    __v.reserve(100);


    __status = 0;
    //  Initialize modified Horner's rule evaluation of u and v polynomials.
    auto __xtsq = std::real(__tsq);
    auto __ytsq = std::imag(__tsq);
    auto __ytsq2 = __ytsq * __ytsq;
    auto __dr = 2 * __xtsq;
    auto __ds = std::norm(__tsq);

    //  Compute u_0,1,2 and v_0,1,2 and store for later use
    auto __tk = __t;
    __u.push_back(__tk * (_S_a[1] * __tsq + _S_a[2]));
    __v.push_back(__tk * (_S_b[1] * __tsq + _S_b[2]));
    std::cout << " > > tk   = " << __tk << '\n';
    std::cout << " > > u[0] = " << __u.back() << '\n';
    std::cout << " > > v[0] = " << __v.back() << '\n';
    __tk *= __t;
    __u.push_back(__tk * __cmplx((_S_a[3] * __xtsq + _S_a[4])
				 * __xtsq + _S_a[5] - _S_a[3] * __ytsq2,
		      (2 * _S_a[3] * __xtsq + _S_a[4]) * __ytsq));
    __v.push_back(__tk * __cmplx((_S_b[3] * __xtsq + _S_b[4])
				 * __xtsq + _S_b[5] - _S_b[3] * __ytsq2,
		      (2 * _S_b[3] * __xtsq + _S_b[4]) * __ytsq));
    std::cout << " > > tk   = " << __tk << '\n';
    std::cout << " > > u[1] = " << __u.back() << '\n';
    std::cout << " > > v[1] = " << __v.back() << '\n';
    __tk *= __t;
    __u.push_back(__tk * __cmplx(((_S_a[6] * __xtsq + _S_a[7])
				 * __xtsq + _S_a[8]) * __xtsq
     		      + _S_a[9] - (3 * _S_a[6] * __xtsq + _S_a[7]) * __ytsq2,
     		      ((3 * _S_a[6] * __xtsq + 2 * _S_a[7]) * __xtsq + _S_a[8]
     		      - _S_a[6] * __ytsq2) * __ytsq));
    __v.push_back(__tk * __cmplx(((_S_b[6] * __xtsq + _S_b[7])
				 * __xtsq + _S_b[8]) * __xtsq
     		      + _S_b[9] - (3 * _S_b[6] * __xtsq + _S_b[7]) * __ytsq2,
     		      ((3 * _S_b[6] * __xtsq + 2 * _S_b[7]) * __xtsq + _S_b[8]
     		      - _S_b[6] * __ytsq2) * __ytsq));
    std::cout << " > > tk   = " << __tk << '\n';
    std::cout << " > > u[2] = " << __u.back() << '\n';
    std::cout << " > > v[2] = " << __v.back() << '\n';

    //  Compute A_0,1, B_0,1, C_0,1, D_0,1 ... note that
    //  B_k and C_k are computed up to -zeta^(-1/2) -zeta^(1/2) factors,
    //  respectively.  These recurring factors are included as appropriate
    //  in the outer factors, thus saving repeated multiplications by them.
    auto __A0 = __zone;
    auto __A1 = __u[1]
	    + __zetm3h * (_S_mu[1] * __zetm3h + _S_mu[0] * __u[0]);
    auto __B0 = __u[0] + _S_lambda[0] * __zetm3h;
    auto __B1 = __u[2] + __zetm3h * (__zetm3h * (_S_lambda[2] * __zetm3h
				       + _S_lambda[1] * __u[0])
		   + _S_lambda[0] * __u[1]);
    auto __C0 = __v[0] + _S_mu[0] * __zetm3h;
    auto __C1 = __v[2] + __zetm3h * (__zetm3h * (_S_mu[2] * __zetm3h
				       + _S_mu[1] * __v[0])
		   + _S_mu[0] * __v[1]);
    auto __D0 = __zone;
    auto __D1 = __v[1] + __zetm3h * (_S_lambda[1] * __zetm3h
		      + _S_lambda[0] * __v[0]);
    std::cout << " > > a0 = " << __A0 << '\n';
    std::cout << " > > a1 = " << __A1 << '\n';
    std::cout << " > > b0 = " << __B0 << '\n';
    std::cout << " > > b1 = " << __B1 << '\n';
    std::cout << " > > c0 = " << __C0 << '\n';
    std::cout << " > > c1 = " << __C1 << '\n';
    std::cout << " > > d0 = " << __D0 << '\n';
    std::cout << " > > d1 = " << __D1 << '\n';

    //  Compute terms
    auto __aterm = __A1 * __1dnusq;
    auto __bterm = __B1 * __1dnusq;
    auto __cterm = __C1 * __1dnusq;
    auto __dterm = __D1 * __1dnusq;
    //  Compute sum of first two terms, thus initializing Kahan summing scheme
    auto __asum = __A0 + __aterm;
    auto __atemp = __aterm - (__asum - __A0);
    auto __bsum = __B0 + __bterm;
    auto __btemp = __bterm - (__bsum - __B0);
    auto __csum = __C0 + __cterm;
    auto __ctemp = __cterm - (__csum - __C0);
    auto __dsum = __D0 + __dterm;
    auto __dtemp = __dterm - (__dsum - __D0);
    std::cout << " > > asum = " << __asum << '\n';
    std::cout << " > > bsum = " << __bsum << '\n';
    std::cout << " > > csum = " << __csum << '\n';
    std::cout << " > > dsum = " << __dsum << '\n';

    //  Set convergence flag to no convergence indication
    __coverged = false;

    //  Combine sums in form appearing in expansions
    __h1sum = __aip * __asum + __zo4dp * __bsum;
    __h2sum = __aim * __asum + __zo4dm * __bsum;
    __h1psum = __zod2p * __csum + __zod0dp * __dsum;
    __h2psum = __zod2m * __csum + __zod0dm * __dsum;
    auto __h1save = __aip + __zo4dp * __B0;
    auto __h2save = __aim + __zo4dm * __B0;
    auto __h1psave = __zod2p * __C0 + __zod0dp;
    auto __h2psave = __zod2m * __C0 + __zod0dm;
    std::cout << " > > h1sum   = " << __h1sum << '\n';
    std::cout << " > > h2sum   = " << __h2sum << '\n';
    std::cout << " > > h1psum  = " << __h1psum << '\n';
    std::cout << " > > h2psum  = " << __h2psum << '\n';
    std::cout << " > > h1save  = " << __h1save << '\n';
    std::cout << " > > h2save  = " << __h2save << '\n';
    std::cout << " > > h1psave = " << __h1psave << '\n';
    std::cout << " > > h2psave = " << __h2psave << '\n';

    //  If convergence criteria now satisfied
    if (__norm_L1(__h1sum - __h1save) < __eps * __norm_L1(__h1sum)
     && __norm_L1(__h2sum - __h2save) < __eps * __norm_L1(__h2sum)
     && __norm_L1(__h1psum - __h1psave) < __eps * __norm_L1(__h1psum)
     && __norm_L1(__h2psum - __h2psave) < __eps * __norm_L1(__h2psum))
      __coverged = true;

    //  Save current sums combined as in expansion for next convergence test
    __h1save = __h1sum;
    __h2save = __h2sum;
    __h1psave = __h1psum;
    __h2psave = __h2psum;

    //  Maintain index into u_k and v_k coefficients
    auto __index = 10;
    auto __indexp = 15;
    //  Maintain power of nu^(-2)
    auto __z1dn2k = __1dnusq;

    //  Loop until convergence criteria satisfied or maximum number of terms reached
    for (auto __k = 2; __k <= __nterms; ++__k)
      {
	//  Initialize for evaluation of two new u and v polynomials
	//  via Horner's rule modified for complex arguments and real coefficients
	auto __indexend = __indexp;
	auto __ukta = _S_a[__index];
	auto __vkta = _S_b[__index];
	++__index;
	auto __uktb = _S_a[__index];
	auto __vktb = _S_b[__index];
	++__index;
	auto __ukpta = _S_a[__indexp];
	auto __vkpta = _S_b[__indexp];
	++__indexp;
	auto __ukptb = _S_a[__indexp];
	auto __vkptb = _S_b[__indexp];
	++__indexp;
	std::cout << " > > > index  = " << __index << '\n';
	std::cout << " > > > indexp = " << __indexp << '\n';
	std::cout << " > > > ukta   = " << __ukta << '\n';
	std::cout << " > > > vkta   = " << __vkta << '\n';
	std::cout << " > > > uktb   = " << __uktb << '\n';
	std::cout << " > > > vktb   = " << __vktb << '\n';
	std::cout << " > > > ukpta  = " << __ukpta << '\n';
	std::cout << " > > > vkpta  = " << __vkpta << '\n';
	std::cout << " > > > ukptb  = " << __ukptb << '\n';
	std::cout << " > > > vkptb  = " << __vkptb << '\n';

	//  Loop until quantities to evaluate lowest order u and v 
	//  polynomials and partial quantities to evaluate
	//  next highest order polynomials computed
	for (; __index < __indexend; ++__index, ++__indexp)
	  {
	    auto __term = __ds * __ukta;
	    __ukta = __uktb + __dr * __ukta;
	    __uktb = _S_a[__index] - __term;
	    __term = __ds * __vkta;
	    __vkta = __vktb + __dr * __vkta;
	    __vktb = _S_b[__index] - __term;

	    __term = __ds * __ukpta;
	    __ukpta = __ukptb + __dr * __ukpta;
	    __ukptb = _S_a[__indexp] - __term;
	    __term = __ds * __vkpta;
	    __vkpta = __vkptb + __dr * __vkpta;
	    __vkptb = _S_b[__indexp] - __term;
	    std::cout << " > > > > index     = " << __index << '\n';
	    std::cout << " > > > > a[index]  = " << _S_a[__index] << '\n';
	    std::cout << " > > > > b[index]  = " << _S_b[__index] << '\n';
	    std::cout << " > > > > indexp    = " << __indexp << '\n';
	    std::cout << " > > > > a[indexp] = " << _S_a[__indexp] << '\n';
	    std::cout << " > > > > b[indexp] = " << _S_b[__indexp] << '\n';
	    std::cout << " > > > > ukta      = " << __ukta << '\n';
	    std::cout << " > > > > vkta      = " << __vkta << '\n';
	    std::cout << " > > > > uktb      = " << __uktb << '\n';
	    std::cout << " > > > > vktb      = " << __vktb << '\n';
	    std::cout << " > > > > ukpta     = " << __ukpta << '\n';
	    std::cout << " > > > > vkpta     = " << __vkpta << '\n';
	    std::cout << " > > > > ukptb     = " << __ukptb << '\n';
	    std::cout << " > > > > vkptb     = " << __vkptb << '\n';
	  }

	//  Update indices into coefficients to reflect initialization
	//++index;

	//  One more iteration for highest order polynomials
	auto __term = __ds * __ukpta;
	__ukpta = __ukptb + __dr * __ukpta;
	__ukptb = _S_a[__indexp] - __term;
	__term = __ds * __vkpta;
	__vkpta = __vkptb + __dr * __vkpta;
	__vkptb = _S_b[__indexp] - __term;
	std::cout << " > > > indexp = " << __indexp << '\n';
	std::cout << " > > > ukpta  = " << __ukpta << '\n';
	std::cout << " > > > ukptb  = " << __ukptb << '\n';
	std::cout << " > > > vkpta  = " << __vkpta << '\n';
	std::cout << " > > > vkptb  = " << __vkptb << '\n';
	++__indexp;

	//  Post multiply and form new polynomials
	std::cout << " > > > nduv    = " << __u.size() << '\n';
	__tk *= __t;
	__u.push_back(__tk * (__ukta * __tsq + __uktb));
	__v.push_back(__tk * (__vkta * __tsq + __vktb));
	std::cout << " > > > u[nduv] = " << __u.back() << '\n';
	std::cout << " > > > v[nduv] = " << __v.back()] << '\n';

	std::cout << " > > > nduv    = " << __u.size() << '\n';
	__tk *= __t;
	__u.push_back(__tk * (__ukpta * __tsq + __ukptb));
	__v.push_back(__tk * (__vkpta * __tsq + __vkptb));
	std::cout << " > > > u[nduv] = " << __u.back() << '\n';
	std::cout << " > > > v[nduv] = " << __v.back() << '\n';

	//  Update indices in preparation for next iteration
	__index = __indexp;
	auto __i2k = 2 * __k - 1;
	auto __i2km1 = __i2k - 1;
	auto __i2kp1 = __i2k + 1;
	__indexp += __i2kp1 + 3;
	std::cout << " > > > index  = " << __index << '\n';
	std::cout << " > > > indexp = " << __indexp << '\n';
	std::cout << " > > > i2k    = " << __i2k << '\n';
	std::cout << " > > > i2km1  = " << __i2km1 << '\n';
	std::cout << " > > > i2kp1  = " << __i2kp1 << '\n';

	//  Start Horner's rule evaluation of a, b, c, and d polynomials.
	__A1 = _S_mu[__i2k] * __zetm3h + _S_mu[__i2km1] * __u[0];
	__D1 = _S_lambda[__i2k] * __zetm3h + _S_lambda[__i2km1] * __v[0];
	__B1 = _S_lambda[__i2kp1] * __zetm3h + _S_lambda[__i2k] * __u[0];
	__C1 = _S_mu[__i2kp1] * __zetm3h + _S_mu[__i2k] * __v[0];
	std::cout << " > > > a1 = " << __A1 << '\n';
	std::cout << " > > > b1 = " << __B1 << '\n';
	std::cout << " > > > c1 = " << __C1 << '\n';
	std::cout << " > > > d1 = " << __D1 << '\n';

	//  Do partial Horner's rule evaluations of a, b, c, and d.
	for(auto __l = 1; __l <= __i2km1; ++__l)
	  {
	    std::cout << " > > > > l    = " << __l << '\n';
	    auto __i2kl = __i2km1 - __l;
	    std::cout << " > > > > i2kl = " << __i2kl << '\n';
	    __A1 = __A1 * __zetm3h + _S_mu[__i2kl] * __u[__l];
	    __D1 = __D1 * __zetm3h + _S_lambda[__i2kl] * __v[__l];
	    __i2kl = __i2k - __l;
	    std::cout << " > > > > i2kl = " << __i2kl << '\n';
	    __B1 = __B1 * __zetm3h + _S_lambda[__i2kl] * __u[__l];
	    __C1 = __C1 * __zetm3h + _S_mu[__i2kl] * __v[__l];
	    std::cout << " > > > > a1   = " << __A1 << '\n';
	    std::cout << " > > > > b1   = " << __B1 << '\n';
	    std::cout << " > > > > c1   = " << __C1 << '\n';
	    std::cout << " > > > > d1   = " << __D1 << '\n';
	  }

	//  Complete the evaluations of a, b, c, and d.
	__A1 = __A1 * __zetm3h + __u[__i2k];
	__D1 = __D1 * __zetm3h + __v[__i2k];
	__B1 = __zetm3h
	     * (__B1 * __zetm3h + _S_lambda[0] * __u[__i2k]) + __u[__i2kp1];
	__C1 = __zetm3h
	     * (__C1 * __zetm3h + _S_mu[0] * __v[__i2k]) + __v[__i2kp1];
	std::cout << " > > > i2k   = " << __i2k << '\n';
	std::cout << " > > > i2kp1 = " << __i2kp1 << '\n';
	std::cout << " > > > a1    = " << __A1 << '\n';
	std::cout << " > > > b1    = " << __B1 << '\n';
	std::cout << " > > > c1    = " << __C1 << '\n';
	std::cout << " > > > d1    = " << __D1 << '\n';

	//  Evaluate new terms for sums.
	__z1dn2k *= __1dnusq;
	__aterm = __A1 * __z1dn2k + __atemp;
	__bterm = __B1 * __z1dn2k + __btemp;
	__cterm = __C1 * __z1dn2k + __ctemp;
	__dterm = __D1 * __z1dn2k + __dtemp;
	std::cout << " > > > aterm = " << __aterm << '\n';
	std::cout << " > > > bterm = " << __bterm << '\n';
	std::cout << " > > > cterm = " << __cterm << '\n';
	std::cout << " > > > dterm = " << __dterm << '\n';

	//  Update sums using Kahan summing scheme.
	__atemp = __asum;
	__asum += __aterm;
	__atemp = __aterm - (__asum - __atemp);
	__btemp = __bsum;
	__bsum += __bterm;
	__btemp = __bterm - (__bsum - __btemp);
	__ctemp = __csum;
	__csum += __cterm;
	__ctemp = __cterm - (__csum - __ctemp);
	__dtemp = __dsum;
	__dsum += __dterm;
	__dtemp = __dterm - (__dsum - __dtemp);
	std::cout << " > > > asum = " << __asum << '\n';
	std::cout << " > > > bsum = " << __bsum << '\n';
	std::cout << " > > > csum = " << __csum << '\n';
	std::cout << " > > > dsum = " << __dsum << '\n';

	//  Combine sums in form appearing in expansions.
	__h1sum  = __aip  * __asum  + __zo4dp * __bsum;
	__h2sum  = __aim  * __asum  + __zo4dm * __bsum;
	__h1psum = __zod2p * __csum + __zod0dp * __dsum;
	__h2psum = __zod2m * __csum + __zod0dm * __dsum;
	std::cout << " > > > h1sum   = " << __h1sum << '\n';
	std::cout << " > > > h2sum   = " << __h2sum << '\n';
	std::cout << " > > > h1psum  = " << __h1psum << '\n';
	std::cout << " > > > h2psum  = " << __h2psum << '\n';
	std::cout << " > > > h1save  = " << __h1save << '\n';
	std::cout << " > > > h2save  = " << __h2save << '\n';
	std::cout << " > > > h1psave = " << __h1psave << '\n';
	std::cout << " > > > h2psave = " << __h2psave << '\n';

	//  If convergence criteria met this term, see if it was before.
	if (__norm_L1(__h1sum - __h1save) < __eps * __norm_L1(__h1sum)
	 && __norm_L1(__h2sum - __h2save) < __eps * __norm_L1(__h2sum)
	 && __norm_L1(__h1psum - __h1psave) < __eps * __norm_L1(__h1psum)
	 && __norm_L1(__h2psum - __h2psave) < __eps * __norm_L1(__h2psum))
	  {
	    if (__coverged) // Converged twice in a row - done!
	      return;
	    else // Converged once...
	      __coverged = true;
	  }
	else
	  __coverged = false;
	std::cout << " > > nduv = " << __u.size() << '\n';
	//  Save combined sums for comparison next iteration.
	__h1save = __h1sum;
	__h2save = __h2sum;
	__h1psave = __h1psum;
	__h2psave = __h2psum;
      }

    //  All allowable terms used - set completion code
    __status = 177;

    return;
  }


/**
 *    Compute parameters depending on z and nu that appear in the
 *    uniform asymptotic expansions of the Hankel functions and
 *    their derivatives, except the arguments to the Airy functions.
 */
template<typename _Tp>
  void
  __hankel_params(std::complex<_Tp> __nu, std::complex<_Tp> __zhat,
		  std::complex<_Tp>& __t, std::complex<_Tp>& __tsq,
		  std::complex<_Tp>& __nusq, std::complex<_Tp>& __1dnusq,
		  std::complex<_Tp>& __num1d3, std::complex<_Tp>& __num2d3,
		  std::complex<_Tp>& __num4d3, std::complex<_Tp>& __zeta,
		  std::complex<_Tp>& __zetaphf, std::complex<_Tp>& __zetamhf,
		  std::complex<_Tp>& __zetm3h, std::complex<_Tp>& __zetrat,
		  int& __status)
  {
    using __cmplx = std::complex<_Tp>;

    //  data statements defining constants used in this subroutine
    //  note that _S_inf and _S_sqrtinf are machine floating-point dependent
    //  constants equal to the largest available floating-point number and
    //  its square root, respectively.

    static constexpr auto _S_inf     = std::numeric_limits<_Tp>::max();
    static constexpr auto _S_sqrtinf = std::sqrt(_S_inf);

    static constexpr auto _S_1d4   = _Tp(0.25L);
    static constexpr auto _S_1d3   = _Tp(0.33333333333333333333L);
    static constexpr auto _S_1d2   = _Tp(0.5L);
    static constexpr auto _S_2d3   = _Tp(0.66666666666666633337L);
    static constexpr auto _S_2pi   = _Tp(6.283185307179586L);
    static constexpr auto _S_lncon = _Tp(0.2703100720721096L);
    static constexpr auto _S_sqrt2 = _Tp(1.4142135623730950L);
    static constexpr auto _S_4d3   = _Tp(1.33333333333333333333L);

    static constexpr __cmplx __zone{1.0L, 0.0L};
    static constexpr __cmplx _S_j{0.0L, 1.0L};

    //  Separate real and imaginary parts of zhat
    auto __dx = std::real(__zhat);
    auto __dy = std::imag(__zhat);
    auto __dxabs = std::abs(__dx);
    auto __dyabs = std::abs(__dy);

    //  If 1 - zhat^2 can be computed without overflow
    if (__dxabs <= _S_sqrtinf &&
	__dyabs <= (_S_sqrtinf - 1))
      {
	//  Find max and min of abs(dx) and abs(dy)
	auto __du = __dxabs;
	auto __dv = __dyabs;
	if (__du < __dv)
	  std::swap(__du, __dv);
	if (__du >= _S_1d2 && __dv > _S_inf / (2 * __du))
	  {
	    //  set completion code - unable to compute 1-zhat^2 and exit
	    __status = 131;
	    return;
	  }
      }
    else
      {
	//  set completion code - unable to compute 1-zhat^2 and exit
	__status = 131;
	return;
      }

    //  compute 1 - zhat^2 and related constants
    auto __ztemp = __cmplx{_Tp(1) - (__dx - __dy) * (__dx + __dy),
			-_Tp(2) * __dx * __dy};
    __ztemp = std::sqrt(__ztemp);
    __t = _Tp(1) / __ztemp;
    __tsq = __t * __t;

    //  if nu^2 can be computed without overflow
    if (std::abs(__nu) <= _S_sqrtinf)
      {
	__nusq = __nu * __nu;
	__1dnusq = _Tp(1) / __nusq;
	//  Compute nu^(-1/3), nu^(-2/3), nu^(-4/3)
	__num4d3 = -std::log(__nu);
	__num1d3 = std::exp(_S_1d3 * __num4d3);
	__num2d3 = std::exp(_S_2d3 * __num4d3);
	__num4d3 = std::exp(_S_4d3 * __num4d3);
      }
    else
      {
	//  set completion code - unable to compute nu^2
	__status = 132;
	return;
      }

    //  compute xi = ln(1+(1-zhat^2)^(1/2)) - ln(zhat) - (1-zhat^2)^(1/2)
    //  using default branch of logarithm and square root
    auto __xi = std::log(__zone + __ztemp) - std::log(__zhat) - __ztemp;
    __zetm3h = _S_2d3 / __xi;

    //  Compute principal value of ln(xi) and then adjust imaginary part
    auto __lnxi = std::log(__xi);

    //  Prepare to adjust logarithm of xi to appropriate Riemann sheet
    auto __dtemp = _Tp(0);

    //  Find adjustment necessary to get on proper Riemann sheet
    if (__dy == _Tp(0))  //  zhat is real
      {
	if (__dx > 1)
	  __dtemp = _S_2pi;
      }
    else  //  zhat is not real
      {
	//  if zhat is in upper half-plane
	if (__dy > _Tp(0))
	  {
	    //  if xi lies in upper half-plane
	    if (std::imag(__xi) > _Tp(0))
	      __dtemp = -_S_2pi;
	    else
	      __dtemp = +_S_2pi;
	  }
      }
    std::cout.precision(std::numeric_limits<double>::max_digits10);
    std::cout << " > > > dtemp = " << __dtemp << '\n';

    //  Adjust logarithm of xi.
    __lnxi += __dtemp * _S_j;
    std::cout << " > > > lnxi = " << __lnxi << '\n';

    //  Compute ln(zeta), zeta, zeta^(+1/2), zeta^(-1/2)
    auto __lnzeta = _S_2d3 * __lnxi + _S_lncon;
    __zeta = std::exp(__lnzeta);
    __zetaphf = std::sqrt(__zeta);
    __zetamhf = _Tp(1) / __zetaphf;

    //  compute (4 * zeta / (1 - zhat^2))^(1/4)
    __ztemp = std::log(__ztemp);
    __zetrat = _S_sqrt2 * std::exp(_S_1d4 * __lnzeta - _S_1d2 * __ztemp);

    return;
  }


/**
    Purpose
      Compute the arguments for the Airy function evaluations
      carefully to prevent premature overflow.  Note that the
      major work here is in safe_div.  A faster, but less safe
      implementation can be obtained without use of safe_div.

    Arguments
    @param[in]  num2d3  nu^(-2/3) - output from hankel_params.
    @param[in]  zeta    zeta in the uniform asymptotic expansions - output
			from hankel_params.
    @param[out]  zargp  exp(+2*pi*i/3) * nu^(-2/3) * zeta.
    @param[out]  zargm  exp(-2*pi*i/3) * nu^(-2/3) * zeta.
    @param[out]  status A completion code.
			status = 0 indicates normal completion.
			status = 133 indicates failure in computing
			nu(2/3)*zeta from the input zmn2d3 and zeta.
 */
template<typename _Tp>
  void
  __airy_arg(std::complex<_Tp> __nm2d3, std::complex<_Tp> __zeta,
	     std::complex<_Tp>& __argp, std::complex<_Tp>& __argm,
	     int& __status)
  {
    using __cmplx = std::complex<_Tp>;

    //  zexpp and zexpm are exp(2*pi*i/3) and its reciprocal, respectively.
    static constexpr auto __expp = __cmplx{-0.5L,  0.8660254037844386L};
    static constexpr auto __expm = __cmplx{-0.5L, -0.8660254037844386L};

    std::cout << " > > > airy_arg:\n";
    std::cout << " > > > > nm2d3 = " << __nm2d3 << '\n';
    std::cout << " > > > > zeta  = " << __zeta << '\n';

    __status = 0;

    if (__safe_div(__zeta, __nm2d3, __argm))
      {
	std::cout << " > > > > argm = " << __argm << '\n';
	__argp = __expp * __argm;
	__argm = __expm * __argm;
      }
    else
      __status = 133;
    std::cout << " > > > > argp = " << __argp << '\n';
    std::cout << " > > > > argm = " << __argm << '\n';
  }

#endif // HANKEL_TCC
