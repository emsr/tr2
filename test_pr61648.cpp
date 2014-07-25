// /home/ed/bin/bin/g++ -std=c++14 -o test_pr61648 test_pr61648.cpp

#include <cstddef>

struct B {};

B operator+(B, B) { return B(); }
B operator""_b(const char *, size_t) { return B(); }

template<char... Str>
  B operator""_c() { return B(); }

template<>
  B operator""_c<'4','2','.','0'>() { return B(); }

template<typename T>
  class C
  {
    friend B operator+(B, B);
    friend B operator""_b(const char *, size_t);

    template<char...>
      friend B operator""_c();
  };

template<>
  class C<int>
  {
    template<char...>
      friend B operator""_c();
  };

template<>
  class C<float>
  {
    friend B operator""_c<'4','2','.','0'>();
  };

int main() { return 0; }
