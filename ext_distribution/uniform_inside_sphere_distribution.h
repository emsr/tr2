#ifndef __UNIFORM_INSIDE_SPHERE_DISTRIBUTION
#define __UNIFORM_INSIDE_SPHERE_DISTRIBUTION 1

#include <random>
#include <array>
#include <ext/random>

namespace __gnu_cxx //_GLIBCXX_VISIBILITY(default)
{
//_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @brief A distribution for random coordinates inside a unit sphere.
   */
  template<std::size_t _Dimen, typename _RealType = double>
    class uniform_inside_sphere_distribution
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");
      static_assert(_Dimen != 0, "dimension is zero");

    public:
      /** The type of the range of the distribution. */
      using result_type = std::array<_RealType, _Dimen>;

      /** Parameter type. */
      struct param_type
      {
	using distribution_type
	  = uniform_inside_sphere_distribution<_Dimen, _RealType>;
	friend class uniform_inside_sphere_distribution<_Dimen, _RealType>;

	explicit
	param_type(_RealType __radius = _RealType(1))
	: _M_radius(__radius)
	{
	  _GLIBCXX_DEBUG_ASSERT(_M_radius > _RealType(0));
	}

	_RealType
	radius() const
	{ return _M_radius; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return __p1._M_radius == __p2._M_radius; }

      private:
	_RealType _M_radius;
      };

      /**
       * @brief Constructors.
       */
      explicit
      uniform_inside_sphere_distribution(_RealType __radius = _RealType(1))
      : _M_param(__radius), _M_uosd()
      { }

      explicit
      uniform_inside_sphere_distribution(const param_type& __p)
      : _M_param(__p), _M_uosd()
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      { _M_uosd.reset(); }

      /**
       * @brief Returns the @f$radius@f$ of the distribution.
       */
      _RealType
      radius() const
      { return _M_param.radius(); }

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
       * This function makes no sense for this distribution.
       */
      result_type
      min() const
      {
	result_type __res;
	__res.fill(0);
	return __res;
      }

      /**
       * @brief Returns the least upper bound value of the distribution.
       * This function makes no sense for this distribution.
       */
      result_type
      max() const
      {
	result_type __res;
	__res.fill(0);
	return __res;
      }

      /**
       * @brief Generating functions.
       */
      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng)
	{ return this->operator()(__urng, _M_param); }

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
       * @brief Return true if two uniform on sphere distributions have
       *        the same parameters and the sequences that would be
       *        generated are equal.
       */
      friend bool
      operator==(const uniform_inside_sphere_distribution& __d1,
		 const uniform_inside_sphere_distribution& __d2)
      { return __d1._M_param == __d2._M_param && __d1._M_uosd == __d2._M_uosd; }

      /**
       * @brief Inserts a %uniform_inside_sphere_distribution random number
       *        distribution @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %uniform_inside_sphere_distribution random number
       *             distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<size_t _Dimen1, typename _RealType1, typename _CharT,
	       typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>& __os,
		   const __gnu_cxx::uniform_inside_sphere_distribution<_Dimen1,
								   _RealType1>&
		   );

      /**
       * @brief Extracts a %uniform_inside_sphere_distribution random number
       *        distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x  A %uniform_inside_sphere_distribution random number
       *             generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<std::size_t _Dimen1, typename _RealType1, typename _CharT,
	       typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>& __is,
		   __gnu_cxx::uniform_inside_sphere_distribution<_Dimen1,
								 _RealType1>&);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;
      uniform_on_sphere_distribution<_Dimen, _RealType> _M_uosd;
    };

  /**
   * @brief Return true if two uniform on sphere distributions are different.
   */
  template<std::size_t _Dimen, typename _RealType>
    inline bool
    operator!=(const __gnu_cxx::uniform_inside_sphere_distribution<_Dimen,
	       _RealType>& __d1,
	       const __gnu_cxx::uniform_inside_sphere_distribution<_Dimen,
	       _RealType>& __d2)
    { return !(__d1 == __d2); }

//_GLIBCXX_END_NAMESPACE_VERSION
} // namespace __gnu_cxx

#endif // __UNIFORM_INSIDE_SPHERE_DISTRIBUTION

