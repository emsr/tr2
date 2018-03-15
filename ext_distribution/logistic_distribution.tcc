#ifndef __LOGISTIC_DISTRIBUTION_TCC
#define __LOGISTIC_DISTRIBUTION_TCC 1

// This is already in __gnu_cxx
namespace std
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
