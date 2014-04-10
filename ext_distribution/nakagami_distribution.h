#ifndef __NAKAGAMI_DISTRIBUTION
#define __NAKAGAMI_DISTRIBUTION 1

#include <random>

namespace __gnu_cxx
{

  /**
   * @brief A Nakagami continuous distribution for random numbers.
   *
   * The formula for the Nakagami probability density function is
   * @f[
   *     p(x|\mu,\omega) = \frac{2\mu^\mu}{\Gamma(\mu)\omega^\mu}
   *                       x^{2\mu-1}e^{-\mu x / \omega}
   * @f]
   * where @f$\Gamma(z)@f$ is the gamma function and @f$\mu >= 0.5@f$
   * and @f$\omega > 0@f$.
   */
  template<typename _RealType = double>
    class
    nakagami_distribution
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      typedef _RealType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef nakagami_distribution<result_type> distribution_type;

	param_type(result_type __mu = result_type(1),
		   result_type __omega = result_type(1))
	: _M_mu(__mu), _M_omega(__omega)
	{
	  _GLIBCXX_DEBUG_ASSERT(_M_mu >= result_type(0.5L));
	  _GLIBCXX_DEBUG_ASSERT(_M_omega > result_type(0));
	}

	result_type
	mu() const
	{ return _M_mu; }

	result_type
	omega() const
	{ return _M_omega; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_mu == __p2._M_mu
              && __p1._M_omega == __p2._M_omega; }

      private:
	void _M_initialize();

	result_type _M_mu;
	result_type _M_omega;
      };

      /**
       * @brief Constructors.
       */
      explicit
      nakagami_distribution(result_type __mu = result_type(1),
			    result_type __omega = result_type(1))
      : _M_param(__mu, __omega),
	_M_gd(__mu, __omega / __mu)
      { }

      explicit
      nakagami_distribution(const param_type& __p)
      : _M_param(__p),
	_M_gd(__p.mu(), __p.omega() / __p.mu())
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      { _M_gd.reset(); }

      /**
       * @brief Return the parameters of the distribution.
       */
      result_type
      mu() const
      { return _M_param.mu(); }

      result_type
      omega() const
      { return _M_param.omega(); }

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
	{ return std::sqrt(this->_M_gd(__urng)); }

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p)
        {
	  typename std::gamma_distribution<result_type>::param_type
	    __pg(__p.mu(), __p.omega() / __p.mu());
	  return std::sqrt(this->_M_gd(__pg, __urng));
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
       * @brief Return true if two Nakagami distributions have
       *        the same parameters and the sequences that would
       *        be generated are equal.
       */
      friend bool
      operator==(const nakagami_distribution& __d1,
		 const nakagami_distribution& __d2)
      { return (__d1.param() == __d2.param()
		&& __d1._M_gd == __d2._M_gd); }

      /**
       * @brief Inserts a %nakagami_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %nakagami_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>&,
		   const nakagami_distribution<_RealType1>&);

      /**
       * @brief Extracts a %nakagami_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x A %nakagami_distribution random number
       *            generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>&,
		   nakagami_distribution<_RealType1>&);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;

      std::gamma_distribution<result_type> _M_gd;
    };

  /**
   * @brief Return true if two Nakagami distributions are not equal.
   */
  template<typename _RealType>
    inline bool
    operator!=(const nakagami_distribution<_RealType>& __d1,
	       const nakagami_distribution<_RealType>& __d2)
    { return !(__d1 == __d2); }

}

#endif // __NAKAGAMI_DISTRIBUTION

#ifndef __NAKAGAMI_DISTRIBUTION_TCC
#define __NAKAGAMI_DISTRIBUTION_TCC 1

namespace __gnu_cxx
{

  template<typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      nakagami_distribution<_RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __p)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	typename std::gamma_distribution<result_type>::param_type
	  __pg(__p.mu(), __p.omega() / __p.mu());
	while (__f != __t)
	  *__f++ = std::sqrt(this->_M_gd(__pg, __urng));
      }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const nakagami_distribution<_RealType>& __x)
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

      __os << __x.mu() << __space << __x.omega();
      __os << __space << __x._M_gd;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       nakagami_distribution<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __mu, __omega;
      __is >> __mu >> __omega;
      __is >> __x._M_gd;
      __x.param(typename nakagami_distribution<_RealType>::
		param_type(__mu, __omega));

      __is.flags(__flags);
      return __is;
    }

}

#endif // __NAKAGAMI_DISTRIBUTION_TCC
