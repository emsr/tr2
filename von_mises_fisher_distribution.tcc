#include <iostream>


  template<typename _RealType, std::size_t _Dim>
    template<typename _UniformRandomNumberGenerator>
      typename von_mises_fisher_distribution<_RealType, _Dim>::result_type
      von_mises_fisher_distribution<_RealType, _Dim>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const typename von_mises_fisher_distribution<_RealType, _Dim>::param_type& __p)
      {
	const result_type __pi
	  = __gnu_cxx::__math_constants<result_type>::__pi;
	std::__detail::_Adaptor<_UniformRandomNumberGenerator, result_type>
	  __aurng(__urng);

	result_type __f;
	while (1)
	  {
	    result_type __rnd = std::cos(__pi * __aurng());
	    __f = (result_type(1) + __p._M_r * __rnd) / (__p._M_r + __rnd);
	    result_type __c = __p._M_kappa * (__p._M_r - __f);

	    result_type __rnd2 = __aurng();
	    if (__c * (result_type(2) - __c) > __rnd2)
	      break;
	    if (std::log(__c / __rnd2) >= __c - result_type(1))
	      break;
	  }

	result_type __res = std::acos(__f);
#if _GLIBCXX_USE_C99_MATH_TR1
	__res = std::copysign(__res, __aurng() - result_type(0.5));
#else
	if (__aurng() < result_type(0.5))
	  __res = -__res;
#endif
	__res += __p._M_mu;
	if (__res > __pi)
	  __res -= result_type(2) * __pi;
	else if (__res < -__pi)
	  __res += result_type(2) * __pi;
	return __res;
      }

  template<typename _RealType, std::size_t _Dim>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      von_mises_fisher_distribution<_RealType, _Dim>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const typename von_mises_fisher_distribution<_RealType, _Dim>::param_type& __param)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	while (__f != __t)
	  *__f++ = this->operator()(__urng, __param);
      }

  template<typename _RealType, std::size_t _Dim,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const von_mises_fisher_distribution<_RealType, _Dim>& __x)
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

      __os << __x.mu();
      __os << __space << __x.kappa();

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType, std::size_t _Dim,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       von_mises_fisher_distribution<_RealType, _Dim>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      std::array<_RealType, _Dim> __mu;
      __is >> __mu;
      _RealType __kappa;
      __is >> __kappa;
      __x.param(typename von_mises_fisher_distribution<_RealType, _Dim>::
		param_type(__mu, __kappa));

      __is.flags(__flags);
      return __is;
    }
