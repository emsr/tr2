
// /home/ed/bin/bin/g++ -std=c++14 -o test_string_literal test_string_literal.cpp

#include <string>
#include "string_literal"

int
main()
{
  using namespace std::literals;

  std::string_literal<'a', 'b', 'c'> a;
  auto aa = "abc"S;

}

