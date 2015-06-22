#ifndef __BETA_DISTRIBUTION
#define __BETA_DISTRIBUTION 1

//  Based on code from Kai-Uwe Bux
//  Method: D. E. Knuth: TAOCP II, 3.4.1 E (2)

#include <random>

namespace __gnu_cxx
{

  /**
   * @brief A beta continuous distribution for random numbers.
   *
   * The formula for the beta probability density function is
   * @f[
   *     p(x|\alpha,\beta) = \frac{x^{\alpha - 1} (1 - x)^{\beta - 1}}
   *                         {B(\alpha,\beta)}
   * @f]
   * where
   * @f[
   *     B(\alpha,\beta) = \frac{\Gamma(\alpha)\Gamma(\beta)}
   *                                {\Gamma(\alpha + \beta)}
   * @f]
   * is the beta function and @f$\alpha > 0@f$ and @f$\beta > 0@f$.
   */
  template<typename _RealType = double>
    class beta_distribution
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      typedef _RealType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef beta_distribution<result_type> distribution_type;
	friend class beta_distribution<result_type>;

	explicit
	param_type(result_type __alpha_val = result_type(1),
		   result_type __beta_val = result_type(1))
	: _M_alpha(__alpha_val), _M_beta(__beta_val)
	{
	  _GLIBCXX_DEBUG_ASSERT(_M_alpha > result_type(0));
	  _GLIBCXX_DEBUG_ASSERT(_M_beta > result_type(0));
	}

	result_type
	alpha() const
	{ return _M_alpha; }

	result_type
	beta() const
	{ return _M_beta; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_alpha == __p2._M_alpha
              && __p1._M_beta == __p2._M_beta; }

      private:

	result_type _M_alpha;
	result_type _M_beta;
      };

    public:
      /**
       * @brief Constructs a beta distribution with parameters
       * @f$ \alpha @f$ and @f$ \beta @f$.
       */
      explicit
      beta_distribution(result_type __alpha_val = result_type(1),
			result_type __beta_val = result_type(1))
      : _M_param(__alpha_val, __beta_val),
	_M_gda(__alpha_val, result_type(1)),
	_M_gdb(__beta_val, result_type(1))
      { }

      explicit
      beta_distribution(const param_type& __p)
      : _M_param(__p),
	_M_gda(__p.alpha(), result_type(1)),
	_M_gdb(__p.beta(), result_type(1))
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      {
	_M_gda.reset();
	_M_gdb.reset();
      }

      /**
       * @brief Returns the @f$ \alpha @f$ of the distribution.
       */
      result_type
      alpha() const
      { return _M_param.alpha(); }

      /**
       * @brief Returns the @f$ \beta @f$ of the distribution.
       */
      result_type
      beta() const
      { return _M_param.beta(); }

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
      { return result_type(1); }

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng);

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p);

      /**
       * @brief Return true if two beta distributions have
       *        the same parameters and the sequences that would
       *        be generated are equal.
       */
      friend bool
      operator==(const beta_distribution& __d1,
		 const beta_distribution& __d2)
      { return (__d1.param() == __d2.param()
		&& __d1._M_gda == __d2._M_gda
		&& __d1._M_gdb == __d2._M_gdb); }

      /**
       * @brief Inserts a %beta_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %beta_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>&,
		   const beta_distribution<_RealType1>&);

      /**
       * @brief Extracts a %beta_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x  A %beta_distribution random number generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>&,
		   beta_distribution<_RealType1>&);

    private:
      param_type _M_param;

      std::gamma_distribution<result_type> _M_gda;
      std::gamma_distribution<result_type> _M_gdb;
    };

  /**
   * @brief Return true if two beta distributions are not equal.
   */
  template<typename _RealType>
    inline bool
    operator!=(const beta_distribution<_RealType>& __d1,
	       const beta_distribution<_RealType>& __d2)
    { return !(__d1 == __d2); }

}

#endif // __BETA_DISTRIBUTION

#ifndef __BETA_DISTRIBUTION_TCC
#define __BETA_DISTRIBUTION_TCC 1

namespace __gnu_cxx
{

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      typename beta_distribution<_RealType>::result_type
      beta_distribution<_RealType>::
      operator()(_UniformRandomNumberGenerator& __urng)
      {
	typedef std::gamma_distribution<result_type> __gamma_dist;
	typedef typename __gamma_dist::param_type __gamma_param;
	while (true)
	  {
	    result_type __x1 = _M_gda(__urng);
	    result_type __x2 = _M_gdb(__urng);
	    __x2 += __x1;
	    if ( __x2 != result_type() )
	      return __x1 / __x2;
	  }
      }

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      typename beta_distribution<_RealType>::result_type
      beta_distribution<_RealType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const param_type& __p)
      {
	typedef std::gamma_distribution<result_type> __gamma_dist;
	typedef typename __gamma_dist::param_type __gamma_param;
	while (true)
	  {
	    result_type __x1 = _M_gda(__urng,
				      __gamma_param(__p.alpha(), 1.0L));
	    result_type __x2 = _M_gdb(__urng,
				      __gamma_param(__p.beta(), 1.0L));
	    __x2 += __x1;
	    if ( __x2 != result_type() )
	      return __x1 / __x2;
	  }
      }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const beta_distribution<_RealType>& __x)
    {
      typedef std::basic_ostream<_CharT, _Traits>  __ostream_type;
      typedef typename __ostream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __os.flags();
      const _CharT __fill = __os.fill();
      const std::streamsize __precision = __os.precision();
      const _CharT __space = __os.widen(' ');
      __os.flags(__ios_base::scientific | __ios_base::left);
      __os.fill(__space);
      __os.precision(std::numeric_limits<_RealType>::digits10 + 1);

      __os << __x.alpha() << __space << __x.beta()
	   << __space << __x._M_gda
	   << __space << __x._M_gdb;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       beta_distribution<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __alpha_val, __beta_val;
      __is >> __alpha_val >> __beta_val >> __x._M_gda >> __x._M_gdb;
      __x.param(typename beta_distribution<_RealType>::
		param_type(__alpha_val, __beta_val));

      __is.flags(__flags);
      return __is;
    }

}

#endif // __BETA_DISTRIBUTION_TCC
