
#include <type_traits>
#include <tuple>
#include <ratio>
#include <system_error>
#include <bits/uses_allocator.h>
#include <functional>
#include <chrono>

namespace std _GLIBCXX_VISIBILITY(default)
{

#if __cplusplus > 201300L

  //  1. In 20.10.2 [meta.type.synop], at the end of the
  "// 20.10.4.1, primary type categories:" chunk, add:

  template<typename _Tp>
    constexpr bool
    is_void_v = is_void<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_null_pointer_v = is_null_pointer<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_integral_v = is_integral<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_floating_point_v = is_floating_point<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_array_v = is_array<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_pointer_v = is_pointer<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_lvalue_reference_v = is_lvalue_reference<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_rvalue_reference_v = is_rvalue_reference<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_member_object_pointer_v = is_member_object_pointer<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_member_function_pointer_v = is_member_function_pointer<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_enum_v = is_enum<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_union_v = is_union<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_class_v = is_class<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_function_v = is_function<_Tp>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  2. In 20.10.2 [meta.type.synop], at the end of the
  //  "// 20.10.4.2, composite type categories:" chunk, add:

  template<typename _Tp>
    constexpr bool
    is_reference_v = is_reference<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_arithmetic_v = is_arithmetic<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_fundamental_v = is_fundamental<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_object_v = is_object<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_scalar_v = is_scalar<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_compound_v = is_compound<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_member_pointer_v = is_member_pointer<_Tp>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  3. In 20.10.2 [meta.type.synop], at the end of the
  //  "// 20.10.4.3, type properties:" chunk, add:

  template<typename _Tp>
    constexpr bool
    is_const_v = is_const<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_volatile_v = is_volatile<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_trivial_v = is_trivial<_Tp>::value;

  //template<typename _Tp>
  //  constexpr bool
  //  is_trivially_copyable_v = is_trivially_copyable<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_standard_layout_v = is_standard_layout<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_pod_v = is_pod<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_literal_type_v = is_literal_type<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_empty_v = is_empty<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_polymorphic_v = is_polymorphic<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_abstract_v = is_abstract<_Tp>::value;

  //template<typename _Tp>
  //  constexpr bool
  //  is_final_v = is_final<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_signed_v = is_signed<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_unsigned_v = is_unsigned<_Tp>::value;

  template<typename _Tp, class... _Args>
    constexpr bool
    is_constructible_v = is_constructible<_Tp, _Args...>::value;

  template<typename _Tp>
    constexpr bool
    is_default_constructible_v = is_default_constructible<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_copy_constructible_v = is_copy_constructible<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_move_constructible_v = is_move_constructible<_Tp>::value;

  template<typename _Tp, typename _Up>
    constexpr bool
    is_assignable_v = is_assignable<_Tp, _Up>::value;

  template<typename _Tp>
    constexpr bool
    is_copy_assignable_v = is_copy_assignable<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_move_assignable_v = is_move_assignable<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_destructible_v = is_destructible<_Tp>::value;

  //template<typename _Tp, typename... _Args>
  //  constexpr bool
  //  is_trivially_constructible_v
  //    = is_trivially_constructible<_Tp, _Args...>::value;

  //template<typename _Tp>
  //  constexpr bool
  //  is_trivially_default_constructible_v
  //    = is_trivially_default_constructible<_Tp>::value;

  //template<typename _Tp>
  //  constexpr bool
  //  is_trivially_copy_constructible_v
  //    = is_trivially_copy_constructible<_Tp>::value;

  //template<typename _Tp>
  //  constexpr bool
  //  is_trivially_move_constructible_v
  //    = is_trivially_move_constructible<_Tp>::value;

  //template<typename _Tp, typename _Up>
  //  constexpr bool
  //  is_trivially_assignable_v
  //    = is_trivially_assignable<_Tp, _Up>::value;

  //template<typename _Tp>
  //  constexpr bool
  //  is_trivially_copy_assignable_v
  //    = is_trivially_copy_assignable<_Tp>::value;

  //template<typename _Tp>
  //  constexpr bool
  //  is_trivially_move_assignable_v
  //    = is_trivially_move_assignable<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_trivially_destructible_v
      = is_trivially_destructible<_Tp>::value;

