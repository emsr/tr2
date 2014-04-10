#ifndef __PARETO_DISTRIBUTION
#define __PARETO_DISTRIBUTION 1

#include <random>

namespace __gnu_cxx
{

  /**
   * @brief A Pareto continuous distribution for random numbers.
   *
   * The formula for the Pareto cumulative probability function is
   * @f[
   *     P(x|\alpha,\mu) = 1 - \left(\frac{\mu}{x}\right)^\alpha
   * @f]
   * The formula for the Pareto probability density function is
   * @f[
   *     p(x|\alpha,\mu) = \frac{\alpha + 1}{\mu}
   *                       \left(\frac{\mu}{x}\right)^{\alpha + 1}
   * @f]
   * where @f$x >= \mu@f$ and @f$\mu > 0@f$, @f$\alpha > 0@f$.
   *
   * <table border=1 cellpadding=10 cellspacing=0>
   * <caption align=top>Distribution Statistics</caption>
   * <tr><td>Mean</td><td>@f$\alpha \mu / (\alpha - 1)@f$
   *              for @f$\alpha > 1@f$</td></tr>
   * <tr><td>Variance</td><td>@f$\alpha \mu^2 / [(\alpha - 1)^2(\alpha - 2)]@f$
   *              for @f$\alpha > 2@f$</td></tr>
   * <tr><td>Range</td><td>@f$[\mu, \infty)@f$</td></tr>
   * </table>
   */
  template<typename _RealType = double>
    class
    pareto_distribution
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      typedef _RealType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef pareto_distribution<result_type> distribution_type;

	param_type(result_type __alpha = result_type(1),
		   result_type __mu = result_type(1))
	: _M_alpha(__alpha), _M_mu(__mu)
	{
	  _GLIBCXX_DEBUG_ASSERT(_M_alpha > result_type(0));
	  _GLIBCXX_DEBUG_ASSERT(_M_mu > result_type(0));
	}

	result_type
	alpha() const
	{ return _M_alpha; }

	result_type
	mu() const
	{ return _M_mu; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_alpha == __p2._M_alpha && __p1._M_mu == __p2._M_mu; }

      private:
	void _M_initialize();

	result_type _M_alpha;
	result_type _M_mu;
      };

      /**
       * @brief Constructors.
       */
      explicit
      pareto_distribution(result_type __alpha = result_type(1),
			  result_type __mu = result_type(1))
      : _M_param(__alpha, __mu),
	_M_ud()
      { }

      explicit
      pareto_distribution(const param_type& __p)
      : _M_param(__p),
	_M_ud()
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      {
	_M_ud.reset();
      }

      /**
       * @brief Return the parameters of the distribution.
       */
      result_type
      alpha() const
      { return _M_param.alpha(); }

      result_type
      mu() const
      { return _M_param.mu(); }

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
      { return this->mu(); }

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
	operator()(_UniformRandomNumberGenerator& __urng)
	{
	  return this->mu() * std::pow(this->_M_ud(__urng),
				       -result_type(1) / this->alpha());
	}

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p)
        {
	  return __p.mu() * std::pow(this->_M_ud(__urng),
					   -result_type(1) / __p.alpha());
	}

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
       * @brief Return true if two Pareto distributions have
       *        the same parameters and the sequences that would
       *        be generated are equal.
       */
      friend bool
      operator==(const pareto_distribution& __d1,
		 const pareto_distribution& __d2)
      { return (__d1.param() == __d2.param()
		&& __d1._M_ud == __d2._M_ud); }

      /**
       * @brief Inserts a %pareto_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %pareto_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>&,
		   const pareto_distribution<_RealType1>&);

      /**
       * @brief Extracts a %pareto_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x A %pareto_distribution random number
       *            generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>&,
		   pareto_distribution<_RealType1>&);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;

      std::uniform_real_distribution<result_type> _M_ud;
    };

  /**
   * @brief Return true if two Pareto distributions are not equal.
   */
  template<typename _RealType>
    inline bool
    operator!=(const pareto_distribution<_RealType>& __d1,
	       const pareto_distribution<_RealType>& __d2)
    { return !(__d1 == __d2); }

}

#endif // __PARETO_DISTRIBUTION

#ifndef __PARETO_DISTRIBUTION_TCC
#define __PARETO_DISTRIBUTION_TCC 1

namespace __gnu_cxx
{

  template<typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      pareto_distribution<_RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __p)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	result_type __mu = __p.mu();
	result_type __malphinv = -result_type(1) / __p.alpha();
	while (__f != __t)
	  *__f++ = __mu * std::pow(this->_M_ud(__urng), __malphinv);
      }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const pareto_distribution<_RealType>& __x)
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

      __os << __x.alpha() << __space << __x.mu();
      __os << __space << __x._M_ud;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       pareto_distribution<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __alpha, __mu;
      __is >> __alpha >> __mu;
      __is >> __x._M_ud;
      __x.param(typename pareto_distribution<_RealType>::
		param_type(__alpha, __mu));

      __is.flags(__flags);
      return __is;
    }

}

#endif // __PARETO_DISTRIBUTION_TCC
