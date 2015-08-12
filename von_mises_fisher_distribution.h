

  /**
   * @brief A von Mises - Fisher distribution for random directions.
   *
   * The formula for the von Mises - Fisher probability density function is
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
   *
   * <table border=1 cellpadding=10 cellspacing=0>
   * <caption align=top>Distribution Statistics</caption>
   * <tr><td>Mean</td><td>@f$ \bold{\mu} @f$</td></tr>
   * <tr><td>Variance</td><td>@f$  @f$</td></tr>
   * <tr><td>Range</td><td>@f$[-\pi, \pi]@f$</td></tr>
   * </table>
   */
  template<typename _RealType = double, std::size_t _Dim = 3>
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
	friend class von_mises_fisher_distribution<_RealType>;

	explicit
	param_type(std::array<_RealType, _Dim> __mu = make_array(1, 0...),
		   _RealType __kappa = _RealType(1))
	: _M_mu(__mu), _M_kappa(__kappa)
	{
	  const _RealType __pi = __gnu_cxx::__math_constants<_RealType>::__pi;
	  _GLIBCXX_DEBUG_ASSERT(_M_mu >= -__pi && _M_mu <= __pi);
	  _GLIBCXX_DEBUG_ASSERT(_M_kappa >= _RealType(0));

	  auto __tau = std::sqrt(_RealType(4) * _M_kappa * _M_kappa
				 + _RealType(1)) + _RealType(1);
	  auto __rho = ((__tau - std::sqrt(_RealType(2) * __tau))
			/ (_RealType(2) * _M_kappa));
	  _M_r = (_RealType(1) + __rho * __rho) / (_RealType(2) * __rho);
	}

	std::array<_RealType, _Dim>
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
	std::array<_RealType, _Dim> _M_mu;
	_RealType _M_kappa;
	_RealType _M_r;
      };

      /**
       * @brief Constructs a von Mises - Fisher distribution with parameters
       * @f$\mu@f$ and @f$\kappa@f$.
       */
      explicit
      von_mises_fisher_distribution(result_type __mu = make_array(),
				    result_type __kappa = result_type::value_type(1))
	: _M_param(__mu, __kappa)
      { }

      explicit
      von_mises_fisher_distribution(const param_type& __p)
      : _M_param(__p)
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      { }

      /**
       * @brief Returns the @f$ \mu @f$ of the distribution.
       */
      result_type
      mu() const
      { return _M_param.mu(); }

      /**
       * @brief Returns the @f$ \kappa @f$ of the distribution.
       */
      result_type::value_type
      kappa() const
      { return _M_param.kappa(); }

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
      {
	return make_array();
      }

      /**
       * @brief Returns the least upper bound value of the distribution.
       */
      result_type
      max() const
      {
	return make_array();
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
	{ this->__generate(__f, __t, __urng, _M_param); }

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
      template<typename _RealType1, std::size_t _Dim1,
	       typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits>&
	operator<<(std::basic_ostream<_CharT, _Traits>& __os,
		   const __gnu_cxx::von_mises_fisher_distribution<_RealType1, _Dim1>& __x);

      /**
       * @brief Extracts a %von_mises_fisher_distribution random number distribution
       * @p __x from the input stream @p __is.
       *
       * @param __is An input stream.
       * @param __x  A %von_mises_fisher_distribution random number generator engine.
       *
       * @returns The input stream with @p __x extracted or in an error state.
       */
      template<typename _RealType1, std::size_t _Dim1,
	       typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits>&
	operator>>(std::basic_istream<_CharT, _Traits>& __is,
		   __gnu_cxx::von_mises_fisher_distribution<_RealType1, _Dim1>& __x);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;
    };

  /**
   * @brief Return true if two von Mises - Fisher distributions are different.
   */
  template<typename _RealType, std::size_t _Dim>
    inline bool
    operator!=(const __gnu_cxx::von_mises_fisher_distribution<_RealType, _Dim>& __d1,
	       const __gnu_cxx::von_mises_fisher_distribution<_RealType, _Dim>& __d2)
    { return !(__d1 == __d2); }


  /**
   * @brief The partial specialization of the von Mises - Fisher distribution
   *        for random directions for dimension 2 (circle).
   *
   * This is the @c von_mises_distribution with a different interface.
   */
  template<typename _RealType = double>
    class von_mises_fisher_distribution<_RealType, 2>
    {
      static_assert(std::is_floating_point<_RealType>::value,
		    "template argument not a floating point type");

    public:
      /** The type of the range of the distribution. */
      using result_type = std::array<_RealType, 2>;
      /** Parameter type. */
      struct param_type
      {
	friend class von_mises_fisher_distribution<_RealType>;

	explicit
	param_type(std::array<_RealType, 2> __mu = make_array(1, 0),
		   _RealType __kappa = _RealType(1))
	: _M_mu(__mu), _M_kappa(__kappa)
	{
	  const _RealType __pi = __gnu_cxx::__math_constants<_RealType>::__pi;
	  _GLIBCXX_DEBUG_ASSERT(_M_mu >= -__pi && _M_mu <= __pi);
	  _GLIBCXX_DEBUG_ASSERT(_M_kappa >= _RealType(0));

	  auto __tau = std::sqrt(_RealType(4) * _M_kappa * _M_kappa
				 + _RealType(1)) + _RealType(1);
	  auto __rho = ((__tau - std::sqrt(_RealType(2) * __tau))
			/ (_RealType(2) * _M_kappa));
	  _M_r = (_RealType(1) + __rho * __rho) / (_RealType(2) * __rho);
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
	_RealType _M_r;
      };

      /**
       * @brief Constructs a von Mises - Fisher distribution with parameters
       * @f$\mu@f$ and @f$\kappa@f$.
       */
      explicit
      von_mises_fisher_distribution(result_type __mu = make_array(),
				    result_type __kappa = result_type::value_type(1))
	: _M_param(__mu, __kappa)
      { }

      explicit
      von_mises_fisher_distribution(const param_type& __p)
      : _M_param(__p)
      { }

      /**
       * @brief Resets the distribution state.
       */
      void
      reset()
      { }

      /**
       * @brief Returns the @f$ \mu @f$ of the distribution.
       */
      result_type
      mu() const
      { return _M_param.mu(); }

      /**
       * @brief Returns the @f$ \kappa @f$ of the distribution.
       */
      result_type::value_type
      kappa() const
      { return _M_param.kappa(); }

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
      {
	return make_array();
      }

      /**
       * @brief Returns the least upper bound value of the distribution.
       */
      result_type
      max() const
      {
	return make_array();
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
	{ this->__generate(__f, __t, __urng, _M_param); }

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
		   const __gnu_cxx::von_mises_fisher_distribution<_RealType1, 2>& __x);

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
		   __gnu_cxx::von_mises_fisher_distribution<_RealType1, 2>& __x);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;
    };

#include "von_mises_fisher_distribution.tcc"
