#ifndef VON_MISES_FISHER_DISTRIBUTION_TCC
#define VON_MISES_FISHER_DISTRIBUTION_TCC 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#include <iostream>

#ifdef _GLIBCXX_USE_C99_STDINT_TR1

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace __detail
  {
    template<typename _RealType, std::size_t _Dim>
      void
      __make_normal(std::array<_RealType, _Dim> &__lambda)
      {
	_RealType __len{};
	for (int i = 0; i < _Dim; ++i)
	  __len += __lambda[i] * __lambda[i];
	__len = std::sqrt(__len);
	for (int i = 0; i < _Dim; ++i)
	  __lambda[i] /=__len;
      }

    template<typename _RealType, std::size_t _Dim>
      void
      __make_basis(const std::array<_RealType, _Dim>& __mu,
		   std::array<std::array<_RealType, _Dim>, _Dim - 1>& __lambda)
      {
	size_t __max = 0;
	//  Find the mu pivot element as the largest abs element.
	for (size_t __i = 1; __i < _Dim; ++__i)
	  if (std::abs(__mu[__i]) > std::abs(__mu[__max]))
	    __max = __i;
	//  The orthogonal lambdas are built with 1 in all te slots other than
	//  the pivot and are orthonormalized wrt pu and teh previous lambdas.
	for (size_t __i = 0; __i < _Dim - 1; ++__i)
	  {
	    __lambda[__i][(__max + __i) % _Dim] = _RealType(1);
	    auto __mudot = __mu[(__max + __i) % _Dim];
	    if (__mudot != _RealType(0))
	      for (size_t __j = 0; __j < _Dim; ++__j)
		__lambda[__i][__j] -= __mudot * __mu[__j];
		for (size_t __k = 0; __k < __i; ++__k)
		  {
		    auto __lambdot = __lambda[__k][(__max + __i) % _Dim];
		    if (__lambdot != _RealType(0))
		      for (size_t __j = 0; __j < _Dim; ++__j)
			__lambda[__i][__j] -= __lambdot * __lambda[__k][__j];
		  }
	    __make_normal(__lambda[__i]);
	  }
      }
  }

  template<std::size_t _Dim, typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      typename von_mises_fisher_distribution<_Dim, _RealType>::result_type
      von_mises_fisher_distribution<_Dim, _RealType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const typename von_mises_fisher_distribution<_Dim, _RealType>::param_type& __p)
      {
	std::__detail::_Adaptor<_UniformRandomNumberGenerator, typename result_type::value_type>
	  __aurng(__urng);

	//  The beta distribution depends only on _Dim.

	typename result_type::value_type __W;
	while (true)
	  {
	    auto __z = this->_M_bd(__urng);
	    __W = (1 - (1 + __p._M_b) * __z) / (1 - (1 - __p._M_b) * __z);
	    auto __thing = __p._M_kappa * __W
			 + __p._M_Dim * std::log(1 - __p._M_x * __W) - __p._M_c;
	    if (__thing >= std::log(__aurng()))
	      break;
	  }
	auto __rt = std::sqrt(1 - __W * __W);
	auto __V = _M_uosd(__urng);

	result_type __res;
	for (int __i = 0; __i < __res.size(); ++__i)
	  __res[__i] = __W * __p._M_mu[__i];
	for (int __k = 0; __k < __res.size() - 1; ++__k)
	  for (int __i = 0; __i < __res.size(); ++__i)
	    __res[__i] += __rt * __V[__k] * __p._M_lambda[__k][__i];

	return __res;
      }

  template<std::size_t _Dim, typename _RealType>
    template<typename _OutputIterator,
	     typename _UniformRandomNumberGenerator>
      void
      von_mises_fisher_distribution<_Dim, _RealType>::
      __generate_impl(_OutputIterator __f, _OutputIterator __t,
		      _UniformRandomNumberGenerator& __urng,
		      const typename von_mises_fisher_distribution<_Dim, _RealType>::param_type& __param)
      {
	__glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator>)

	while (__f != __t)
	  *__f++ = this->operator()(__urng, __param);
      }

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      typename von_mises_fisher_distribution<2, _RealType>::result_type
      von_mises_fisher_distribution<2, _RealType>::
      operator()(_UniformRandomNumberGenerator& __urng)
      {
	using __res_t = typename von_mises_distribution<_RealType>::result_type;
	__res_t __vmd_res = _M_vmd(__urng);

	result_type __res;
	__res[0] = std::cos(__vmd_res);
	__res[1] = std::sin(__vmd_res);

	return __res;
      }

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      typename von_mises_fisher_distribution<2, _RealType>::result_type
      von_mises_fisher_distribution<2, _RealType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const typename von_mises_fisher_distribution<2, _RealType>::param_type& __p)
      {
	using __parm_t = typename von_mises_distribution<_RealType>::param_type;
	using __res_t = typename von_mises_distribution<_RealType>::result_type;
	__parm_t __parm(__p._M_theta0);
	__res_t __vmd_res = _M_vmd(__urng, __parm);

	result_type __res;
	__res[0] = std::cos(__vmd_res);
	__res[1] = std::sin(__vmd_res);

	return __res;
      }

  template<typename _RealType>
    template<typename _UniformRandomNumberGenerator>
      typename von_mises_fisher_distribution<3, _RealType>::result_type
      von_mises_fisher_distribution<3, _RealType>::
      operator()(_UniformRandomNumberGenerator& __urng,
		 const typename von_mises_fisher_distribution<3, _RealType>::param_type& __p)
      {
	std::__detail::_Adaptor<_UniformRandomNumberGenerator,
				typename result_type::value_type>
	  __aurng(__urng);

	auto __xi = __aurng();
	auto __W = 1 + std::log(__xi + (1 - __xi) * std::exp(-2 * __p.kappa()))
		 / __p.kappa();
	auto __rt = std::sqrt(1 - __W * __W);
	auto __V = _M_uosd(__urng);

	result_type __res;
	for (int __i = 0; __i < 3; ++__i)
	  __res[__i] = __W * __p._M_mu[__i]
		     + __rt * __V[0] * __p._M_lambda[0][__i]
		     + __rt * __V[1] * __p._M_lambda[1][__i];

	return __res;
      }


  template<std::size_t _Dim, typename _RealType,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const von_mises_fisher_distribution<_Dim, _RealType>& __x)
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

      for (auto __k : __x.mu())
	__os << __k << __space;
      __os << __x.kappa();

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const von_mises_fisher_distribution<2, _RealType>& __x)
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

      __os << __x.mu()[0] << __space << __x.mu()[1];
      __os << __space << __x.kappa();

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<typename _RealType,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const von_mises_fisher_distribution<3, _RealType>& __x)
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

      __os << __x.mu()[0] << __space << __x.mu()[1] << __space << __x.mu()[2];
      __os << __space << __x.kappa();

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);
      return __os;
    }

  template<std::size_t _Dim, typename _RealType,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       von_mises_fisher_distribution<_Dim, _RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      std::array<_RealType, _Dim> __mu;
      __is >> __mu;
      _RealType __kappa;
      __is >> __kappa;
      __x.param(typename von_mises_fisher_distribution<_Dim, _RealType>::
		param_type(__mu, __kappa));

      __is.flags(__flags);
      return __is;
    }

  template<typename _RealType,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       von_mises_fisher_distribution<2, _RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      std::array<_RealType, 2> __mu;
      __is >> __mu[0] >> __mu[1];
      _RealType __kappa;
      __is >> __kappa;
      __x.param(typename von_mises_fisher_distribution<2, _RealType>::
		param_type(__mu, __kappa));

      __is.flags(__flags);
      return __is;
    }

  template<typename _RealType,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       von_mises_fisher_distribution<3, _RealType>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      std::array<_RealType, 3> __mu;
      __is >> __mu[0] >> __mu[1] >> __mu[2];
      _RealType __kappa;
      __is >> __kappa;
      __x.param(typename von_mises_fisher_distribution<3, _RealType>::
		param_type(__mu, __kappa));

      __is.flags(__flags);
      return __is;
    }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif // _GLIBCXX_USE_C99_STDINT_TR1

#endif // C++11

#endif // _VON_MISES_FISHER_DISTRIBUTION_TCC
