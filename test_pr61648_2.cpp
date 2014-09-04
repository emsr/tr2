// /home/ed/bin_tr2/bin/g++ -std=c++14 -o test_pr61648_2 test_pr61648_2.cpp

#include <cstddef>

struct B {};

template<char... Str>
  B operator""_c() { return B(); }

template<>
  B operator""_c<'4','2','.','0'>() { return B(); }

template<typename T>
  class C
  {};

template<>
  class C<float>
  {
    friend B operator""_c<'4','2','.','0'>();
  };
