
// /home/ed/bin/bin/g++ -g -std=c++11 -o gamma gamma.cpp

#include <iostream>
#include <complex>
#include <limits>

// From bits/special_function_util.h

namespace std
{

  namespace __detail
  {

    /// A class to encapsulate type dependent floating point
    /// constants.  Not everything will be able to be expressed as
    /// type logic.
    template<typename _Tp>
    struct __floating_point_constant
    {
      static const _Tp __value;
    };


    /// A structure for numeric constants.
    template<typename _Tp>
      struct __numeric_constants
      {
	///  Constant @f$ \pi @f$.
	static _Tp __pi() throw()
	{ return static_cast<_Tp>(3.1415926535897932384626433832795029L); }
	///  Constant @f$ \pi / 2 @f$.
	static _Tp __pi_2() throw()
	{ return static_cast<_Tp>(1.5707963267948966192313216916397514L); }
	///  Constant @f$ \pi / 3 @f$.
	static _Tp __pi_3() throw()
	{ return static_cast<_Tp>(1.0471975511965977461542144610931676L); }
	///  Constant @f$ \pi / 4 @f$.
	static _Tp __pi_4() throw()
	{ return static_cast<_Tp>(0.7853981633974483096156608458198757L); }
	///  Constant @f$ 1 / \pi @f$.
	static _Tp __1_pi() throw()
	{ return static_cast<_Tp>(0.3183098861837906715377675267450287L); }
	///  Constant @f$ 2 / \sqrt(\pi) @f$.
	static _Tp __2_sqrtpi() throw()
	{ return static_cast<_Tp>(1.1283791670955125738961589031215452L); }
	///  Constant @f$ \sqrt(2) @f$.
	static _Tp __sqrt2() throw()
	{ return static_cast<_Tp>(1.4142135623730950488016887242096981L); }
	///  Constant @f$ \sqrt(3) @f$.
	static _Tp __sqrt3() throw()
	{ return static_cast<_Tp>(1.7320508075688772935274463415058723L); }
	///  Constant @f$ \sqrt(\pi/2) @f$.
	static _Tp __sqrtpio2() throw()
	{ return static_cast<_Tp>(1.2533141373155002512078826424055226L); }
	///  Constant @f$ 1 / sqrt(2) @f$.
	static _Tp __sqrt1_2() throw()
	{ return static_cast<_Tp>(0.7071067811865475244008443621048490L); }
	///  Constant @f$ \log(\pi) @f$.
	static _Tp __lnpi() throw()
	{ return static_cast<_Tp>(1.1447298858494001741434273513530587L); }
	///  Constant Euler's constant @f$ \gamma_E @f$.
	static _Tp __gamma_e() throw()
	{ return static_cast<_Tp>(0.5772156649015328606065120900824024L); }
	///  Constant Euler-Mascheroni @f$ e @f$
	static _Tp __euler() throw()
	{ return static_cast<_Tp>(2.7182818284590452353602874713526625L); }
      };


#if _GLIBCXX_USE_C99_MATH && !_GLIBCXX_USE_C99_FP_MACROS_DYNAMIC

    /// This is a wrapper for the isnan function. Otherwise, for NaN,
    /// all comparisons result in false. If/when we build a std::isnan
    /// out of intrinsics, this will disappear completely in favor of
    /// std::isnan.
    template<typename _Tp>
    inline bool __isnan(const _Tp __x)
    {
      return std::isnan(__x);
    }

#else

    template<typename _Tp>
    inline bool __isnan(const _Tp __x)
    {
      return __builtin_isnan(__x);
    }

    template<>
    inline bool __isnan<float>(const float __x)
    {
      return __builtin_isnanf(__x);
    }

    template<>
    inline bool __isnan<long double>(const long double __x)
    {
      return __builtin_isnanl(__x);
    }

#endif

  } // namespace __detail

}

// End bits/special_function_util.h

namespace std
{
  namespace __detail
  {
    template<typename _Tp>
      struct __ellint_traits
      {
	typedef _Tp __value_type;
      };

