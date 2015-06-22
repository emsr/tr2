// $HOME/bin_git/bin/g++ -std=c++14 -o is_final_std is_final_std.cpp

#include<type_traits>
#include<iostream>

class C1 {};

class C2 : C1 {};

class Cn final : C2 {};

int
main()
{
#ifdef __cpp_lib_is_final
#if __cpp_lib_is_final == 201402L
  std::cout << "std::is_final<C1>::value = " << std::is_final<C1>::value << '\n';
  std::cout << "std::is_final<C2>::value = " << std::is_final<C2>::value << '\n';
  std::cout << "std::is_final<Cn>::value = " << std::is_final<Cn>::value << '\n';
  std::cout << "std::is_final<Cn>::value = " << std::is_final<const Cn>::value << '\n';
  std::cout << "std::is_final<Cn>::value = " << std::is_final<volatile Cn>::value << '\n';
  std::cout << "std::is_final<Cn>::value = " << std::is_final<const volatile Cn>::value << '\n';
#else
#error "__cpp_lib_is_final == " __cpp_lib_is_final
#endif
#else
#error "__cpp_lib_is_final not defined"
#endif
}