  template<typename _Tp, class... _Args>
    constexpr bool
    is_nothrow_constructible_v
      = is_nothrow_constructible<_Tp, _Args...>::value;

  template<typename _Tp>
    constexpr bool
    is_nothrow_default_constructible_v
      = is_nothrow_default_constructible<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_nothrow_copy_constructible_v
      = is_nothrow_copy_constructible<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_nothrow_move_constructible_v
      = is_nothrow_move_constructible<_Tp>::value;

  template<typename _Tp, typename _Up>
    constexpr bool
    is_nothrow_assignable_v = is_nothrow_assignable<_Tp, _Up>::value;

  template<typename _Tp>
    constexpr bool
    is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_nothrow_move_assignable_v = is_nothrow_move_assignable<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_nothrow_destructible_v = is_nothrow_destructible<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    has_virtual_destructor_v = has_virtual_destructor<_Tp>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  4. In 20.10.2 [meta.type.synop], at the end of the
  //  "// 20.10.5, type property queries:" chunk, add:

  template<typename _Tp>
    constexpr std::size_t
    alignment_of_v = alignment_of<_Tp>::value;

  template<typename _Tp>
    constexpr std::size_t
    rank_v = rank<_Tp>::value;

  template<typename _Tp, unsigned _I = 0>
    constexpr std::size_t
    extent_v = extent<_Tp, _I>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  5. In 20.10.2 [meta.type.synop], at the end of the
  //  "// 20.10.6, type relations:" chunk, add:

  template<typename _Tp, typename _Up>
    constexpr bool
    is_same_v = is_same<_Tp, _Up>::value;

  template<typename Base, typename _Derived>
    constexpr bool
    is_base_of_v = is_base_of<Base, _Derived>::value;

  template<typename From, typename _Tpo>
    constexpr bool
    is_convertible_v = is_convertible<From, _Tpo>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  6. In 20.4.1 [tuple.general]/2, at the end of the
  "// 20.4.2.5, tuple helper classes:" chunk, add:

  template<typename _Tp>
    constexpr std::size_t
    tuple_size_v = tuple_size<_Tp>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  7. In 20.11.2 [ratio.syn], at the end of the
  //  "// 20.11.5, ratio comparison" chunk, add:

  template<typename _R1, typename _R2>
    constexpr bool
    ratio_equal_v = ratio_equal<_R1, _R2>::value;

  template<typename _R1, typename _R2>
    constexpr bool
    ratio_not_equal_v = ratio_not_equal<_R1, _R2>::value;

  template<typename _R1, typename _R2>
    constexpr bool
    ratio_less_v = ratio_less<_R1, _R2>::value;

  template<typename _R1, typename _R2>
    constexpr bool
    ratio_less_equal_v = ratio_less_equal<_R1, _R2>::value;

  template<typename _R1, typename _R2>
    constexpr bool
    ratio_greater_v = ratio_greater<_R1, _R2>::value;

  template<typename _R1, typename _R2>
    constexpr bool
    ratio_greater_equal_v = ratio_greater_equal<_R1, _R2>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  8. In 19.5 [syserr]/2, after the definition of is_error_condition_enum, add:

  template<typename _Tp>
    constexpr bool
    is_error_code_enum_v = is_error_code_enum<_Tp>::value;

  template<typename _Tp>
    constexpr bool
    is_error_condition_enum_v = is_error_condition_enum<_Tp>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  In 20.7.2 [memory.syn]/1, at the end of the
  //  "// 20.7.7, uses_allocator" chunk, add:

  template<typename _Tp, typename _Alloc>
    constexpr bool
    uses_allocator_v = uses_allocator<_Tp, _Alloc>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

  //  In 20.9 [function.objects]/2, after the declaration of is_placeholder, add:

  template<typename _Tp>
    constexpr bool
    is_bind_expression_v = is_bind_expression<_Tp>::value;

  template<typename _Tp>
    constexpr int
    is_placeholder_v = is_placeholder<_Tp>::value;

#endif // __cplusplus > 201300L


#if __cplusplus > 201300L

//  11. In 20.12.2 [time.syn], at the end of the
//  "// 20.12.4, customization traits" chunk, add:

template<typename _Rep>
  constexpr bool
  treat_as_floating_point_v = treat_as_floating_point<_Rep>::value;

#endif // __cplusplus > 201300L

}
