// { dg-options -std=c++11 }

#include <cstddef>

int
operator""_a(const char *, std::size_t = 0) // { dg-error "has invalid argument list" }
{
  return 1;
}

int
operator""_a(const char *)
{
  return 2;
}

template<char... Str>
  int
  operator"" _b(std::size_t = 0) // { dg-error "has invalid argument list" }
  {
    return 1 + sizeof...(Str);
  }

template<char... Str>
  int
  operator"" _b()
  {
    return 1 + sizeof...(Str);
  }

int
main()
{
  int i = 123_a; // OK, raw literal, not ambiguous.
  int j = operator""_a("123"); // OK, raw literal, not ambiguous.
  int k = 123_b;
  int l = operator""_b<'1', '2', '3'>();
}
