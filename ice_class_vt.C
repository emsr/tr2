
#include <type_traits>  // add_pointer, enable_if, ...

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
    template<typename _Ptr>
      static constexpr bool
      _S_is_compat = std::is_convertible<std::add_pointer_t<_Ptr>, pointer>();

    template<typename _Tp2,
	     typename = std::enable_if_t<_S_is_compat<_Tp2>>>
      explicit
      observer_ptr(_Tp2 * __ptr) noexcept
      : _M_ptr{__ptr}
      { }
 };

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63889
