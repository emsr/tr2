#ifndef __K_DISTRIBUTION2
#define __K_DISTRIBUTION2 1

#include <random>

namespace __gnu_cxx
{

  /**
   * @brief A K continuous distribution for random numbers.
   *
   * The formula for the K probability density function is
   * @f[
   *     p(x|\lambda, \mu, \nu) = \frac{2}{x} 
   *             \left(\frac{\lambda\nu x}{\mu}\right)^{\frac{\lambda + \nu}{2}}
   *             \frac{1}{\Gamma(\lambda)\Gamma(\nu)}
   *             K_{\nu - \lambda}\left(2\sqrt{\frac{\lambda\nu x}{\mu}}\right)
   * @f]
   * where @f$I_0(z)@f$ is the modified Bessel function of the second kind
   * of order @f$\nu - \lambda@f$ and @f$\lambda > 0@f$, @f$\mu > 0@f$
   * and @f$\nu > 0@f$.
   *
   * <table border=1 cellpadding=10 cellspacing=0>
   * <caption align=top>Distribution Statistics</caption>
   * <tr><td>Mean</td><td>@f$\mu@f$</td></tr>
   * <tr><td>Variance</td><td>@f$\mu^2\frac{\lambda + \nu + 1}{\lambda\nu}@f$</td></tr>
   * <tr><td>Range</td><td>@f$[0, \infty)@f$</td></tr>
   * </table>
   */
  template<typename _RealType = double>
    class
    k_distribution2
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      typedef _RealType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef k_distribution2<result_type> distribution_type;

	param_type(result_type __lambda = result_type(1),
		   result_type __mu = result_type(1),
		   result_type __nu = result_type(1))
	: _M_lambda(__lambda), _M_mu(__mu), _M_nu(__nu)
	{
	  _GLIBCXX_DEBUG_ASSERT(_M_lambda > result_type(0));
	  _GLIBCXX_DEBUG_ASSERT(_M_mu > result_type(0));
	  _GLIBCXX_DEBUG_ASSERT(_M_nu > result_type(0));
	}

	result_type
	lambda() const
	{ return _M_lambda; }

	result_type
	mu() const
	{ return _M_mu; }

	result_type
	nu() const
	{ return _M_nu; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_lambda == __p2._M_lambda
              && __p1._M_mu == __p2._M_mu
	      && __p1._M_nu == __p2._M_nu; }

      private:
	void _M_initialize();

	result_type _M_lambda;
	result_type _M_mu;
	result_type _M_nu;
      };

      /**
       * @brief Constructors.
       */
      explicit
      k_distribution2(result_type __lambda = result_type(1),
		      result_type __mu = result_type(1),
		      result_type __nu = result_type(1))
      : _M_param(__lambda, __mu, __nu),
	_M_gd1(__lambda, result_type(1) / __lambda),
	_M_gd2(__nu, __mu / __nu)
      { }

      explicit
      k_distribution2(const param_type& __p)
      : _M_param(__p),
	_M_gd1(__p.lambda(), result_type(1) / __p.lambda()),
	_M_gd2(__p.nu(), __p.mu() / __p.nu())
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      {
	_M_gd1.reset();
	_M_gd2.reset();
      }

      /**
       * @brief Return the parameters of the distribution.
       */
      result_type
      lambda() const
      { return _M_param.lambda(); }

      result_type
      mu() const
      { return _M_param.mu(); }

      result_type
      nu() const
      { return _M_param.nu(); }

      /**
       * @brief Returns the parameter set of the distribution.
       */
      param_type
      param() const
      { return _M_param; }

      /**
       * @brief Sets the parameter set of the distribution.
       * @param __param The new parameter set of the distribution.
       */
      void
      param(const param_type& __param)
      { _M_param = __param; }

      /**
       * @brief Returns the greatest lower bound value of the distribution.
       */
      result_type
      min() const
      { return result_type(0); }

      /**
       * @brief Returns the least upper bound value of the distribution.
       */
      result_type
      max() const
      { return std::numeric_limits<result_type>::max(); }

      /**
       * @brief Generating functions.
       */
      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng);

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p);

      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate(_ForwardIterator __f, _ForwardIterator __t,
		   _UniformRandomNumberGenerator& __urng)
	{ this->__generate(__f, __t, __urng, this->param()); }

      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate(_ForwardIterator __f, _ForwardIterator __t,
		   _UniformRandomNumberGenerator& __urng,
		   const param_type& __p)
	{ this->__generate_impl(__f, __t, __urng, __p); }

      template<typename _UniformRandomNumberGenerator>
	void
	__generate(result_type* __f, result_type* __t,
		   _UniformRandomNumberGenerator& __urng,
		   const param_type& __p)
	{ this->__generate_impl(__f, __t, __urng, __p); }

      /**
       * @brief Return true if two K distributions have
       *        the same parameters and the sequences that would
       *        be generated are equal.
       */
      friend bool
      operator==(const k_distribution2<_RealType1>& __d1,
		 const k_distribution2<_RealType1>& __d2)
      { return (__d1.param() == __d2.param()
		&& __d1._M_gd1 == __d2._M_gd1
		&& __d1._M_gd2 == __d2._M_gd2); }

      /**
       * @brief Inserts a %k_distribution2 random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %k_distribution2 random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>&,
		   const k_distribution2<_RealType1>&);

      /**
       * @brief Extracts a %k_distribution2 random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x A %k_distribution2 random number
       *            generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>&,
		   k_distribution2<_RealType1>&);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;

      std::gamma_distribution<result_type> _M_gd1;
      std::gamma_distribution<result_type> _M_gd2;
    };

  /**
   * @brief Return true if two K distributions are not equal.
   */
  template<typename _RealType1>
    inline bool
    operator!=(const k_distribution2<_RealType1>& __d1,
	       const k_distribution2<_RealType1>& __d2)
    { return !(__d1 == __d2); }

}

