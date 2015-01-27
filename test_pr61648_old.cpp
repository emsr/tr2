// /home/ed/bin_tr2/bin/g++ -std=c++14 -o test_pr61648_old test_pr61648_old.cpp

#include <cstddef>

struct B {};

B
operator+(B, B)
{ return B(); }

B
operator""_b(const char *, size_t)
{ return B(); }

template<typename T>
  class C
  {
    friend B operator+(B, B);
    friend B operator""_b(const char *, size_t);
  };

int
main()
{ return 0; }
