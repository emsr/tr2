#pragma once

#ifndef _GLIBCXX_COORDINATE_H
#define _GLIBCXX_COORDINATE_H 1

#pragma GCC system_header

#if __cplusplus < 201500L
# include <bits/c++1z_warning.h>
#else

#include <assert.h>
#include <iterator>
#include <experimental/type_traits>

#define constexpr

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace experimental
{
inline namespace fundamentals_v2
{

namespace __detail
{

  template<typename _ConcTp, typename _Tp, int _Rank>
	class coordinate_facade
	{
	  static_assert(std::is_fundamental_v<_Tp>, "ValueType must be fundamental!");
	  static_assert(_Rank > 0, "Rank must be greater than 0!");

	public:
	  using reference       = _Tp&;
	  using const_reference = const _Tp&;
	  using size_type       = std::size_t;
	  using value_type      = _Tp;
	  static const int rank = _Rank;

	  constexpr
	  coordinate_facade() noexcept
	  {
		static_assert(std::is_base_of_v<coordinate_facade, _ConcTp>,
					  "ConcreteType must be derived from coordinate_facade.");
		for (int i = 0; i < rank; ++i)
		  elems[i] = {};
	  }

	  constexpr
	  coordinate_facade(value_type e0) noexcept
	  {
		static_assert(std::is_base_of_v<coordinate_facade, _ConcTp>,
					  "ConcreteType must be derived from coordinate_facade.");
		static_assert(rank == 1, "This constructor can only be used with rank == 1.");
		elems[0] = e0;
	  }

		// Preconditions: il.size() == rank
	  constexpr
	  coordinate_facade(std::initializer_list<value_type> il)
	  {
		static_assert(std::is_base_of_v<coordinate_facade, _ConcTp>,
					  "ConcreteType must be derived from coordinate_facade.");
		assert(il.size() == rank);
		for (int i = 0; i < rank; ++i)
		  elems[i] = begin(il)[i];
	  }

	protected:
	  coordinate_facade&
	  operator=(const coordinate_facade& rhs) = default;

		// Preconditions: component_idx < rank
	  constexpr reference
	  operator[](size_type component_idx)
	  { return elems[component_idx]; }

		// Preconditions: component_idx < rank
	  constexpr const_reference
	  operator[](size_type component_idx) const
	  { return elems[component_idx]; }

	  constexpr bool
	  operator==(const _ConcTp& rhs) const noexcept
	  {
		for (int i = 0; i < rank; ++i)
		  if (elems[i] != rhs.elems[i])
			return false;
		return true;
	  }

	  constexpr bool
	  operator!=(const _ConcTp& rhs) const noexcept
	  { return !(to_concrete() == rhs); }

	  constexpr _ConcTp
	  operator+() const noexcept
	  { return to_concrete(); }

	  constexpr _ConcTp
	  operator-() const
	  {
		_ConcTp ret = to_concrete();
		for (int i = 0; i < rank; ++i)
		  ret.elems[i] = -ret.elems[i];
		return ret;
	  }

	  constexpr _ConcTp
	  operator+(const _ConcTp& rhs) const
	  {
		_ConcTp ret = to_concrete();
		ret += rhs;
		return ret;
	  }

	  constexpr _ConcTp
	  operator-(const _ConcTp& rhs) const
	  {
		_ConcTp ret = to_concrete();
		ret -= rhs;
		return ret;
	  }

	  constexpr _ConcTp&
	  operator+=(const _ConcTp& rhs)
	  {
		for (int i = 0; i < rank; ++i)
		  elems[i] += rhs.elems[i];
		return to_concrete();
	  }

	  constexpr _ConcTp&
	  operator-=(const _ConcTp& rhs)
	  {
		for (int i = 0; i < rank; ++i)
		  elems[i] -= rhs.elems[i];
		return to_concrete();
	  }

	  constexpr _ConcTp&
	  operator++()
	  {
		static_assert(rank == 1, "This operator can only be used with rank == 1.");
		++elems[0];
		return to_concrete();
	  }

	  constexpr _ConcTp
	  operator++(int)
	  {
		static_assert(rank == 1, "This operator can only be used with rank == 1.");
		_ConcTp ret = to_concrete();
		++(*this);
		return ret;
	  }

	  constexpr _ConcTp&
	  operator--()
	  {
		static_assert(rank == 1, "This operator can only be used with rank == 1.");
		--elems[0];
		return to_concrete();
	  }

	  constexpr _ConcTp
	  operator--(int)
	  {
		static_assert(rank == 1, "This operator can only be used with rank == 1.");
		_ConcTp ret = to_concrete();
		--(*this);
		return ret;
	  }

	  constexpr _ConcTp
	  operator*(value_type v) const
	  {
		_ConcTp ret = to_concrete();
		ret *= v;
		return ret;
	  }

	  constexpr _ConcTp
	  operator/(value_type v) const
	  {
		_ConcTp ret = to_concrete();
		ret /= v;
		return ret;
	  }

	  friend constexpr _ConcTp
	  operator*(value_type v, const _ConcTp& rhs)
	  { return rhs * v; }

	  constexpr _ConcTp& operator*=(value_type v)
	  {
		for (int i = 0; i < rank; ++i)
		  elems[i] *= v;
		return to_concrete();
	  }

	  constexpr _ConcTp& operator/=(value_type v)
	  {
		for (int i = 0; i < rank; ++i)
		  elems[i] /= v;
		return to_concrete();
	  }

	  value_type elems[rank];

	private:
	  constexpr const _ConcTp&
	  to_concrete() const noexcept
	  { return static_cast<const _ConcTp&>(*this); }

	  constexpr _ConcTp&
	  to_concrete() noexcept
	  { return static_cast<_ConcTp&>(*this); }
	};

	template<typename T>
	class arrow_proxy
	{
	public:
	  explicit
	  arrow_proxy(T t)
	  : val{t}
	  { }

	  const T operator*() const noexcept
	  { return val; }

	  const T* operator->() const noexcept
	  { return &val; }

	private:
	  T val;
	};

} // namespaqce __detail

  template<int _Rank>
	class index
	: private __detail::coordinate_facade<index<_Rank>, std::ptrdiff_t, _Rank>
	{
	  using Base = __detail::coordinate_facade<index<_Rank>, std::ptrdiff_t, _Rank>;
	  friend Base;

	public:
	  using Base::rank;
	  using reference       = typename Base::reference;
	  using const_reference = typename Base::const_reference;
	  using size_type       = typename Base::size_type;
	  using value_type      = typename Base::value_type;

	  constexpr index() noexcept : Base(){}
	  template<typename = std::enable_if_t<rank == 1>>
	  constexpr index(value_type e0) noexcept : Base(e0){}
	  constexpr index(std::initializer_list<value_type> il) : Base(il){}

	  using Base::operator[];
	  using Base::operator==;
	  using Base::operator!=;
	  using Base::operator+;
	  using Base::operator-;
	  using Base::operator+=;
	  using Base::operator-=;
	  using Base::operator++;
	  using Base::operator--;
	  using Base::operator*;
	  using Base::operator/;
	  using Base::operator*=;
	  using Base::operator/=;
	};

  template<>
	class index<1>
	{
	public:
	  static const int rank = 1;
	  using reference = std::ptrdiff_t&;
	  using const_reference = const std::ptrdiff_t&;
	  using size_type = std::size_t;
	  using value_type = std::ptrdiff_t;

	  constexpr
	  index() noexcept
	  : value(0)
	  { }

	  constexpr
	  index(value_type e0) noexcept
	  : value(e0)
	  { }

		// Preconditions: il.size() == rank
	  constexpr
	  index(std::initializer_list<value_type> il)
	  {
		assert(il.size() == rank);
		value = begin(il)[0];
	  }

		// Preconditions: component_idx < rank
	  constexpr reference
	  operator[](size_type component_idx) noexcept
	  {
		assert(component_idx == 0);
		(component_idx);
		return value;
	  }

		// Preconditions: component_idx < rank
	  constexpr const_reference
	  operator[](size_type component_idx) const noexcept
	  {
		assert(component_idx == 0);
		(component_idx);
		return value;
	  }

	  constexpr bool
	  operator==(const index& rhs) const noexcept
	  { return value == rhs.value; }

	  constexpr bool
	  operator!=(const index& rhs) const noexcept
	  { return !(*this == rhs); }

	  constexpr index
	  operator+() const noexcept
	  { return *this; }

	  constexpr index
	  operator-() const noexcept
	  { return index(-value); }

	  constexpr index operator+(const index& rhs) const noexcept
	  { return index(value + rhs.value); }

	  constexpr index operator-(const index& rhs) const noexcept
	  { return index(value - rhs.value); }

	  constexpr index&
	  operator+=(const index& rhs) noexcept
	  {
		value += rhs.value;
		return *this;
	  }

	  constexpr index&
	  operator-=(const index& rhs) noexcept
	  {
		value -= rhs.value;
		return *this;
	  }

	  constexpr index&
	  operator++() noexcept
	  {
		++value;
		return *this;
	  }

	  constexpr index
	  operator++(int) noexcept
	  {
		index ret = *this;
		++(*this);
		return ret;
	  }

	  constexpr index&
	  operator--() noexcept
	  {
		--value;
		return *this;
	  }

	  constexpr index
	  operator--(int) noexcept
	  {
		index ret = *this;
		--(*this);
		return ret;
	  }

	  constexpr index
	  operator*(value_type v) const noexcept
	  { return index(value * v); }

	  constexpr index
	  operator/(value_type v) const noexcept
	  { return index(value / v); }

	  constexpr index&
	  operator*=(value_type v) noexcept
	  {
		value *= v;
		return *this;
	  }

	  constexpr index&
	  operator/=(value_type v) noexcept
	  {
		value /= v;
		return *this;
	  }

	  friend constexpr index
	  operator*(value_type v, const index& rhs) noexcept
	  { return index(rhs * v); }

	private:
	  std::ptrdiff_t value;
	};

  template<int _Rank>
	struct bounds_iterator;

	// Preconditions (for all functions): after operation:
	//  - every component must be greater than or equal to zero
	//  - product of all components must not overflow ptrdiff_t
  template<int _Rank>
	class bounds
	: private __detail::coordinate_facade<bounds<_Rank>, std::ptrdiff_t, _Rank>
	{
	  using Base = __detail::coordinate_facade<bounds<_Rank>, std::ptrdiff_t, _Rank>;
	  friend Base;

	public:
	  using Base::rank;
	  using reference       = typename Base::reference;
	  using const_reference = typename Base::const_reference;
	  using iterator        = bounds_iterator<rank>;
	  using const_iterator  = bounds_iterator<rank>;
	  using size_type       = typename Base::size_type;
	  using value_type      = typename Base::value_type;

	  constexpr bounds() noexcept : Base(){}

	  template<typename = std::enable_if_t<rank == 1>>
		constexpr
		bounds(value_type e0)
		: Base(e0)
		{ assert(e0 >= 0); }

	  constexpr
	  bounds(std::initializer_list<value_type> il)
	  : Base(il)
	  {
#ifndef NDEBUG
		for (const auto& v : il)
		  assert(v >= 0);
#endif
	  }

	  using Base::operator[];
	  using Base::operator==;
	  using Base::operator!=;
	  using Base::operator*;
	  using Base::operator/;
	  using Base::operator*=;
	  using Base::operator/=;

	  constexpr bounds
	  operator+(const index<rank>& rhs) const
	  {
		auto ret = *this;
		ret += rhs;
		return ret;
	  }

	  constexpr bounds
	  operator-(const index<rank>& rhs) const
	  {
		auto ret = *this;
		ret -= rhs;
		return ret;
	  }

	  friend constexpr bounds
	  operator+(const index<rank>& lhs, const bounds& rhs)
	  { return rhs + lhs; }

	  constexpr bounds&
	  operator+=(const index<rank>& rhs)
	  {
		for (int i = 0; i < rank; ++i)
		  Base::elems[i] += rhs[i];
		return *this;
	  }

	  constexpr bounds&
	  operator-=(const index<rank>& rhs)
	  {
		for (int i = 0; i < rank; ++i)
		  Base::elems[i] -= rhs[i];
		return *this;
	  }

	  constexpr size_type
	  size() const noexcept
	  {
		size_type ret = Base::elems[0];
		for (int i = 1; i < rank; ++i)
		  ret *= Base::elems[i];
		return ret;
	  }

	  constexpr bool
	  contains(const index<rank>& idx) const noexcept
	  {
		for (int i = 0; i < rank; ++i)
		  if (idx[i] < 0 || idx[i] >= Base::elems[i])
			return false;
		return true;
	  }

	  const_iterator
	  begin() const noexcept
	  { return const_iterator{ *this }; }

	  const_iterator
	  end() const noexcept
	  {
		index<rank> idx_end;
		for (int i = 0; i < rank; ++i)
		  idx_end[i] = (*this)[i];
		return const_iterator{*this, idx_end};
	  }
	};

  template<int _Rank>
	struct bounds_iterator
	: public iterator<random_access_iterator_tag,
					  index<_Rank>,
					  std::ptrdiff_t,
					  const __detail::arrow_proxy<index<_Rank>>,
					  const index<_Rank>>
	{
		// Preconditions: bnd.contains(curr) unless bnd.size() == 0
	  explicit
	  bounds_iterator(bounds<_Rank> bnd, index<_Rank> curr = index<_Rank>{}) noexcept
	  : bnd{std::move(bnd)},
		curr{std::move(curr)}
	  { }

	  reference
	  operator*() const noexcept
	  { return curr; }

	  pointer
	  operator->() const noexcept
	  { return __detail::arrow_proxy<index<_Rank>>{curr}; }

	  bounds_iterator&
	  operator++() noexcept
	  {
		for (int i = _Rank; i-- > 0;)
		{
		  if (++curr[i] < bnd[i])
		    return *this;
		  else
			curr[i] = 0;
		}

		// If we're here we've wrapped over - set to past-the-end.
		for (int i = 0; i < _Rank; ++i)
		  curr[i] = bnd[i];
		return *this;
	  }

	  bounds_iterator operator++(int) noexcept
	  {
		auto ret = *this;
		++(*this);
		return ret;
	  }

	  bounds_iterator&
	  operator--() noexcept
	  {
		for (int i = _Rank; i-- > 0;)
		{
		  if (curr[i]-- > 0)
				  return *this;
		  else
				  curr[i] = bnd[i] - 1;
		}

		// If we're here the preconditions were violated
		// "pre: there exists s such that r == ++s"
		assert(false);
		return *this;
	  }

	  bounds_iterator
	  operator--(int) noexcept
	  {
		auto ret = *this;
		--(*this);
		return ret;
	  }

	  bounds_iterator
	  operator+(difference_type n) const noexcept
	  {
		bounds_iterator ret{ *this };
		return ret += n;
	  }

	  bounds_iterator&
	  operator+=(difference_type n) noexcept
	  {
		auto linear_idx = linearize(curr) + n;

		bounds<_Rank> stride;
		stride[_Rank - 1] = 1;
		for (int i = _Rank - 1; i-- > 0;)
		  stride[i] = stride[i + 1] * bnd[i + 1];

		for (int i = 0; i < _Rank; ++i)
		{
		  curr[i] = linear_idx / stride[i];
		  linear_idx = linear_idx % stride[i];
		}

		return *this;
	  }

	  bounds_iterator
	  operator-(difference_type n) const noexcept
	  {
		bounds_iterator ret{ *this };
		return ret -= n;
	  }

	  bounds_iterator&
	  operator-=(difference_type n) noexcept
	  { return *this += -n; }

	  difference_type
	  operator-(const bounds_iterator& rhs) const noexcept
	  { return linearize(curr) - linearize(rhs.curr); }

	  reference
	  operator[](difference_type n) const noexcept
	  { return *(*this + n); }

	  bool
	  operator==(const bounds_iterator& rhs) const noexcept
	  { return curr == rhs.curr; }

	  bool
	  operator!=(const bounds_iterator& rhs) const noexcept
	  { return !(*this == rhs); }

	  bool
	  operator<(const bounds_iterator& rhs) const noexcept
	  {
		for (int i = 0; i < _Rank; ++i)
		{
		  if (curr[i] < rhs.curr[i])
					return true;
		}
		return false;
	  }

	  bool
	  operator<=(const bounds_iterator& rhs) const noexcept
	  { return !(rhs < *this); }

	  bool
	  operator>(const bounds_iterator& rhs) const noexcept
	  { return rhs < *this; }

	  bool
	  operator>=(const bounds_iterator& rhs) const noexcept
	  { return !(rhs > *this); }

	  void
	  swap(bounds_iterator& rhs) noexcept
	  {
		std::swap(bnd, rhs.bnd);
		std::swap(curr, rhs.curr);
	  }

	private:
	  std::ptrdiff_t
	  linearize(const index<_Rank>& idx) const noexcept
	  {
		// TODO: Smarter impl.

		// Check if past-the-end
		bool pte = true;
		for (int i = 0; i < _Rank; ++i)
		{
		  if (idx[i] != bnd[i])
			{
					pte = false;
					break;
			}
		}

		std::ptrdiff_t multiplier = 1;
		std::ptrdiff_t res = 0;

		if (pte)
		{
		  res = 1;
		  for (int i = _Rank; i-- > 0;)
			{
					res += (idx[i] - 1) * multiplier;
					multiplier *= bnd[i];
			}
		}
		else
		{
		  for (int i = _Rank; i-- > 0;)
			{
					res += idx[i] * multiplier;
					multiplier *= bnd[i];
			}
		}

		return res;
	  }

	  bounds<_Rank> bnd;
	  index<_Rank> curr;
	};

  template<>
	struct bounds_iterator<1>
	: public iterator<random_access_iterator_tag,
					  index<1>,
					  std::ptrdiff_t,
					  const __detail::arrow_proxy<index<1>>,
					  const index<1>>
	{
		// Preconditions: bnd.contains(curr) unless bnd.size() == 0
	  explicit
	  bounds_iterator(bounds<1>, index<1> curr = index<1>{}) noexcept
	  : curr( std::move(curr) )
	  { }

	  reference
	  operator*() const noexcept
	  { return curr; }

	  pointer
	  operator->() const noexcept
	  { return __detail::arrow_proxy<index<1>>{ curr }; }

	  bounds_iterator&
	  operator++() noexcept
	  {
		++curr;
		return *this;
	  }

	  bounds_iterator
	  operator++(int) noexcept
	  {
		auto ret = *this;
		++(*this);
		return ret;
	  }

	  bounds_iterator&
	  operator--() noexcept
	  {
		curr--;
		return *this;
	  }

	  bounds_iterator
	  operator--(int) noexcept
	  {
		auto ret = *this;
		--(*this);
		return ret;
	  }

	  bounds_iterator
	  operator+(difference_type n) const noexcept
	  {
		bounds_iterator ret{ *this };
		return ret += n;
	  }

	  bounds_iterator&
	  operator+=(difference_type n) noexcept
	  {
		curr += n;
		return *this;
	  }

	  bounds_iterator
	  operator-(difference_type n) const noexcept
	  {
		bounds_iterator ret{ *this };
		return ret -= n;
	  }

	  bounds_iterator&
	  operator-=(difference_type n) noexcept
	  { return *this += -n; }

	  difference_type
	  operator-(const bounds_iterator& rhs) const noexcept
	  { return curr[0] - rhs.curr[0]; }

	  reference
	  operator[](difference_type n) const noexcept
	  { return curr + n; }

	  bool
	  operator==(const bounds_iterator& rhs) const noexcept
	  { return curr == rhs.curr; }

	  bool
	  operator!=(const bounds_iterator& rhs) const noexcept
	  { return !(*this == rhs); }

	  bool
	  operator<(const bounds_iterator& rhs) const noexcept
	  { return curr[0] < rhs.curr[0]; }

	  bool
	  operator<=(const bounds_iterator& rhs) const noexcept
	  { return !(rhs < *this); }

	  bool
	  operator>(const bounds_iterator& rhs) const noexcept
	  { return rhs < *this; }

	  bool
	  operator>=(const bounds_iterator& rhs) const noexcept
	  { return !(rhs > *this); }

	  void
	  swap(bounds_iterator& rhs) noexcept
	  { std::swap(curr, rhs.curr); }

	private:
	  index<1> curr;
	};

	template<int _Rank>
	bounds_iterator<_Rank>
	operator+(typename bounds_iterator<_Rank>::difference_type n,
			  const bounds_iterator<_Rank>& rhs) noexcept
	{ return rhs + n; }

} // inline namespace fundamentals_v2
} // namespace experimental
} // namespace std

#endif // _GLIBCXX_COORDINATE_H
