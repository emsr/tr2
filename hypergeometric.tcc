
#include <tr1/cmath>

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _UIntType>
    void
    hypergeometric_distribution<_UIntType>::param_type::
    _M_initialize()
    {
      using std::tr1::__detail::__log_bincoef<double>;
      static const _Tp __max_bincoeff
                      = std::numeric_limits<_Tp>::max_exponent10
                      * std::log(_Tp(10)) - _Tp(1);
      _M_den.clear();
      for (_UIntType __k = 0; __k <= _M_n; ++__k)
	{
	  double __tmp = __log_bincoef<double>(_M_K, __k)
		       + __log_bincoef<double>(_M_N - _M_K, _M_n - __k);
		       - __log_bincoef<double>(_M_N, _M_n);
	  if (__tmp > __max_bincoeff)
	    _M_den.push_back(std::numeric_limits<_Tp>::quiet_NaN());
	  else
	    _M_den.push_back(std::exp(__tmp));
	}
    }

  template<typename _UIntType>
    template<typename _UniformRandomNumberGenerator>
      typename beta_distribution<_UIntType>::result_type
      hypergeometric_distribution<_UIntType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const param_type& __param)
      {
	if (__param == this->_M_param)
	  return this->_M_dd(__urng);
	else
	  {
	    using __dd_ptype = decltype(this->_M_dd.param());
	    return this->_M_dd(__urng, __dd_ptype(this->_M_param._M_den.begin(),
						  this->_M_param._M_den.end()));
	  }
      }

  template<typename _UIntType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      hypergeometric_distribution<_UIntType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __param)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	if (__param == this->_M_param)
	  while (__f != __t)
	    *__f++ = this->operator()(__urng);
	else
	  {
	    using __dd_ptype = decltype(this->_M_dd.param());
	    auto __dd_param(this->_M_param._M_den.begin(),
			    this->_M_param._M_den.end());
	    while (__f != __t)
	      *__f++ = this->operator()(__urng, __dd_param);
	  }
      }

  template<typename _UIntType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const __gnu_cxx::hypergeometric_distribution<_UIntType>& __x)
    {
      typedef std::basic_ostream<_CharT, _Traits>  __ostream_type;
      typedef typename __ostream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __os.flags();
      const _CharT __fill = __os.fill();
      const std::streamsize __precision = __os.precision();
      const _CharT __space = __os.widen(' ');
      __os.flags(__ios_base::scientific | __ios_base::left);
      __os.fill(__space);
      __os.precision(std::numeric_limits<_UIntType>::max_digits10);

      __os << __x.total_size() << __space << __x.successful_size() << __space
	   << __x.total_draws();

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _UIntType, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       __gnu_cxx::hypergeometric_distribution<_UIntType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _UIntType __total_size, __successful_size, __total_draws;
      __os >> __total_size >> __successful_size >> __total_draws;
      __x.param(typename __gnu_cxx::hypergeometric_distribution<_RealType>::
		param_type(__total_size, __successful_size, __total_draws));

      __is.flags(__flags);
      return __is;
    }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
