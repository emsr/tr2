#pragma GCC system_header

namespace __old //_GLIBCXX_VISIBILITY(default)
{
//_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace {

    // Helper class for the uniform_inside_sphere_distribution generation
    // function.
    template<std::size_t _Dimen, typename _RealType>
      class uniform_inside_sphere_helper
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

  }


  template<std::size_t _Dimen, typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      typename uniform_inside_sphere_distribution<_Dimen, _RealType>::result_type
      uniform_inside_sphere_distribution<_Dimen, _RealType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const param_type& __p)
      {
        uniform_inside_sphere_helper<_Dimen, _RealType> __helper;
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
	       const __old::uniform_inside_sphere_distribution<_Dimen,
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

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os << __x.radius() << __space << __x._M_uosd;
    }

  template<std::size_t _Dimen, typename _RealType, typename _CharT,
	   typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       __old::uniform_inside_sphere_distribution<_Dimen,
							     _RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __radius_val;
      __is >> __radius_val;
      __is >> __x._M_uosd;
      __x.param(typename uniform_inside_sphere_distribution<_Dimen, _RealType>::
		param_type(__radius_val));

      __is.flags(__flags);
      return __is;
    }

//_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
