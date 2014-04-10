#ifndef __RICE_DISTRIBUTION
#define __RICE_DISTRIBUTION 1

#include <random>

namespace __gnu_cxx
{

  /**
   * @brief A Rice continuous distribution for random numbers.
   *
   * The formula for the Rice probability density function is
   * @f[
   *     p(x|\nu,\sigma) = \frac{x}{\sigma^2}
   *                       \exp\left(-\frac{x^2+\nu^2}{2\sigma^2}\right)
   *                       I_0\left(\frac{x \nu}{\sigma^2}\right)
   * @f]
   * where @f$I_0(z)@f$ is the modified Bessel function of the first kind
   * of order 0 and @f$\nu >= 0@f$ and @f$\sigma > 0@f$.
   *
   * <table border=1 cellpadding=10 cellspacing=0>
   * <caption align=top>Distribution Statistics</caption>
   * <tr><td>Mean</td><td>@f$\sqrt{\pi/2}L_{1/2}(-\nu^2/2\sigma^2)@f$</td></tr>
   * <tr><td>Variance</td><td>@f$2\sigma^2 + \nu^2
   *                   + (\pi\sigma^2/2)L^2_{1/2}(-\nu^2/2\sigma^2)@f$</td></tr>
   * <tr><td>Range</td><td>@f$[0, \infty)@f$</td></tr>
   * </table>
   * where @f$L_{1/2}(x)@f$ is the Laguerre polynomial of order 1/2.
   */
  template<typename _RealType = double>
    class
    rice_distribution
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      typedef _RealType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef rice_distribution<result_type> distribution_type;

	param_type(result_type __nu = result_type(0),
		   result_type __sigma = result_type(1))
	: _M_nu(__nu), _M_sigma(__sigma)
	{
	  _GLIBCXX_DEBUG_ASSERT(_M_nu >= result_type(0));
	  _GLIBCXX_DEBUG_ASSERT(_M_sigma > result_type(0));
	}

	result_type
	nu() const
	{ return _M_nu; }

	result_type
	sigma() const
	{ return _M_sigma; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_nu == __p2._M_nu
              && __p1._M_sigma == __p2._M_sigma; }

      private:
	void _M_initialize();

	result_type _M_nu;
	result_type _M_sigma;
      };

      /**
       * @brief Constructors.
       */
      explicit
      rice_distribution(result_type __nu = result_type(0),
			result_type __sigma = result_type(1))
      : _M_param(__nu, __sigma),
	_M_ndx(__nu, __sigma),
	_M_ndy(result_type(0), __sigma)
      { }

      explicit
      rice_distribution(const param_type& __p)
      : _M_param(__p),
	_M_ndx(__p.nu(), __p.sigma()),
	_M_ndy(result_type(0), __p.sigma())
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      {
	_M_ndx.reset();
	_M_ndy.reset();
      }

      /**
       * @brief Return the parameters of the distribution.
       */
      result_type
      nu() const
      { return _M_param.nu(); }

      result_type
      sigma() const
      { return _M_param.sigma(); }

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
	operator()(_UniformRandomNumberGenerator& __urng)
	{
	  result_type __x = this->_M_ndx(__urng);
	  result_type __y = this->_M_ndy(__urng);
	  return std::sqrt(__x * __x + __y * __y);
	}

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p)
        {
	  typename std::normal_distribution<result_type>::param_type
	    __px(__p.nu(), __p.sigma()), __py(result_type(0), __p.sigma());
	  result_type __x = this->_M_ndx(__px, __urng);
	  result_type __y = this->_M_ndy(__py, __urng);
	  return std::sqrt(__x * __x + __y * __y);
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
       * @brief Return true if two Rice distributions have
       *        the same parameters and the sequences that would
       *        be generated are equal.
       */
      template<typename _RealType1>
        friend bool
        operator==(const rice_distribution<_RealType1>& __d1,
		   const rice_distribution<_RealType1>& __d2)
        { return (__d1.param() == __d2.param()
		  && __d1._M_ndx == __d2._M_ndx
		  && __d1._M_ndy == __d2._M_ndy); }

      /**
       * @brief Inserts a %rice_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %rice_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>&,
		   const rice_distribution<_RealType1>&);

      /**
       * @brief Extracts a %rice_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x A %rice_distribution random number
       *            generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>&,
		   rice_distribution<_RealType1>&);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;

      std::normal_distribution<result_type> _M_ndx;
      std::normal_distribution<result_type> _M_ndy;
    };

  /**
   * @brief Return true if two Rice distributions are not equal.
   */
  template<typename _RealType1>
    inline bool
    operator!=(const rice_distribution<_RealType1>& __d1,
	       const rice_distribution<_RealType1>& __d2)
    { return !(__d1 == __d2); }

}

#endif // __RICE_DISTRIBUTION

#ifndef __RICE_DISTRIBUTION_TCC
#define __RICE_DISTRIBUTION_TCC 1

namespace __gnu_cxx
{

  template<typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      rice_distribution<_RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __p)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	while (__f != __t)
	  {
	    typename std::normal_distribution<result_type>::param_type
	      __px(__p.nu(), __p.sigma()), __py(result_type(0), __p.sigma());
	    result_type __x = this->_M_ndx(__px, __urng);
	    result_type __y = this->_M_ndy(__py, __urng);
	    *__f++ = std::hypot(__x, __y);
	  }
      }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const rice_distribution<_RealType>& __x)
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

      __os << __x.nu() << __space << __x.sigma();
      __os << __space << __x._M_ndx;
      __os << __space << __x._M_ndy;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       rice_distribution<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __nu, __sigma;
      __is >> __nu >> __sigma;
      __is >> __x._M_ndx;
      __is >> __x._M_ndy;
      __x.param(typename rice_distribution<_RealType>::
		param_type(__nu, __sigma));

      __is.flags(__flags);
      return __is;
    }

}

#endif // __RICE_DISTRIBUTION_TCC
