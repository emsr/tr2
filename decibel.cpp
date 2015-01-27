
// /home/ed/bin/bin/g++ -std=c++14 -o decibel decibel.cpp

#include <iostream>

#include "decibel.h"

using namespace decibel_literals;

int
main()
{
  decibel<float> deff;
  std::cout << "deff = " << deff << '\n';

  decibel<double> def;
  std::cout << "def  = " << def << '\n';

  decibel<long double> defl;
  std::cout << "defl = " << defl << '\n';

  auto half = -3_dB;
  std::cout << "half = " << half << '\n';
  std::cout << "half.power() = " << half.power() << '\n';

  std::cout << std::boolalpha;

  bool eq = (-3_dB == 3_dB);
  bool ne = (-3_dB != 3_dB);
  bool lt = (-3_dB < 3_dB);
  bool ge = (-3_dB >= 3_dB);
  bool gt = (-3_dB > 3_dB);
  bool le = (-3_dB <= 3_dB);
  std::cout << "(-3_dB == 3_dB): " << eq << '\n';
  std::cout << "(-3_dB != 3_dB): " << ne << '\n';
  std::cout << "(-3_dB  < 3_dB): " << lt << '\n';
  std::cout << "(-3_dB >= 3_dB): " << ge << '\n';
  std::cout << "(-3_dB  > 3_dB): " << gt << '\n';
  std::cout << "(-3_dB <= 3_dB): " << le << '\n';

  auto loss = 54_dB + half;
  std::cout << loss << '\n';

  auto gain = 16.4_dB - half;
  std::cout << gain << '\n';
}
