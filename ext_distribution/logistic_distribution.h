#ifndef __LOGISTIC_DISTRIBUTION
#define __LOGISTIC_DISTRIBUTION 1

#include <random>

namespace __gnu_cxx
{

  /**
   * @brief A logistic continuous distribution for random numbers.
   *
   * The formula for the logistic probability density function is
   * @f[
   *     p(x|\a,\b) = \frac{e^{(x - a)/b}}{b[1 + e^{(x - a)/b}]^2}
   * @f]
   * where @f$b > 0@f$.
   *
   * The formula for the logistic probability function is
   * @f[
   *     cdf(x|\a,\b) = \frac{e^{(x - a)/b}}{1 + e^{(x - a)/b}}
   * @f]
   * where @f$b > 0@f$.
   *
   * <table border=1 cellpadding=10 cellspacing=0>
   * <caption align=top>Distribution Statistics</caption>
   * <tr><td>Mean</td><td>@f$a@f$</td></tr>
   * <tr><td>Variance</td><td>@f$b^2\pi^2/3@f$</td></tr>
   * <tr><td>Range</td><td>@f$[0, \infty)@f$</td></tr>
   * </table>
   */
  template<typename _RealType = double>
    class
    logistic_distribution
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      typedef _RealType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef logistic_distribution<result_type> distribution_type;

	param_type(result_type __a = result_type(0),
		   result_type __b = result_type(1))
	: _M_a(__a), _M_b(__b)
	{
	  _GLIBCXX_DEBUG_ASSERT(_M_b > result_type(0));
	}

	result_type
	a() const
	{ return _M_a; }

	result_type
	b() const
	{ return _M_b; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_a == __p2._M_a
              && __p1._M_b == __p2._M_b; }

      private:
	void _M_initialize();

	result_type _M_a;
	result_type _M_b;
      };

      /**
       * @brief Constructors.
       */
      explicit
      logistic_distribution(result_type __a = result_type(0),
			    result_type __b = result_type(1))
      : _M_param(__a, __b)
      { }

      explicit
      logistic_distribution(const param_type& __p)
      : _M_param(__p)
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      { }

      /**
       * @brief Return the parameters of the distribution.
       */
      result_type
      a() const
      { return _M_param.a(); }

      result_type
      b() const
      { return _M_param.b(); }

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
      { return -std::numeric_limits<result_type>::max(); }

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
	  std::__detail::_Adaptor<_UniformRandomNumberGenerator, result_type>
	    __aurng(__urng);

	  result_type __arg = result_type(1);
	  while (__arg == result_type(1) || __arg == result_type(0))
	    __arg = __aurng();
	  return this->param().a()
	       + this->param().b() * std::log(__arg / (result_type(1) - __arg));
	}

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p)
        {
	  std::__detail::_Adaptor<_UniformRandomNumberGenerator, result_type>
	    __aurng(__urng);

	  result_type __arg = result_type(1);
	  while (__arg == result_type(1) || __arg == result_type(0))
	    __arg = __aurng();
	  return __p.a()
	       + __p.b() * std::log(__arg / (result_type(1) - __arg));
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
       * @brief Return true if two logistic distributions have
       *        the same parameters and the sequences that would
       *        be generated are equal.
       */
      template<typename _RealType1>
        friend bool
        operator==(const logistic_distribution<_RealType1>& __d1,
		   const logistic_distribution<_RealType1>& __d2)
        { return __d1.param() == __d2.param(); }

      /**
       * @brief Inserts a %logistic_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %logistic_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>&,
		   const logistic_distribution<_RealType1>&);

      /**
       * @brief Extracts a %logistic_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x A %logistic_distribution random number
       *            generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>&,
		   logistic_distribution<_RealType1>&);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;
    };

  /**
   * @brief Return true if two logistic distributions are not equal.
   */
  template<typename _RealType1>
    inline bool
    operator!=(const logistic_distribution<_RealType1>& __d1,
	       const logistic_distribution<_RealType1>& __d2)
    { return !(__d1 == __d2); }

}

#endif // __LOGISTIC_DISTRIBUTION

#ifndef __LOGISTIC_DISTRIBUTION_TCC
#define __LOGISTIC_DISTRIBUTION_TCC 1

namespace __gnu_cxx
{

  template<typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      logistic_distribution<_RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __p)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)
	std::__detail::_Adaptor<_UniformRandomNumberGenerator, result_type>
	  __aurng(__urng);

	while (__f != __t)
	  {
	    result_type __arg = result_type(1);
	    while (__arg == result_type(1) || __arg == result_type(0))
	      __arg = __aurng();
	    *__f++ = __p.a()
		   + __p.b() * std::log(__arg / (result_type(1) - __arg));
	  }
      }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const logistic_distribution<_RealType>& __x)
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

      __os << __x.a() << __space << __x.b();

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       logistic_distribution<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __a, __b;
      __is >> __a >> __b;
      __x.param(typename logistic_distribution<_RealType>::
		param_type(__a, __b));

      __is.flags(__flags);
      return __is;
    }

}

#endif // __LOGISTIC_DISTRIBUTION_TCC
