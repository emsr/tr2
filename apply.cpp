
//  n3915 - apply() call a function with arguments from a tuple (V2) - Peter Sommerlad - 2013-12-23

// $HOME/bin/bin/g++ -std=c++1y -o apply apply.cpp

#include<tuple>
#include<iostream>
#include<cmath>

//  This should go into <tuple>.

namespace std
{
  template<typename _Fn, typename _Tuple, std::size_t... _Idx>
    decltype(auto)
    __apply_helper(_Fn&& __f, _Tuple&& __t, std::index_sequence<_Idx...>)
    {
      return std::forward<_Fn>(__f)
	     (std::get<_Idx>(std::forward<_Tuple>(__t))...);
    }

  template<typename _Fn, typename _Tuple>
    decltype(auto)
    apply(_Fn&& __f, _Tuple&& __t)
    {
      using _Size = std::tuple_size<std::decay_t<_Tuple>>;
      using _Indices = std::make_index_sequence<_Size::value>;
      return __apply_helper(std::forward<_Fn>(__f),
			    std::forward<_Tuple>(__t), _Indices{});
    }
} // namespace std

double
foo(unsigned int p, double x, double y, double z)
{
  double xp = 1.0, yp = 1.0, zp = 1.0;
  for (unsigned int i = 0; i < p; ++i)
    {
      xp *= x;
      yp *= y;
      zp *= z;
    }
  return xp + yp + zp;
}
/*
template<typename... _Tp>
  std::common_type_t<_Tp...>
  bar(unsigned int p, _Tp... x)
  {
    using _Up = std::common_type_t<_Tp...>;
    _Up a[]{std::abs(x)...};
    _Up s{};
    for (std::size_t id = 0; id < sizeof...(x); ++id)
    {
      _Up xp{1};
      for (unsigned int i = 0; i < p; ++i)
	xp *= a[id];
      s += xp;
    }
    return s;
  }
*/
int
main()
{
  std::cout << std::apply(foo, std::make_tuple(4U, 1.1, 2.2, 3.3)) << '\n';
  std::cout << bar(4U, 1.1, 2.2, 3.3) << '\n';
  //std::cout << std::apply(bar, std::make_tuple(4U, 1.1, 2.2, 3.3)) << '\n';
}
