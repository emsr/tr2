//  libstdc++/64351 - std::uniform_real_distribution<float>(0, 1) returns 1

// /home/ed/bin/bin/g++ -std=c++14 -o pr64351 pr64351.cpp

#include <random>
#include <cstdint>
#include <iostream>

template<typename _Tp>
  void
  test()
  {
    std::mt19937 rng(8890);
    std::uniform_real_distribution<_Tp> dist;

    uint64_t num_1 = 0;
    for (uint64_t i = 0; i < 39975715; ++i)
    {
      if (dist(rng) == 1.0F)
        ++num_1;
    }

    std::cout << num_1 << std::endl;
  }

int
main()
{
  test<float>();

  test<double>();

  return 0;
}
