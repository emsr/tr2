

  template<typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      fermi_dirac_distribution<_RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const param_type& __p)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	while (__f != __t)
	  {
	    typename std::normal_distribution<result_type>::param_type
	      __px(__p.beta(), __p.mu()), __py(result_type(0), __p.mu());
	    result_type __x = this->_M_ndx(__px, __urng);
	    result_type __y = this->_M_ndy(__py, __urng);
#if _GLIBCXX_USE_C99_MATH_TR1
	    *__f++ = std::hypot(__x, __y);
#else
	    *__f++ = std::sqrt(__x * __x + __y * __y);
#endif
	  }
      }

  template<typename _RealType, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const fermi_dirac_distribution<_RealType>& __x)
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

      __os << __x.beta() << __space << __x.mu();
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
	       fermi_dirac_distribution<_RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      _RealType __beta_val, __mu_val;
      __is >> __beta_val >> __mu_val;
      __is >> __x._M_ndx;
      __is >> __x._M_ndy;
      __x.param(typename fermi_dirac_distribution<_RealType>::
		param_type(__beta_val, __mu_val));

      __is.flags(__flags);
      return __is;
    }
