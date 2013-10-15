
// /home/ed/bin/bin/g++ -std=c++1y -o literal_bitset literal_bitset.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./literal_bitset

#include <bitset>
#include <iostream>

template<typename _CharT, _CharT... chars>
  /*constexpr*/ std::bitset<sizeof...(chars)>
  operator""_bs()
  {
    const _CharT __str[]{chars...};
    return std::bitset<sizeof...(chars)>{__str, sizeof...(chars)};
  }

int
main()
{
  auto bs1 = "010101010101111100"_bs;
  std::cout << bs1 << std::endl;
}

// A better way might be to loop through and set bits after default ctor.
// This will work with constexpr after the latter gets C++1y freedoms.
