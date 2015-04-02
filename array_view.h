// <array_view.h> -*- C++ -*-

// Copyright (C) 2001-2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef _GLIBCXX_ARRAY_VIEW_H
#define _GLIBCXX_ARRAY_VIEW_H 1

#pragma GCC system_header

#if __cplusplus < 201500L
# include <bits/c++1z_warning.h>
#else

#include <stdexcept>
#include <type_traits>
#include <utility>
#include "coordinate.h"

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace experimental
{
inline namespace fundamentals_v2
{

  template<typename _Tp, int _Rank> class array_view;
  template<typename _Tp, int _Rank> class strided_array_view;

  namespace __detail
  {
  	// Note: This should be replaced with std::index_sequence if available.
  	template<std::size_t... I> struct index_seq {};

  	template<typename T>  struct append_1;
  	template<std::size_t... I> struct append_1<index_seq<I...>> { using type = index_seq<I..., sizeof...(I)>; };
  	template<typename T>  using  append_1_t = typename append_1<T>::type;

  	template<std::size_t N> struct make_seq;
  	template<std::size_t N> using  make_seq_t = typename make_seq<N>::type;

  	template<std::size_t N> struct make_seq    { using type = append_1_t<make_seq_t<N - 1>>; };
  	template<>         struct make_seq<0> { using type = index_seq<>; };

  	template<typename T, std::size_t... I>
  	constexpr bounds<sizeof...(I)> make_bounds_inner(index_seq<I...>) noexcept
  	{ return{ static_cast<std::ptrdiff_t>(std::extent<T, I>::value)... }; }

  	// Make bounds from an array type extents.
  	template<typename T>
  	constexpr auto make_bounds() noexcept -> decltype(make_bounds_inner<T>(make_seq_t<std::rank<T>::value>{}))
  	{ return make_bounds_inner<T>(make_seq_t<std::rank<T>::value>{}); }

  	// Make a stride vector from bounds, assuming contiguous memory.
  	template<int _Rank>
  	  constexpr index<_Rank>
	  make_stride(const bounds<_Rank>& bnd) noexcept
  	  {
		index<_Rank> stride;
		stride[_Rank - 1] = 1;
		for (int i = _Rank - 1; i-- > 0;)
		  stride[i] = stride[i + 1] * bnd[i + 1];
		return stride;
  	  }

  	template<typename T>
  	  constexpr T*
	  to_pointer(T& t) noexcept
  	  { return &t; }

  	template<typename T, int N>
  	  constexpr std::remove_all_extents_t<T>*
	  to_pointer(T(&t)[N]) noexcept
  	  { return to_pointer(t[0]); }

  	template<typename T, typename _Tp>
  	  struct is_viewable
	  : std::integral_constant<bool,
							   std::is_convertible<decltype(std::declval<T>().size()), std::ptrdiff_t>::value
												&& std::is_convertible<decltype(std::declval<T>().data()), _Tp*>::value
												&& std::is_same<std::remove_cv_t<std::remove_pointer_t<decltype(std::declval<T>().data())>>,
												   std::remove_cv_t<_Tp>>::value>
	  {};

  	template<typename T>
  	  struct is_array_view_oracle
	  : false_type
  	  {};
	
  	template<typename T, int N>
  	  struct is_array_view_oracle<array_view<T, N>>
	  : true_type
  	  {};

  	template<typename T>
  	  struct is_array_view
	  : is_array_view_oracle<std::decay_t<T>>
  	  {};

  	template<template<typename, int> typename ViewType, typename _Tp, int _Rank>
  	  struct slice_return_type
	  { using type = ViewType<_Tp, _Rank - 1>; };

  	template<template<typename, int> typename ViewType, typename _Tp>
  	  struct slice_return_type<ViewType, _Tp, 1>
	  { using type = void; };

  	template<template<typename, int> typename ViewType, typename _Tp, int _Rank>
  	  using slice_return_type_t = typename slice_return_type<ViewType, _Tp, _Rank>::type;

  	template<typename _Tp, int _Rank>
  	  class any_array_view_base
  	  {
  	  public:
		static const int rank = _Rank;
		using index_type  = index<rank>;
		using bounds_type = bounds<rank>;
		using size_type   = std::size_t;
		using value_type  = _Tp;
		using pointer     = _Tp*;
		using reference   = _Tp&;

		constexpr bounds_type
		bounds() const noexcept
		{ return bnd; }

		constexpr size_type
		size() const noexcept
		{ return bnd.size(); }

		constexpr index_type
		stride() const noexcept
		{ return srd; }

		// Preconditions: (*this).bounds().contains(idx)
		constexpr reference
		operator[](const index_type& idx) const
		{
		  assert(bnd.contains(idx));
		  auto ptr = data_ptr;
		  for (int i = 0; i < rank; i++)
			ptr += idx[i] * srd[i];
		  return *ptr;
		}

		// Preconditions: for any index idx, if section_bounds.contains(idx),
		// bounds().contains(origin + idx) must be true
		constexpr strided_array_view<value_type, rank>
		section(const index_type& origin, const bounds_type& section_bnd) const
		{
		  assert(bnd.contains(origin));
		  assert(check_section_correct(origin, section_bnd));
		  return{ section_bnd, srd, &operator[](origin) };
		}

		// Preconditions: for any index idx, if section_bounds.contains(idx),
		// bounds().contains(origin + idx) must be true
		constexpr strided_array_view<value_type, rank>
		section(const index_type& origin) const
		{
		  assert(bnd.contains(origin));
		  bounds_type section_bnd = bnd - origin;
		  return section(origin, section_bnd);
		}

	  protected:
		constexpr
		any_array_view_base(bounds_type bnd, index_type stride, pointer data) noexcept
		: data_ptr{std::move(data)},
		  bnd{std::move(bnd)},
		  srd{std::move(stride)}
		{ }

		constexpr bool
		check_section_correct(const index_type& origin, const bounds_type& section_bnd) const noexcept
		{
		  for (int i = 0; i < rank; ++i)
			if (origin[i] > bnd[i] - section_bnd[i])
			  return false;
		  return true;
		}

		pointer data_ptr;
		bounds_type bnd;
		index_type srd;
		// Note: for non-strided array view, stride can be computed on-the-fly
		// thus saving couple of bytes. It should be measured whether it's
		// beneficial.
  	  };

  } // namespace __detail

  template<typename _Tp, int _Rank = 1>
	class array_view
	: public __detail::any_array_view_base<_Tp, _Rank>
	{
	  using Base = __detail::any_array_view_base<_Tp, _Rank>;
	  template<typename AnyValueType, int AnyRank> friend class array_view;
	  template<typename AnyValueType, int AnyRank> friend class strided_array_view;

	public:
	  using Base::rank;
	  using index_type  = typename Base::index_type;
	  using bounds_type = typename Base::bounds_type;
	  using size_type   = typename Base::size_type;
	  using value_type  = typename Base::value_type;
	  using pointer     = typename Base::pointer;
	  using reference   = typename Base::reference;

	  constexpr
	  array_view() noexcept
	  : Base{{}, {}, nullptr}
	  { }

	  template<typename Viewable,
		typename = std::enable_if_t<rank == 1
								 && __detail::is_viewable<Viewable, value_type>::value
								 && !__detail::is_array_view<Viewable>::value>>
		constexpr array_view(Viewable&& cont)
		: Base{static_cast<typename bounds_type::value_type>(cont.size()), 1, cont.data()}
		{ }

	  template<typename ViewValueType, int ViewRank,
			   typename = std::enable_if_t<rank == 1
										&& std::is_convertible<std::add_pointer_t<ViewValueType>, pointer>::value
										&& std::is_same<std::remove_cv_t<ViewValueType>, std::remove_cv_t<value_type>>::value>>
		constexpr array_view(const array_view<ViewValueType, ViewRank>& rhs) noexcept
		: Base{static_cast<typename bounds_type::value_type>(rhs.size()), 1, rhs.data()}
		{ }

	  // Preconditions: product of the ArrayType extents must be <= ptrdiff_t max.
	  template<typename ArrayType,
			   typename = std::enable_if_t<std::is_convertible<std::add_pointer_t<std::remove_all_extents_t<ArrayType>>, pointer>::value
										&& std::is_same<std::remove_cv_t<std::remove_all_extents_t<ArrayType>>, std::remove_cv_t<value_type>>::value
										&& std::rank<ArrayType>::value == rank>>
		constexpr array_view(ArrayType& data) noexcept
		: Base{__detail::make_bounds<ArrayType>(),
			   __detail::make_stride(__detail::make_bounds<ArrayType>()),
  		       __detail::to_pointer(data)}
		{ }

	  template<typename ViewValueType,
			   typename = std::enable_if_t<std::is_convertible<std::add_pointer_t<ViewValueType>,
															   pointer>::value
										&& std::is_same<std::remove_cv_t<ViewValueType>,
														std::remove_cv_t<value_type>>::value>>
		constexpr
		array_view(const array_view<ViewValueType, rank>& rhs) noexcept
		: Base{rhs.bnd, rhs.srd, rhs.data_ptr}
		{ }

	  // Preconditions: bounds.size() <= cont.size()
	  template<typename Viewable,
			   typename = std::enable_if_t<__detail::is_viewable<Viewable, value_type>::value>>
		constexpr
		array_view(bounds_type bounds, Viewable&& cont)
		: Base{bounds, __detail::make_stride(bounds), cont.data()}
		{ assert(Base::bnd.size() <= cont.size()); }

	  constexpr
	  array_view(bounds_type bounds, pointer data)
	  : Base{bounds, __detail::make_stride(bounds), data}
	  { }

	  template<typename ViewValueType,
			   typename = std::enable_if_t<std::is_convertible<std::add_pointer_t<ViewValueType>,
															   pointer>::value
										&& std::is_same<std::remove_cv_t<ViewValueType>,
														std::remove_cv_t<value_type>>::value>>
		constexpr array_view&
		operator=(const array_view<ViewValueType, rank>& rhs) noexcept
		{
		  Base::bnd = rhs.bnd;
		  Base::srd = rhs.srd;
		  Base::data_ptr = rhs.data_ptr;
		  return *this;
		}

	  using Base::operator[];

	  // Returns a slice of the view.
	  // Preconditions: slice < (*this).bounds()[0]
	  template<int _dummy_rank = rank>
		constexpr typename __detail::slice_return_type<std::experimental::fundamentals_v2::array_view, value_type, _Rank>::type
		operator[](typename std::enable_if<_dummy_rank != 1, typename index_type::value_type>::type slice) const
		{
		  static_assert(_dummy_rank == rank, "_dummy_rank must have the default value!");
		  assert(slice < Base::bnd[0]);

		  index_type idx;
		  idx[0] = slice;

		  std::experimental::fundamentals_v2::bounds<rank - 1> bound;
		  for (int i = 1; i < rank; ++i)
			bound[i - 1] = Base::bnd[i];

		  return{bound, &operator[](idx)};
		}

	  constexpr pointer
	  data() const noexcept
	  { return Base::data_ptr; }
	};

  template<typename _Tp, int _Rank = 1>
	class strided_array_view
	: public __detail::any_array_view_base<_Tp, _Rank>
	{
	  using Base = __detail::any_array_view_base<_Tp, _Rank>;
	  friend strided_array_view<const _Tp, _Rank>;

	public:
	  using Base::rank;
	  using index_type  = typename Base::index_type;
	  using bounds_type = typename Base::bounds_type;
	  using size_type   = typename Base::size_type;
	  using value_type  = typename Base::value_type;
	  using pointer     = typename Base::pointer;
	  using reference   = typename Base::reference;

	  constexpr strided_array_view() noexcept
	  : Base{{}, {}, nullptr}
	  { }

	  template<typename ViewValueType,
			   typename = std::enable_if_t<std::is_convertible<std::add_pointer_t<ViewValueType>, pointer>::value
										&& std::is_same<std::remove_cv_t<ViewValueType>, std::remove_cv_t<value_type>>::value>>
		constexpr strided_array_view(const array_view<ViewValueType, rank>& rhs) noexcept
		: Base{rhs.bnd, rhs.srd, rhs.data_ptr}
		{ }

  	  template<typename ViewValueType,
			   typename = std::enable_if_t<std::is_convertible<std::add_pointer_t<ViewValueType>, pointer>::value
										&& std::is_same<std::remove_cv_t<ViewValueType>, std::remove_cv_t<value_type>>::value>>
		constexpr
		strided_array_view(const strided_array_view<ViewValueType, rank>& rhs) noexcept
		: Base{rhs.bnd, rhs.srd, rhs.data_ptr}
		{ }

	  // Preconditions:
  	  //   - for any index idx, if bounds().contains(idx),
 	  //     for i = [0,rank), idx[i] * stride[i] must be representable as ptrdiff_t
  	  //   - for any index idx, if bounds().contains(idx),
  	  //     (*this)[idx] must refer to a valid memory location
	  constexpr
	  strided_array_view(bounds_type bounds, index_type stride, pointer data) noexcept
	  : Base{std::move(bounds), std::move(stride), data}
	  { }

	  template<typename ViewValueType,
			   typename = std::enable_if_t<std::is_convertible<std::add_pointer_t<ViewValueType>,
															   pointer>::value
										&& std::is_same<std::remove_cv_t<ViewValueType>,
														std::remove_cv_t<value_type>>::value>>
		constexpr strided_array_view&
		operator=(const strided_array_view<ViewValueType, rank>& rhs) noexcept
		{
		  Base::bnd = rhs.bnd;
		  Base::srd = rhs.srd;
		  Base::data_ptr = rhs.data_ptr;
		  return *this;
		}

  	  using Base::operator[];

	  // Returns a slice of the view.
	  // Preconditions: slice < (*this).bounds()[0]
	  template<int _dummy_rank = rank>
		constexpr __detail::slice_return_type_t<std::experimental::fundamentals_v2::strided_array_view, value_type, _Rank>
		operator[](typename std::enable_if<_dummy_rank != 1,
										   typename index_type::value_type>::type slice) const noexcept
		{
		  static_assert(_dummy_rank == rank, "_dummy_rank must have the default value!");
		  assert(slice < Base::bnd[0]);

		  index_type idx;
		  idx[0] = slice;

		  std::experimental::fundamentals_v2::bounds<rank - 1> bound;
		  std::experimental::fundamentals_v2::index<rank - 1> stride;
		  for (int i = 1; i < rank; ++i)
		  {
			bound[i - 1] = Base::bnd[i];
			stride[i - 1] = Base::srd[i];
		  }

		  return{bound, stride, &operator[](idx)};
		}
	};

} // inline namespace fundamentals_v2
} // namespace experimenta
} // namespace std

#endif // _GLIBCXX_ARRAY_VIEW_H
