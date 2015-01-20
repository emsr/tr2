// /home/ed/bin/bin/g++ -std=c++14 -o test_make_array test_make_array.cpp

#include "make_array"

int
main()
{
  auto a = std::experimental::make_array(0.0, 1.0, 2.0);
}
