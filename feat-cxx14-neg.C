// /home/ed/bin_feature/bin/g++ -std=gnu++1y -c feat-cxx14-neg.C
// { dg-do compile { target c++14 } }

#ifndef __cpp_constexpr
#  error "__cpp_constexpr"
#elif __cpp_constexpr != 200704
#  error "__cpp_constexpr" // { dg-error "error" }
#endif
