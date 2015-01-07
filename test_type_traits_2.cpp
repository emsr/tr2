
// /home/ed/bin/bin/g++ -std=c++14 -o test_type_traits_2 test_type_traits_2.cpp

#include<experimental/type_traits>
#include<iostream>

#if __cplusplus >= 201300L // > 201402L
//namespace std {
//  template<typename _Tp>
//    constexpr bool
//    is_void_v = is_void<_Tp>::value;
//}
#endif // __cplusplus >= 201300L // > 201402L

int
main()
{
  std::cout << std::boolalpha;
  auto c = std::experimental::is_void_v<char>;
  std::cout << "is_void_v<char> = " << c << std::endl;
  auto v = std::experimental::is_void_v<void>;
  std::cout << "is_void_v<void> = " << v << std::endl;
}
