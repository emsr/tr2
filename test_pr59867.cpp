// $HOME/bin/bin/g++ -std=c++14 -o test_pr59867 test_pr59867.cpp

#include <iostream>
using namespace std;

// constant
template<typename T, T x>
  struct meta_value
  {
    typedef meta_value type;
    typedef T value_type;
    static const T value = x;
  };

// array
template<typename T, T... data>
  struct meta_array
  {
    typedef meta_array type;
    typedef T item_type;
  };

// static array -> runtime array conversion utility
template<typename T>
  struct array_gen;

template<typename T, T... xs>
  struct array_gen<meta_array<T, xs...>>
  {
    static const T value[sizeof...(xs)];
  };

template<typename T, T... xs>
  const T
  array_gen<meta_array<T, xs...>>::value[sizeof...(xs)] = {xs...};

// static string
template<typename T, T... xs>
  constexpr meta_array<T, xs...>
  operator""_s()
  {
    static_assert(sizeof...(xs) == 3, "What's wrong with you?");
    //static_assert(sizeof...(xs) != 3, "What's wrong with you?");
    return meta_array<T, xs...>();
  }

int
main()
{
  //  This used to be [2] - invalid ref
  auto a = "123"_s;
  const char (& xs)[3] = array_gen<decltype("123"_s)>::value;
  for (auto & x : xs)
    cout << x << endl;
}