#ifndef __UNIFORM_INSIDE_SPHERE_DISTRIBUTION_TCC
#define __UNIFORM_INSIDE_SPHERE_DISTRIBUTION_TCC 1

namespace __gnu_cxx //_GLIBCXX_VISIBILITY(default)
{
//_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace {

    // Helper class for the uniform_inside_sphere_distribution generation
    // function.
    template<std::size_t _Dimen, bool _SmallDimen, typename _RealType>
      class uniform_inside_sphere_helper;

    template<std::size_t _Dimen, typename _RealType>
      class uniform_inside_sphere_helper<_Dimen, false, _RealType>
      {
	using result_type
	  = typename uniform_inside_sphere_distribution<_Dimen, _RealType>::
	    result_type;

      public:
	template<typename _UniformOnSphereDistribution,
		 typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformOnSphereDistribution& __uosd,
		   _UniformRandomNumberGenerator& __urng,
		   _RealType __radius)
        {
	  std::__detail::_Adaptor<_UniformRandomNumberGenerator,
				  _RealType> __aurng(__urng);

	  _RealType __pow = 1 / _RealType(_Dimen);
	  _RealType __urt = __radius * std::pow(__aurng(), __pow);
	  result_type __ret = __uosd(__aurng);

	  std::transform(__ret.begin(), __ret.end(), __ret.begin(),
			 [__urt](_RealType __val)
			 { return __val * __urt; });

	  return __ret;
        }
      };

    // Helper class for the uniform_inside_sphere_distribution generation
    // function specialized for small dimensions.
    template<std::size_t _Dimen, typename _RealType>
      class uniform_inside_sphere_helper<_Dimen, true, _RealType>
      {
	using result_type
	  = typename uniform_inside_sphere_distribution<_Dimen, _RealType>::
	    result_type;

      public:
	template<typename _UniformOnSphereDistribution,
		 typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformOnSphereDistribution&,
		   _UniformRandomNumberGenerator& __urng,
		   _RealType __radius)
        {
	  result_type __ret;
	  _RealType __sq;
	  _RealType __radsq = __radius * __radius;
	  std::__detail::_Adaptor<_UniformRandomNumberGenerator,
				  _RealType> __aurng(__urng);

	  do
	    {
	      __sq = _RealType(0);
	      for (int i = 0; i < _Dimen; ++i)
		{
		  __ret[i] = _RealType(2) * __aurng() - _RealType(1);
		  __sq += __ret[i] * __ret[i];
		}
	    }
	  while (__sq > _RealType(1));

	  for (int i = 0; i < _Dimen; ++i)
            __ret[i] *= __radius;

	  return __ret;
        }
      };
  } // namespace

  //
  //  Experiments have shown that rejection is more efficient than transform
  //  for dimensions less than 8.
  //
  template<std::size_t _Dimen, typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      typename uniform_inside_sphere_distribution<_Dimen, _RealType>::result_type
      uniform_inside_sphere_distribution<_Dimen, _RealType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const param_type& __p)
      {
        uniform_inside_sphere_helper<_Dimen, _Dimen < 8, _RealType> __helper;
        return __helper(_M_uosd, __urng, __p.radius());
      }

  template<std::size_t _Dimen, typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      uniform_inside_sphere_distribution<_Dimen, _RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __param)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	while (__f != __t)
	  *__f++ = this->operator()(__urng, __param);
      }

  template<std::size_t _Dimen, typename _RealType, typename _CharT,
	   typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const __gnu_cxx::uniform_inside_sphere_distribution<_Dimen,
								_RealType>& __x)
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

      __os << __x.radius() << __space << __x._M_uosd;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);

      return __os;
    }

  template<std::size_t _Dimen, typename _RealType, typename _CharT,
	   typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       __gnu_cxx::uniform_inside_sphere_distribution<_Dimen,
							     _RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __radius_val;
      __is >> __radius_val >> __x._M_uosd;
      __x.param(typename uniform_inside_sphere_distribution<_Dimen, _RealType>::
		param_type(__radius_val));

      __is.flags(__flags);

      return __is;
    }

//_GLIBCXX_END_NAMESPACE_VERSION
} // namespace __gnu_cxx

#endif // __UNIFORM_INSIDE_SPHERE_DISTRIBUTION_TCC