    template<>
      template<typename _Tp>
	struct __ellint_traits<std::complex<_Tp> >
	{
	  typedef _Tp __value_type;
	};

    /**
     *   @brief Return \f$log(\Gamma(x))\f$ by the Lanczos method.
     *          This method dominates all others on the positive axis I think.
     *
     *   @param __x The argument of the log of the gamma function.
     *   @return  The logarithm of the gamma function.
     */
    template<typename _Tp>
      _Tp
      __log_gamma_lanczos(_Tp __x)
      {
	typedef typename __ellint_traits<_Tp>::__value_type _Val;
	const _Tp __xm1 = __x - _Tp(1);

	static const _Val __lanczos_cheb_7[9] = {
	 _Val( 0.99999999999980993227684700473478L),
	 _Val( 676.520368121885098567009190444019L),
	 _Val(-1259.13921672240287047156078755283L),
	 _Val( 771.3234287776530788486528258894L),
	 _Val(-176.61502916214059906584551354L),
	 _Val( 12.507343278686904814458936853L),
	 _Val(-0.13857109526572011689554707L),
	 _Val( 9.984369578019570859563e-6L),
	 _Val( 1.50563273514931155834e-7L)
	};

	static const _Val __LOGROOT2PI
	    = _Val(0.9189385332046727417803297364056176L);

	_Tp __sum = _Tp(__lanczos_cheb_7[0]);
	for(unsigned int __k = 1; __k < 9; ++__k)
	  __sum += _Tp(__lanczos_cheb_7[__k]) / (__xm1 + _Tp(__k));

	const _Tp __term1 = (__xm1 + _Tp(0.5L))
			  * std::log((__xm1 + _Tp(7.5L))
			 / __numeric_constants<_Val>::__euler());
	const _Tp __term2 = _Tp(__LOGROOT2PI) + std::log(__sum);
	const _Tp __result = __term1 + (__term2 - _Tp(7));

	return __result;
      }


    /**
     *   @brief Return \f$ log(|\Gamma(x)|) \f$.
     *          This will return values even for \f$ x < 0 \f$.
     *          To recover the sign of \f$ \Gamma(x) \f$ for
     *          any argument use @a __sign_gamma.
     *
     *   @param __x The argument of the log of the gamma function.
     *   @return  The logarithm of the gamma function.
     */
    template<typename _Tp>
      _Tp
      __log_gamma(_Tp __x)
      {
	typedef typename __ellint_traits<_Tp>::__value_type _Val;
	if (std::real(__x) > _Val(0.5L))
	  return __log_gamma_lanczos(__x);
	else
	  {
	    const _Tp __sin_fact
		   = std::abs(std::sin(__numeric_constants<_Val>::__pi() * __x));
	    if (__sin_fact == _Tp(0))
	      std::__throw_domain_error(__N("Argument is nonpositive integer "
					    "in __log_gamma"));
	    return __numeric_constants<_Val>::__lnpi()
		       - std::log(__sin_fact)
		       - __log_gamma_lanczos(_Tp(1) - __x);
	  }
      }


    /**
     *   @brief Return the sign of \f$ \Gamma(x) \f$.
     *          At nonpositive integers zero is returned.
     *
     *   @param __x The argument of the gamma function.
     *   @return  The sign of the gamma function.
     */
    template<typename _Tp>
      _Tp
      __sign_gamma(_Tp __x)
      {
	typedef typename __ellint_traits<_Tp>::__value_type _Val;
	if (__x > _Tp(0))
	  return _Tp(1);
	else
	  {
	    const _Tp __sin_fact
		    = std::sin(__numeric_constants<_Tp>::__pi() * __x);
	    if (__sin_fact > _Tp(0))
	      return (1);
	    else if (__sin_fact < _Tp(0))
	      return -_Tp(1);
	    else
	      return _Tp(0);
	  }
      }

    template<typename _Tp>
      std::complex<_Tp>
      __sign_gamma(std::complex<_Tp> __x)
      {
	return _Tp(1);
      }

