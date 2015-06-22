// PR c++/63889 - Ice with redundant static in class scope constexpr variable template.

// $HOME/bin/bin/g++ -std=c++14 -c ice_class_vt.C 

#include <experimental/type_traits>

template<typename _Tp>
  class observer_ptr
  {
  public:
    using element_type    = _Tp;
    using pointer         = std::add_pointer_t<_Tp>;
    using reference       = std::add_lvalue_reference_t<_Tp>;

    /* Making this static iced - that shouldn't happen.
       "A variable template at class scope is a static data member template."
       So this is redundant but shouldn't blow.  */
    template<typename _Tp2>
      static constexpr bool
      _S_is_compat = std::experimental::is_convertible_v<std::add_pointer_t<_Tp2>, pointer>;

    template<typename _Tp2,
	     typename _Ptr2 = std::enable_if_t<_S_is_compat<_Tp2>, std::add_pointer_t<_Tp2>>>
      explicit constexpr
      observer_ptr(_Ptr2 * __ptr) noexcept
      : _M_ptr{__ptr}
      { }
  private:
    pointer _M_ptr;
  };