#endif // __K_DISTRIBUTION2

#ifndef __K_DISTRIBUTION2_TCC
#define __K_DISTRIBUTION2_TCC 1

namespace __gnu_cxx
{

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      result_type
      k_distribution2<_RealType>::
      operator()(_UniformRandomNumberGenerator& __urng)
      {
	result_type __sigma = this->_M_gd2(__urng);
	typename std::gamma_distribution<result_type>::param_type
	  __p1(_M_gd1.alpha(), __sigma / _M_gd1.alpha());
	result_type __x = this->_M_gd1(__urng, __p1);
	return std::sqrt(__x);
      }

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      result_type
      k_distribution2<_RealType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const param_type& __p)
      {
	typename std::gamma_distribution<result_type>::param_type
	  __p2(__p.nu(), __p.mu() / __p.nu());
	result_type __sigma = this->_M_gd2(__p2, __urng);
	typename std::gamma_distribution<result_type>::param_type
	  __p1(__p.alpha(), __sigma / __p.alpha());
	result_type __x = this->_M_gd1(__urng, __p1);
	return std::sqrt(__x);
      }

  template<typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      k_distribution<_RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __p)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	typename std::gamma_distribution<result_type>::param_type
	  __p2(__p.nu(), __p.mu() / __p.nu());
	result_type __alpha = __p.alpha();
	while (__f != __t)
	  {
	    result_type __sigma = this->_M_gd2(__p2, __urng);
	    typename std::gamma_distribution<result_type>::param_type
	      __p1(__alpha, __sigma / __alpha);
	    result_type __x = this->_M_gd1(__urng, __p1);
	    *__f++ = std::sqrt(__x);
	  }
      }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const k_distribution2<_RealType>& __x)
    {
      typedef std::basic_ostream<_CharT, _Traits>  __ostream_type;
      typedef typename __ostream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __os.flags();
      const _CharT __fill = __os.fill();
      const std::streamsize __precision = __os.precision();
      const _CharT __space = __os.widen(' ');
      __os.flags(__ios_base::scientific | __ios_base::left);
      __os.fill(__space);
      __os.precision(std::numeric_limits<_RealType>::max_digits10);

      __os << __x.lambda() << __space << __x.mu() << __space << __x.nu();
      __os << __space << __x._M_gd1;
      __os << __space << __x._M_gd2;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       k_distribution2<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __lambda, __mu, __nu;
      __is >> __lambda >> __mu >> __nu;
      __is >> __x._M_gd1;
      __is >> __x._M_gd2;
      __x.param(typename k_distribution2<_RealType>::
		param_type(__lambda, __mu, __nu));

      __is.flags(__flags);
      return __is;
    }

}

#endif // __K_DISTRIBUTION2_TCC
