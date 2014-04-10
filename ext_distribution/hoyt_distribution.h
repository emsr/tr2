#ifndef __HOYT_DISTRIBUTION
#define __HOYT_DISTRIBUTION 1

#include <random>
#include "arcsine_distribution.h"

namespace __gnu_cxx
{

  /**
   * @brief A Hoyt continuous distribution for random numbers.
   *
   * The formula for the Hoyt probability density function is
   * @f[
   *     p(x|q,\omega) = \frac{(1 + q^2)x}{q\omega}
   *                     \exp\left(-\frac{(1 + q^2)^2 x^2}{4 q^2 \omega}\right)
   *                       I_0\left(\frac{(1 - q^4) x^2}{4 q^2 \omega}\right)
   * @f]
   * where @f$I_0(z)@f$ is the modified Bessel function of the first kind
   * of order 0 and @f$0 < q < 1@f$.
   *
   * <table border=1 cellpadding=10 cellspacing=0>
   * <caption align=top>Distribution Statistics</caption>
   * <tr><td>Mean</td><td>@f$ \sqrt{\frac{2}{\pi}} \sqrt{\frac{\omega}{1 + q^2}}
   *                       E(1 - q^2) @f$</td></tr>
   * <tr><td>Variance</td><td>@f$ \omega \left(1 - \frac{2E^2(1 - q^2)}
   *                                      {\pi (1 + q^2)}\right) @f$</td></tr>
   * <tr><td>Range</td><td>@f$[0, \infty)@f$</td></tr>
   * </table>
   * where @f$E(x)@f$ is the elliptic function of the second kind.
   */
  template<typename _RealType = double>
    class
    hoyt_distribution
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      typedef _RealType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef hoyt_distribution<result_type> distribution_type;

	param_type(result_type __q = result_type(0.5L),
		   result_type __omega = result_type(1))
	: _M_q(__q), _M_omega(__omega)
	{
	  _GLIBCXX_DEBUG_ASSERT(_M_q > result_type(0));
	  _GLIBCXX_DEBUG_ASSERT(_M_q < result_type(1));
	}

	result_type
	q() const
	{ return _M_q; }

	result_type
	omega() const
	{ return _M_omega; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_q == __p2._M_q
              && __p1._M_omega == __p2._M_omega; }

      private:
	void _M_initialize();

	result_type _M_q;
	result_type _M_omega;
      };

      /**
       * @brief Constructors.
       */
      explicit
      hoyt_distribution(result_type __q = result_type(0.5L),
			result_type __omega = result_type(1))
      : _M_param(__q, __omega),
	_M_ad(result_type(0.5L) * (result_type(1) + __q * __q),
	      result_type(0.5L) * (result_type(1) + __q * __q)
				/ (__q * __q)),
	_M_ed(result_type(1))
      { }

      explicit
      hoyt_distribution(const param_type& __p)
      : _M_param(__p),
	_M_ad(result_type(0.5L) * (result_type(1) + __p.q() * __p.q()),
	      result_type(0.5L) * (result_type(1) + __p.q() * __p.q())
				/ (__p.q() * __p.q())),
	_M_ed(result_type(1))
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      {
	_M_ad.reset();
	_M_ed.reset();
      }

      /**
       * @brief Return the parameters of the distribution.
       */
      result_type
      q() const
      { return _M_param.q(); }

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
       * @brief Return true if two Hoyt distributions have
       *        the same parameters and the sequences that would
       *        be generated are equal.
       */
      friend bool
      operator==(const hoyt_distribution& __d1,
		 const hoyt_distribution& __d2)
      { return (__d1.param() == __d2.param()
		&& __d1._M_ad == __d2._M_ad
		&& __d1._M_ed == __d2._M_ed); }

      /**
       * @brief Inserts a %hoyt_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %hoyt_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>&,
		   const hoyt_distribution<_RealType1>&);

      /**
       * @brief Extracts a %hoyt_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x A %hoyt_distribution random number
       *            generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>&,
		   hoyt_distribution<_RealType1>&);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;

      __gnu_cxx::arcsine_distribution<result_type> _M_ad;
      std::exponential_distribution<result_type> _M_ed;
    };

  /**
   * @brief Return true if two Hoyt distributions are not equal.
   */
  template<typename _RealType>
    inline bool
    operator!=(const hoyt_distribution<_RealType>& __d1,
	       const hoyt_distribution<_RealType>& __d2)
    { return !(__d1 == __d2); }

}

#endif // __HOYT_DISTRIBUTION

#ifndef __HOYT_DISTRIBUTION_TCC
#define __HOYT_DISTRIBUTION_TCC 1

namespace __gnu_cxx
{

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      result_type
      hoyt_distribution<_RealType>::
      operator()(_UniformRandomNumberGenerator& __urng)
      {
	result_type __x = this->_M_ad(__urng);
	result_type __y = this->_M_ed(__urng);
	return (result_type(2) * this->q()
		  / (result_type(1) + this->q() * this->q()))
	       * std::sqrt(this->omega() * __x * __y);
      }

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      result_type
      hoyt_distribution<_RealType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const param_type& __p)
      {
	result_type __q2 = __p.q() * __p.q();
	result_type __num = result_type(0.5L) * (result_type(1) + __q2);
	typename __gnu_cxx::arcsine_distribution<result_type>::param_type
	  __pa(__num, __num / __q2);
	result_type __x = this->_M_ad(__pa, __urng);
	result_type __y = this->_M_ed(__urng);
	return (result_type(2) * __p.q() / (result_type(1) + __q2))
	       * std::sqrt(__p.omega() * __x * __y);
      }

  template<typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      hoyt_distribution<_RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __p)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	result_type __2q = result_type(2) * __p.q();
	result_type __q2 = __p.q() * __p.q();
	result_type __q2p1 = result_type(1) + __q2;
	result_type __num = result_type(0.5L) * __q2p1;
	result_type __omega = __p.omega();
	typename __gnu_cxx::arcsine_distribution<result_type>::param_type
	  __pa(__num, __num / __q2);
	while (__f != __t)
	  {
	    result_type __x = this->_M_ad(__pa, __urng);
	    result_type __y = this->_M_ed(__urng);
	    *__f++ = (__2q / __q2p1) * std::sqrt(__omega * __x * __y);
	  }
      }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const hoyt_distribution<_RealType>& __x)
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

      __os << __x.q() << __space << __x.omega();
      __os << __space << __x._M_ad;
      __os << __space << __x._M_ed;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       hoyt_distribution<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __q, __omega;
      __is >> __q >> __omega;
      __is >> __x._M_ad;
      __is >> __x._M_ed;
      __x.param(typename hoyt_distribution<_RealType>::
		param_type(__q, __omega));

      __is.flags(__flags);
      return __is;
    }

}

#endif // __HOYT_DISTRIBUTION_TCC