    /**
     *   @brief Return the logarithm of the binomial coefficient.
     *   The binomial coefficient is given by:
     *   @f[
     *   \left(  \right) = \frac{n!}{(n-k)! k!}
     *   @f]
     *
     *   @param __n The first argument of the binomial coefficient.
     *   @param __k The second argument of the binomial coefficient.
     *   @return  The binomial coefficient.
     */
    template<typename _Tp>
      _Tp
      __log_bincoef(unsigned int __n, unsigned int __k)
      {
	//  Max e exponent before overflow.
	static const _Tp __max_bincoeff
			= std::numeric_limits<_Tp>::max_exponent10
			* std::log(_Tp(10)) - _Tp(1);
	_Tp __coeff = __log_gamma(_Tp(1 + __n))
		    - __log_gamma(_Tp(1 + __k))
		    - __log_gamma(_Tp(1 + __n - __k));
      }


    /**
     *   @brief Return the binomial coefficient.
     *   The binomial coefficient is given by:
     *   @f[
     *   \left(  \right) = \frac{n!}{(n-k)! k!}
     *   @f]
     *
     *   @param __n The first argument of the binomial coefficient.
     *   @param __k The second argument of the binomial coefficient.
     *   @return  The binomial coefficient.
     */
    template<typename _Tp>
      _Tp
      __bincoef(unsigned int __n, unsigned int __k)
      {
	//  Max e exponent before overflow.
	static const _Tp __max_bincoeff
			= std::numeric_limits<_Tp>::max_exponent10
			* std::log(_Tp(10)) - _Tp(1);

	const _Tp __log_coeff = __log_bincoef<_Tp>(__n, __k);
	if (__log_coeff > __max_bincoeff)
	  return std::numeric_limits<_Tp>::quiet_NaN();
	else
	  return std::exp(__log_coeff);
      }


    /**
     *   @brief Return \f$ \Gamma(x) \f$.
     *
     *   @param __x The argument of the gamma function.
     *   @return  The gamma function.
     */
    template<typename _Tp>
      _Tp
      __gamma(_Tp __x)
      {
	typedef typename __ellint_traits<_Tp>::__value_type _Val;
	//  Max e exponent before overflow.
	static const _Val __max_gam
			 = std::numeric_limits<_Val>::max_exponent10
			 * std::log(_Val(10)) - _Val(1);

	const _Tp __log_gam = __log_gamma(__x);
	const _Tp __sgn_gam = __sign_gamma(__x);
	if (std::real(__log_gam) > __max_gam)
	  return __sgn_gam * std::numeric_limits<_Val>::quiet_NaN();
	else
	  return __sgn_gam * std::exp(__log_gam);
      }

  }
}


int
main()
{
  std::cout.precision(14);

  std::cout << "gamma(1) = " << std::__detail::__gamma(1.0) << std::endl;  //  
  std::cout << "gamma(1.5) = " << std::__detail::__gamma(1.5) << std::endl;  //  
  std::cout << "gamma(2) = " << std::__detail::__gamma(2.0) << std::endl;  //  
  std::cout << "gamma(0.5) = " << std::__detail::__gamma(0.5) << std::endl;  //  
  std::cout << "gamma(-0.5) = " << std::__detail::__gamma(-0.5) << std::endl;  //  

  std::cout << "gamma(1 + i) = " << std::__detail::__gamma(std::complex<double>(1.0,1.0)) << std::endl;  //  
  std::cout << "abs(gamma(1 + i) = " << std::abs(std::__detail::__gamma(std::complex<double>(1.0,1.0))) << std::endl;  //  
  std::cout << "arg(gamma(1 + i) = " << std::arg(std::__detail::__gamma(std::complex<double>(1.0,1.0))) << std::endl;  //  
  std::cout << "gamma(1 - 1) = " << std::__detail::__gamma(std::complex<double>(1.0,-1.0)) << std::endl;  //  

  return 0;
}

