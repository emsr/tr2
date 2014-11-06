#include <ext/random>

namespace __old //_GLIBCXX_VISIBILITY(default)
{
//_GLIBCXX_BEGIN_NAMESPACE_VERSION


  /**
   * @brief A distribution for random coordinates inside a unit sphere.
   *
   * The method used in the generation function is attributed by Donald Knuth
   * to G. W. Brown, Modern Mathematics for the Engineer (1956).
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
		   const __old::uniform_inside_sphere_distribution<_Dimen1,
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
		   __old::uniform_inside_sphere_distribution<_Dimen1,
								 _RealType1>&);

    private:
      template<typename _ForwardIterator,
	       typename _UniformRandomNumberGenerator>
	void
	__generate_impl(_ForwardIterator __f, _ForwardIterator __t,
			_UniformRandomNumberGenerator& __urng,
			const param_type& __p);

      param_type _M_param;
      __gnu_cxx::uniform_on_sphere_distribution<_Dimen, _RealType> _M_uosd;
    };

  /**
   * @brief Return true if two uniform on sphere distributions are different.
   */
  template<std::size_t _Dimen, typename _RealType>
    inline bool
    operator!=(const __old::uniform_inside_sphere_distribution<_Dimen,
	       _RealType>& __d1,
	       const __old::uniform_inside_sphere_distribution<_Dimen,
	       _RealType>& __d2)
    { return !(__d1 == __d2); }

//_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#include "uniform_inside_sphere_distribution_old.tcc"
