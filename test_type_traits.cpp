
// $HOME/bin/bin/g++ -std=c++14 -o test_type_traits test_type_traits.cpp

#include<experimental/type_traits>

int
main()
{
  static_assert(std::experimental::is_void_v<void> == true, "");
  static_assert(std::experimental::is_void_v<char> == false, "");

  static_assert(std::experimental::is_null_pointer_v<std::nullptr_t> == true, "");
  static_assert(std::experimental::is_null_pointer_v<char> == false, "");

  static_assert(std::experimental::is_integral_v<int> == true, "");
  static_assert(std::experimental::is_integral_v<float> == false, "");

  static_assert(std::experimental::is_floating_point_v<float> == true, "");
  static_assert(std::experimental::is_floating_point_v<int> == false, "");

  static_assert(std::experimental::is_array_v<char[5]> == true, "");
  static_assert(std::experimental::is_array_v<char> == false, "");
}
