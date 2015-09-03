#include <array>
#include <type_traits>
#include <ext/random>
#include <ext/cmath>


#ifndef VON_MISES_FISHER_DISTRIBUTION_H
#define VON_MISES_FISHER_DISTRIBUTION_H 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#ifdef _GLIBCXX_USE_C99_STDINT_TR1

namespace __gnu_cxx _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace __detail
  {
    template<std::size_t _Dim, typename _RealType>
      constexpr _RealType
      __modulus(const std::array<_RealType, _Dim> & __arr)
      {
	_RealType __mod = 0;
        for (__comp : __arr)
	  __mod += __comp * __comp;
	return std::sqrt(__mod);
      }

    template<typename _RealType, std::size_t _Dim>
      void
      __make_basis(const std::array<_RealType, _Dim>& __mu,
		   std::array<std::array<_RealType, _Dim>, _Dim - 1>& __lambda);
  }

  /**
   * @brief A von Mises - Fisher distribution for random directions.
   *
   * The formula for the von Mises - Fisher probability density
   * function for dimension \f$ D \f$ is
   * @f[
   *     p_D(\bold{x}|\bold{\mu},\kappa) = C_D(\kappa)
   *                                       e^{\kappa \bold{\mu} \cdot \bold{x}}
   * @f]
   * where the normalization factor is
   * @f[
   *    C_D(\kappa) = \frac{\kappa^{D/2-1}}{(2\pi)^{D/2} I_{D/2-1}(\kappa)}
   * @f]
   * The generating functions use the method according to:
   *
   * @see "Generating von Mises - Fisher distribution on the unit sphere (S^2)"
   *      Sungkyu Jung, Tech. rep., University of Pittsburgh, October 12, 2009
   * @see "Numerically stable sampling of the von Mises Fisher distribution
   *      on S^2 (and other tricks)", Wenzel Jakob, 
   *
   * <table border=1 cellpadding=10 cellspacing=0>
   * <caption align=top>Distribution Statistics</caption>
   * <tr><td>Mean</td><td>@f$ \bold{\mu} @f$</td></tr>
   * <tr><td>Variance</td><td>@f$  @f$</td></tr>
   * <tr><td>Range</td><td>@f$[-\pi, \pi]@f$</td></tr>
   * </table>
   */
  template<std::size_t _Dim, typename _RealType = double>
    class von_mises_fisher_distribution
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");
      static_assert(_Dim >= 2,
		    "dimension template argument must be greater than one");

    public:
      /** The type of the range of the distribution. */
      using result_type = std::array<_RealType, _Dim>;
      /** Parameter type. */
      struct param_type
      {
	friend class von_mises_fisher_distribution<_Dim, _RealType>;

	explicit
	param_type(std::array<_RealType, _Dim> __mu/* = make_array(1, 0...)*/,
		   _RealType __kappa = _RealType(1))
	: _M_mu(__mu),
	  _M_kappa(__kappa)
	{
	  _GLIBCXX_DEBUG_ASSERT(std::abs(__detail::__modulus(_M_mu) - 1)
				 < std::numeric_limits<_RealType>::epsilon());
	  _GLIBCXX_DEBUG_ASSERT(_M_kappa >= _RealType(0));

	  this->_M_Dim = _RealType(_Dim - 1);
	  auto __tau = std::sqrt(4 * this->_M_kappa * this->_M_kappa
				   + this->_M_Dim * this->_M_Dim);
	  this->_M_b = _M_Dim / (__tau + 2 * this->_M_kappa);
	  this->_M_x = (1 - this->_M_b) / (1 + this->_M_b);

	  this->_M_c = this->_M_kappa * this->_M_x
		     + this->_M_Dim * std::log(1 - this->_M_x * this->_M_x);

	  __detail::__make_basis(this->_M_mu, this->_M_lambda);
	}

	std::array<_RealType, _Dim>
	mu() const
	{ return this->_M_mu; }

	_RealType
	kappa() const
	{ return this->_M_kappa; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return (__p1._M_mu == __p2._M_mu
		  && __p1._M_kappa == __p2._M_kappa); }

      private:
	std::array<_RealType, _Dim> _M_mu;
	std::array<std::array<_RealType, _Dim>, _Dim - 1> _M_lambda;
	_RealType _M_kappa;
	_RealType _M_Dim;
	_RealType _M_b;
	_RealType _M_c;
	_RealType _M_x;
      };

      /**
       * @brief Constructs a von Mises - Fisher distribution with parameters
       * @f$\bold{\mu}@f$ and @f$\kappa@f$.
       */
      explicit
      von_mises_fisher_distribution(result_type __mu/* = make_array(1, 0, 0)*/,
				    typename result_type::value_type __kappa
						= result_type::value_type(1))
      : _M_param(__mu, __kappa),
	_M_uosd(),
	_M_bd(_RealType(_Dim - 1) / 2, _RealType(_Dim - 1) / 2)
      { }

      explicit
      von_mises_fisher_distribution(const param_type& __p)
      : _M_param(__p),
	_M_uosd(),
	_M_bd(_RealType(_Dim - 1) / 2, _RealType(_Dim - 1) / 2)
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      {
	this->_M_uosd.reset();
	this->_M_bd.reset();
      }

      /**
       * @brief Returns the @f$ \bold{\mu} @f$ of the distribution.
       */
      result_type
      mu() const
      { return this->_M_param.mu(); }

      /**
       * @brief Returns the @f$ \kappa @f$ of the distribution.
       */
      typename result_type::value_type
      kappa() const
      { return this->_M_param.kappa(); }

      /**
       * @brief Returns the parameter set of the distribution.
       */
      param_type
      param() const
      { return this->_M_param; }

      /**
       * @brief Sets the parameter set of the distribution.
       * @param __param The new parameter set of the distribution.
       */
      void
      param(const param_type& __param)
      { this->_M_param = __param; }

      /**
       * @brief Returns the greatest lower bound value of the distribution.
       */
      result_type
      min() const
      {
	constexpr auto __minv = -std::numeric_limits<_RealType>::max();
        std::array<_RealType, _Dim> __arr;
        __arr.fill(__minv);
	return __arr;
      }

      /**
       * @brief Returns the least upper bound value of the distribution.
       */
      result_type
      max() const
      {
	constexpr auto __maxv = std::numeric_limits<_RealType>::max();
        std::array<_RealType, _Dim> __arr;
        __arr.fill(__maxv);
	return __arr;
      }

      /**
       * @brief Generating functions.
       */
      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng)
	{ return this->operator()(__urng, this->_M_param); }

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p);

      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate(_ForwardIterator __f, _ForwardIterator __t,
		   _UniformRandomNumberGenerator& __urng)
	{ this->__generate(__f, __t, __urng, this->_M_param); }

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
       * @brief Return true if two von Mises - Fisher distributions have the same
       *        parameters and the sequences that would be generated
       *        are equal.
       */
      friend bool
      operator==(const von_mises_fisher_distribution& __d1,
		 const von_mises_fisher_distribution& __d2)
      { return __d1._M_param == __d2._M_param; }

      /**
       * @brief Inserts a %von_mises_fisher_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %von_mises_fisher_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<std::size_t _Dim1, typename _RealType1,
	       typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>& __os,
		   const von_mises_fisher_distribution<_Dim1, _RealType1>& __x);

      /**
       * @brief Extracts a %von_mises_fisher_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x  A %von_mises_fisher_distribution random number generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<std::size_t _Dim1, typename _RealType1,
	       typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>& __is,
		   von_mises_fisher_distribution<_Dim1, _RealType1>& __x);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;
      uniform_on_sphere_distribution<_Dim - 1, _RealType> _M_uosd;
      beta_distribution<_RealType> _M_bd;
    };

  /**
   * @brief Return true if two von Mises - Fisher distributions are different.
   */
  template<std::size_t _Dim, typename _RealType>
    inline bool
    operator!=(const von_mises_fisher_distribution<_Dim, _RealType>& __d1,
	       const von_mises_fisher_distribution<_Dim, _RealType>& __d2)
    { return !(__d1 == __d2); }


  /**
   * @brief The partial specialization of the von Mises - Fisher distribution
   *        for random directions for dimension 2 (circle).
   *
   * This is the @c von_mises_distribution with a different interface.
   */
  template<typename _RealType>
    class von_mises_fisher_distribution<2, _RealType>
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      using result_type = std::array<_RealType, 2>;
      /** Parameter type. */
      struct param_type
      {
	friend class von_mises_fisher_distribution<2, _RealType>;

	explicit
	param_type(std::array<_RealType, 2> __mu/* = make_array(1, 0)*/,
		   _RealType __kappa = _RealType(1))
	: _M_mu(__mu),
	  _M_kappa(__kappa)
	{
	  _GLIBCXX_DEBUG_ASSERT(std::abs(__detail::__modulus(this->_M_mu) - 1)
				 < std::numeric_limits<_RealType>::epsilon());
	  _GLIBCXX_DEBUG_ASSERT(this->_M_kappa >= _RealType(0));

	  _M_theta0 = std::atan2(this->_M_mu[1], this->_M_mu[0]);
	}

	std::array<_RealType, 2>
	mu() const
	{ return _M_mu; }

	_RealType
	kappa() const
	{ return _M_kappa; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return (__p1._M_mu == __p2._M_mu
		  && __p1._M_kappa == __p2._M_kappa); }

      private:
	std::array<_RealType, 2> _M_mu;
	_RealType _M_kappa;
	_RealType _M_theta0;
      };

      /**
       * @brief Constructs a von Mises - Fisher distribution with parameters
       * @f$\mu@f$ and @f$\kappa@f$.
       */
      explicit
      von_mises_fisher_distribution(result_type __mu/* = make_array(1, 0)*/,
				    typename result_type::value_type __kappa
						= result_type::value_type(1))
      : _M_param(__mu, __kappa),
	_M_vmd(std::atan2(this->_M_param.mu()[1], this->_M_param.mu()[0]))
      { }

      explicit
      von_mises_fisher_distribution(const param_type& __p)
      : _M_param(__p),
	_M_vmd(std::atan2(this->_M_param.mu()[1], this->_M_param.mu()[0]))
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      { this->_M_vmd.reset(); }

      /**
       * @brief Returns the @f$ \bold{\mu} @f$ of the distribution.
       */
      result_type
      mu() const
      { return this->_M_param.mu(); }

      /**
       * @brief Returns the @f$ \kappa @f$ of the distribution.
       */
      typename result_type::value_type
      kappa() const
      { return this->_M_param.kappa(); }

      /**
       * @brief Returns the parameter set of the distribution.
       */
      param_type
      param() const
      { return this->_M_param; }

      /**
       * @brief Sets the parameter set of the distribution.
       * @param __param The new parameter set of the distribution.
       */
      void
      param(const param_type& __param)
      {
	this->_M_param = __param;
	typename von_mises_distribution<_RealType>::param_type
	  __vmd(std::atan2(this->_M_param.mu()[1], this->_M_param.mu()[0]));
	this->_M_vmd.param(__vmd);
      }

      /**
       * @brief Returns the greatest lower bound value of the distribution.
       */
      result_type
      min() const
      {
	constexpr auto __minv = -std::numeric_limits<_RealType>::max();
        result_type __arr;
        __arr.fill(__minv);
	return __arr;
      }

      /**
       * @brief Returns the least upper bound value of the distribution.
       */
      result_type
      max() const
      {
	constexpr auto __minv = -std::numeric_limits<_RealType>::max();
        result_type __arr;
        __arr.fill(__minv);
	return __arr;
      }

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
		   _UniformRandomNumberGenerator& __urng);

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
       * @brief Return true if two von Mises - Fisher distributions have the same
       *        parameters and the sequences that would be generated
       *        are equal.
       */
      friend bool
      operator==(const von_mises_fisher_distribution& __d1,
		 const von_mises_fisher_distribution& __d2)
      { return __d1._M_param == __d2._M_param; }

      /**
       * @brief Inserts a %von_mises_fisher_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %von_mises_fisher_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>& __os,
		   const von_mises_fisher_distribution<2, _RealType1>& __x);

      /**
       * @brief Extracts a %von_mises_fisher_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x  A %von_mises_fisher_distribution random number generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>& __is,
		   von_mises_fisher_distribution<2, _RealType1>& __x);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;
      von_mises_distribution<_RealType> _M_vmd;
    };


  /**
   * @brief The partial specialization of the von Mises - Fisher distribution
   *        for random directions for dimension 3 (circle).
   *
   * This is the @c von_mises_distribution with a different interface.
   */
  template<typename _RealType>
    class von_mises_fisher_distribution<3, _RealType>
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      using result_type = std::array<_RealType, 3>;
      /** Parameter type. */
      struct param_type
      {
	friend class von_mises_fisher_distribution<3, _RealType>;

	explicit
	param_type(std::array<_RealType, 3> __mu/* = make_array(1, 0, 0)*/,
		   _RealType __kappa = _RealType(1))
	: _M_mu(__mu),
	  _M_kappa(__kappa)
	{
	  _GLIBCXX_DEBUG_ASSERT(std::abs(__detail::__modulus(this->_M_mu) - 1)
				 < std::numeric_limits<_RealType>::epsilon());
	  _GLIBCXX_DEBUG_ASSERT(this->_M_kappa >= _RealType(0));

	  __detail::__make_basis(this->_M_mu, this->_M_lambda);

	  _M_c = 0;
	}

	std::array<_RealType, 3>
	mu() const
	{ return this->_M_mu; }

	_RealType
	kappa() const
	{ return this->_M_kappa; }

	friend bool
	operator==(const param_type& __p1, const param_type& __p2)
	{ return (__p1._M_mu == __p2._M_mu
		  && __p1._M_kappa == __p2._M_kappa); }

      private:
	std::array<_RealType, 3> _M_mu;
	std::array<std::array<_RealType, 3>, 2> _M_lambda;
	_RealType _M_kappa;
	_RealType _M_c;
      };

      /**
       * @brief Constructs a von Mises - Fisher distribution with parameters
       * @f$\mu@f$ and @f$\kappa@f$.
       */
      explicit
      von_mises_fisher_distribution(result_type __mu/* = make_array(1, 0)*/,
				    typename result_type::value_type __kappa
						= result_type::value_type(1))
      : _M_param(__mu, __kappa),
	_M_uosd()
      { }

      explicit
      von_mises_fisher_distribution(const param_type& __p)
      : _M_param(__p),
	_M_uosd()
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      { _M_uosd.reset(); }

      /**
       * @brief Returns the @f$ \bold{\mu} @f$ of the distribution.
       */
      result_type
      mu() const
      { return this->_M_param.mu(); }

      /**
       * @brief Returns the @f$ \kappa @f$ of the distribution.
       */
      typename result_type::value_type
      kappa() const
      { return this->_M_param.kappa(); }

      /**
       * @brief Returns the parameter set of the distribution.
       */
      param_type
      param() const
      { return this->_M_param; }

      /**
       * @brief Sets the parameter set of the distribution.
       * @param __param The new parameter set of the distribution.
       */
      void
      param(const param_type& __param)
      { this->_M_param = __param; }

      /**
       * @brief Returns the greatest lower bound value of the distribution.
       */
      result_type
      min() const
      {
	constexpr auto __minv = -std::numeric_limits<_RealType>::max();
        result_type __arr;
        __arr.fill(__minv);
	return __arr;
      }

      /**
       * @brief Returns the least upper bound value of the distribution.
       */
      result_type
      max() const
      {
	constexpr auto __minv = -std::numeric_limits<_RealType>::max();
        result_type __arr;
        __arr.fill(__minv);
	return __arr;
      }

      /**
       * @brief Generating functions.
       */
      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng)
	{ return this->operator()(__urng, this->_M_param); }

      template<typename _UniformRandomNumberGenerator>
	result_type
	operator()(_UniformRandomNumberGenerator& __urng,
		   const param_type& __p);

      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate(_ForwardIterator __f, _ForwardIterator __t,
		   _UniformRandomNumberGenerator& __urng)
	{ this->__generate(__f, __t, __urng, this->_M_param); }

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
       * @brief Return true if two von Mises - Fisher distributions have the same
       *        parameters and the sequences that would be generated
       *        are equal.
       */
      friend bool
      operator==(const von_mises_fisher_distribution& __d1,
		 const von_mises_fisher_distribution& __d2)
      { return __d1._M_param == __d2._M_param; }

      /**
       * @brief Inserts a %von_mises_fisher_distribution random number distribution
       * @p __x into the output stream @p __os.
       *
       * @param __os An output stream.
       * @param __x  A %von_mises_fisher_distribution random number distribution.
       *
       * @returns The output stream with the state of @p __x inserted or in
       * an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>& __os,
		   const von_mises_fisher_distribution<3, _RealType1>& __x);

      /**
       * @brief Extracts a %von_mises_fisher_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x  A %von_mises_fisher_distribution random number generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>& __is,
		   von_mises_fisher_distribution<3, _RealType1>& __x);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;
      uniform_on_sphere_distribution<2, _RealType> _M_uosd;
    };

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#include "von_mises_fisher_distribution.tcc"

#endif // _GLIBCXX_USE_C99_STDINT_TR1

#endif // C++11

#endif // _VON_MISES_FISHER_DISTRIBUTION_H
