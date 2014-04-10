#ifndef __RICE_DISTRIBUTION_MULTI
#define __RICE_DISTRIBUTION_MULTI 1


#include <vector>
#include <random>
#include <limits>
#include <tr1/cmath>


  /**
   * @brief A generalized Rice distribution.
   */
  template<typename _RealType = double>
    class
    rice_density
    {

      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:

      typedef _RealType result_type;

      rice_density(const std::vector<result_type>& __mean, result_type __sigma);

      result_type operator()(result_type __x);

      std::vector<result_type>
      mean() const
      { return _M_mean; }

      result_type
      sigma() const
      { return _M_sigma; }

    private:
      std::vector<result_type> _M_mean;
      result_type _M_sigma;
      result_type _M_s;
    };

  /**
   * @brief Rice density constructor.
   */
  template<typename _RealType>
    rice_density<_RealType>::rice_density(const std::vector<_RealType>& __mean, _RealType __sigma)
    : _M_mean(__mean), _M_sigma(__sigma)
    {
      _M_s = result_type(0);
      for ( std::size_t __m = 0; __m < _M_mean.size(); ++__m)
        _M_s += _M_mean[__m] * _M_mean[__m];
      _M_s = std::sqrt(_M_s);
    }

  /**
   * @brief 
   */
  template<typename _RealType>
    _RealType
    rice_density<_RealType>::operator()(_RealType __x)
    {
      result_type __mu = _M_mean.size() / result_type(2);
      result_type __nu = __mu - result_type(1);
      result_type __sig2 = _M_sigma *_M_sigma;
      result_type __fact = std::pow(__x, __mu) / (__sig2 * std::pow(_M_s, __nu));
      return __fact * std::exp(-(__x * __x + _M_s * _M_s) / (2.0 * __sig2))
                    * std::tr1::cyl_bessel_i(__nu, __x * _M_s / __sig2);
    }

  /**
   * @brief 
   */
  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const rice_density<_RealType>& __x)
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

      const auto& __mean = __x.mean();
      size_t __n = __mean.size();
      __os << __n;
      for (size_t __i = 0; __i < __n; ++__i)
	__os << __space << __mean[__i];
      __os << __space << __x.sigma();

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

//
// @brief The Marcum Q function.
//
template<typename _Tp>
  _Tp
  marcum_q(unsigned int __m, _Tp __a, _Tp __b)
  {
    if ( __b == _Tp(0))
      return _Tp(1);

    _Tp __q1 = _Tp(0);
    const _Tp __arg = __a * __b;
    const _Tp __ab = __a / __b;
    _Tp __temp = _Tp(1);
    unsigned int __k = 0;
    while (true)
      {
	_Tp __term = __temp * std::tr1::cyl_bessel_i(__k, __arg);
	__q1 += __term;
	if (std::abs(__term) < std::numeric_limits<_Tp>::epsilon())
          break;
	__temp *= __ab;
	++__k;
      }

    _Tp __q = __q1;

    const _Tp __ba = __b / __a;
    __temp = _Tp(1);
    for (unsigned int __k = 1; __k < __m; ++__k)
      {
	__temp *= __ba;
	__q += __temp * std::tr1::cyl_bessel_i(__k, __arg);
      }

    __q *= std::exp(-(__a * __a + __b * __b) / 2.0);

    return __q;
  }


  /**
   *   Build up a higher order Ricean distribution by sampling a pdf function
   *   and making a piecewise linear approximation.
   */
  template<typename _RealType = double>
    class
    rice_distribution_multi
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      typedef _RealType result_type;
      /** Parameter type. */
      struct param_type
      {
	typedef rice_distribution_multi<result_type> distribution_type;

	template<typename _InputIteratorM>
	  param_type(_InputIteratorM __mbegin,
		     _InputIteratorM __mend,
		     result_type __sigma);

	param_type(std::initializer_list<result_type> __ml, result_type __sigma);

	std::vector<result_type>
	mean() const
	{ return _M_mean; }

	result_type
	sigma() const
	{ return _M_sigma; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_mean == __p2._M_mean
              && __p1._M_sigma == __p2._M_sigma; }

      private:
	void _M_initialize();

	std::vector<result_type> _M_mean;
	result_type _M_sigma;
      };

      explicit
      rice_distribution_multi(result_type __mean = result_type(0),
			result_type __sigma = result_type(1))
      : _M_param(__mean, __sigma), _M_pld()
      { }

      explicit
      rice_distribution_multi(const param_type& __p)
      : _M_param(__p), _M_pld()
      { }

      /**
       * Resets the distribution state.
       */
      void
      reset()
      { _M_pld.reset(); }

      /**
       *
       */
      std::vector<result_type>
      mean() const
      { return _M_param.mean(); }

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
	{ return this->_M_pld(__urng); }

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p)
        { return std::exp(__p.s() * _M_pld(__urng) + __p.m()); }

      /**
       * @brief Return true if two Rice distributions have
       *        the same parameters and the sequences that would
       *        be generated are equal.
       */
      template<typename _RealType1>
        friend bool
        operator==(const rice_distribution_multi<_RealType1>& __d1,
		   const rice_distribution_multi<_RealType1>& __d2)
        { return (__d1.param() == __d2.param()
		  && __d1._M_pld == __d2._M_pld); }

      /**
       * @brief Inserts a %rice_distribution_multi random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %rice_distribution_multi random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>&,
		   const rice_distribution_multi<_RealType1>&);

      /**
       * @brief Extracts a %rice_distribution_multi random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x A %rice_distribution_multi random number
       *            generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>&,
		   rice_distribution_multi<_RealType1>&);

    private:
      param_type _M_param;

      std::piecewise_linear_distribution<result_type> _M_pld;
    };

  /**
   *
   */
  template<typename _RealType>
    rice_distribution_multi<_RealType>::param_type::
    param_type(std::initializer_list<_RealType> __ml, _RealType __sigma)
    {
    }

  /**
   *
   */
  template<typename _RealType>
    template<typename _InputIteratorM>
      rice_distribution_multi<_RealType>::param_type::
      param_type(_InputIteratorM __mbegin,
                 _InputIteratorM __mend, _RealType __sigma)
      {
      }

  /**
   * @brief Write a generalized Rice distribution to an output stream.
   * @param __is The output stream.
   * @param __x The generalized Rice distribution.
   * @return The output stream.
   */
  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const rice_distribution_multi<_RealType>& __x)
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

      auto& __mean = __x.mean();
      size_t __n = __mean.size();
      __os << __n;
      for (size_t __i = 0; __i < __n; ++__i)
	__os << __space << __mean[__i];
      __os << __space << __x.sigma();
      __os << __space << __x._M_pld;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  /**
   * @brief Read a generalized Rice distribution from an input stream.
   * @param __is The input stream.
   * @param __x The generalized Rice distribution.
   * @return The input stream.
   */
  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       rice_distribution_multi<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      size_t __n;
      __is >> __n;

      std::vector<_RealType> __mean;
      __mean.reserve(__n);
      for (size_t __i = 0; __i < __n; ++__i)
	{
	  _RealType __m;
          __is >> __m;
          __mean.push_back(__m);
        }
      _RealType __sigma;
      __is >> __sigma;
      __is >> __x._M_pld;
      __x.param(typename rice_distribution_multi<_RealType>::
		param_type(__mean, __sigma));

      __is.flags(__flags);
      return __is;
    }

#endif // __RICE_DISTRIBUTION_TCC_MULTI
