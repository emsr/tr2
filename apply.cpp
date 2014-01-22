
//  n3829 - apply() call a function with arguments from a tuple (V2) - Peter Sommerlad - 2013-12-23
// /home/ed/bin/bin/g++ -std=c++1y -o apply apply.cpp

#include<tuple>

//  This should go into <tuple>.

template <typename _Fn, typename _Tuple, std::size_t... _Idx>
  decltype(auto)
  apply_impl(_Fn&& __f, _Tuple&& __t, std::index_sequence<_Idx...>)
  {
    return std::forward<_Fn>(__f)(std::get<_Idx>(std::forward<_Tuple>(__t))...);
  }

template <typename _Fn, typename _Tuple>
  decltype(auto)
  apply(_Fn&& __f, _Tuple&& __t)
  {
    using _Size = std::tuple_size<std::decay_t<_Tuple>>;
    using _Indices = std::make_index_sequence<_Size::value>;
    return apply_impl(std::forward<_Fn>(__f),
		      std::forward<_Tuple>(__t), _Indices{});
  }

int
main()
{
}
