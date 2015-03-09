
// 

namespace std
{
namespace experimental
{
inline namespace fundamentals_v2
{

  template<typename _Tp>
    class propagate_const
    {
    public:

      using element_type = remove_reference_t<decltype(*declval<_Tp&>())>;

      // [propagate_const.ctor], constructors

      constexpr
      propagate_const() = default;

      propagate_const(const propagate_const& p) = delete;

      constexpr
      propagate_const(propagate_const&& p) = default;

      template<typename _Up>
	see below constexpr
	propagate_const(propagate_const<_Up>&& pu);

      template<typename _Up>
	see below constexpr
	propagate_const(_Up&& u);

      // [propagate_const.assignment], assignment

      propagate_const&
      operator=(const propagate_const& p) = delete;

      constexpr propagate_const&
      operator=(propagate_const&& p) = default;

      template<typename _Up>
	constexpr propagate_const&
	operator=(propagate_const<_Up>&& pu)
	{
          this->_M_t = std::move(pu._M_t);
          return *this;
	}

      template<typename _Up>
	constexpr propagate_const&
	operator=(_Up&& u)
	{
	  this->_M_t = std::forward<_Up>(__u);
	  return *this;
	}

      // [propagate_const.const_observers], const observers

      explicit constexpr
      operator bool() const
      { return (bool)_M_t; }

      constexpr const element_type*
      operator->() const
      { return this->get(); }

      constexpr operator
      const element_type*() const; //Not always defined

      constexpr const element_type&
      operator*() const
      { return *this->get(); }

      constexpr const element_type*
      get() const;

      // [propagate_const.non_const_observers], non-const observers

      constexpr element_type*
      operator->()
      { return this->get(); }

      constexpr operator
      element_type*(); //Not always defined

      constexpr element_type&
      operator*()
      { return *this->get(); }

      constexpr element_type*
      get();

      // [propagate_const.modifiers], modifiers
      constexpr void
      swap(propagate_const& __pt) noexcept(see below)
      { std::swap(this->_M_t, __pt._M_t); }

    private:
      _Tp _M_t; //exposition only
    };


  // [propagate_const.relational], relational operators
  template<typename _Tp>
    constexpr bool
    operator==(const propagate_const<_Tp>& __pt, nullptr_t)
    { return __pt._M_t != nullptr; }

  template<typename _Tp>
    constexpr bool
    operator==(nullptr_t, const propagate_const<_Tp>& __pu)
    { return nullptr != __pu._M_t; }


  template<typename _Tp>
    constexpr bool
    operator!=(const propagate_const<_Tp>& __pt, nullptr_t)
    { return !(__pt == nullptr_t); }

  template<typename _Tp>
    constexpr bool
    operator!=(nullptr_t, const propagate_const<_Tp>& __pu)
    { return !(nullptr_t == __pu); }


  template<typename _Tp, typename _Up>
    constexpr bool
    operator==(const propagate_const<_Tp>& __pt,
	       const propagate_const<_Up>& __pu)
    { return __pt == __up; }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator!=(const propagate_const<_Tp>& __pt,
	       const propagate_const<_Up>& __pu)
    { return !(__pt == __pu); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator<(const propagate_const<_Tp>& __pt,
	      const propagate_const<_Up>& __pu);

  template<typename _Tp, typename _Up>
    constexpr bool
    operator>(const propagate_const<_Tp>& __pt,
	      const propagate_const<_Up>& __pu)
    { return __pu < __pt; }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator<=(const propagate_const<_Tp>& __pt,
	       const propagate_const<_Up>& __pu)
    { return !(__pu < __pt); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator>=(const propagate_const<_Tp>& __pt,
	       const propagate_const<_Up>& __pu)
    { return !(__pt < __pu); }


  template<typename _Tp, typename _Up>
    constexpr bool
    operator==(const propagate_const<_Tp>& pt, const _Up& u)
    { return __pt == u; }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator!=(const propagate_const<_Tp>& __pt, const _Up& __u)
    { return !(__pt == __u); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator<(const propagate_const<_Tp>& pt, const _Up& u);

  template<typename _Tp, typename _Up>
    constexpr bool
    operator>(const propagate_const<_Tp>& __pt, const _Up& __u)
    { return __u < __pt; }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator<=(const propagate_const<_Tp>& __pt, const _Up& __u)
    { return !(__u < __pt); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator>=(const propagate_const<_Tp>& __pt, const _Up& __u)
    { return !(__pt < __u); }


  template<typename _Tp, typename _Up>
    constexpr bool
    operator==(const _Tp& __t, const propagate_const<_Up>& __pu)
    { return __t == __pu; }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator!=(const _Tp& __t, const propagate_const<_Up>& __pu)
    { return !(__t == __pu); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator<(const _Tp& t, const propagate_const<_Up>& pu);

  template<typename _Tp, typename _Up>
    constexpr bool
    operator>(const _Tp& __t, const propagate_const<_Up>& __pu)
    { return __pu < __t; }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator<=(const _Tp& __t, const propagate_const<_Up>& __pu)
    { return !(__pu < __t); }

  template<typename _Tp, typename _Up>
    constexpr bool
    operator>=(const _Tp& __t, const propagate_const<_Up>& __pu)
    { return !(__t < __pu); }

  // [propagate_const.algorithms], specialized algorithms

  template<typename _Tp>
    constexpr void
    swap(propagate_const<_Tp>& pt, propagate_const<_Tp>& pt2) noexcept(see below);

  // [propagate_const.underlying], underlying pointer access
  template<typename _Tp>
    constexpr const _Tp&
    get_underlying(const propagate_const<_Tp>& pt) noexcept;

  template<typename _Tp>
    constexpr _Tp&
    get_underlying(propagate_const<_Tp>& pt) noexcept;

} // namespace fundamentals_v2
} // namespace experimental

  // [propagate_const.hash], hash support

  template<typename _Tp>
    struct hash<experimental::fundamentals_v2::propagate_const<_Tp>>
    {

    };

  // [propagate_const.comparison_function_objects], comparison function objects

  template<typename _Tp>
    struct equal_to<experimental::fundamentals_v2::propagate_const<_Tp>>
    {

    };

  template<typename _Tp>
    struct not_equal_to<experimental::fundamentals_v2::propagate_const<_Tp>>
    {

    };

  template<typename _Tp>
    struct less<experimental::fundamentals_v2::propagate_const<_Tp>>
    {

    };

  template<typename _Tp>
    struct greater<experimental::fundamentals_v2::propagate_const<_Tp>>
    {

    };

  template<typename _Tp>
    struct less_equal<experimental::fundamentals_v2::propagate_const<_Tp>>
    {

    };

  template<typename _Tp>
    struct greater_equal<experimental::fundamentals_v2::propagate_const<_Tp>>
    {

    };

} // namespace std
