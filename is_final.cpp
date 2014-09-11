// /home/ed/bin/bin/g++ -std=c++14 -o is_final is_final.cpp

#include<type_traits>
#include<iostream>

namespace std
{
  template<bool>
    struct __is_final_helper
    : public false_type { };

  template<>
    struct __is_final_helper<true>
    : public true_type { };

  /// is_final
  template<typename _Tp>
    struct is_final
    : public __is_final_helper<__is_final(typename remove_cv<_Tp>::type)>::type
    { };
}

class C1 {};

class C2 : C1 {};

class Cn final : C2 {};

int
main()
{
  std::cout << "std::is_final<C1>::value = " << std::is_final<C1>::value << '\n';
  std::cout << "std::is_final<C2>::value = " << std::is_final<C2>::value << '\n';
  std::cout << "std::is_final<Cn>::value = " << std::is_final<Cn>::value << '\n';
}
