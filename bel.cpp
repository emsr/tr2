
// $HOME/bin/bin/g++ -std=c++14 -o bel bel.cpp

#include <iostream>
#include <iomanip>

#include "bel.h"

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

  std::cout << std::setprecision(std::numeric_limits<decltype(3_dB)::value_type>::digits10);

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
  std::cout << "loss = " << loss << '\n';

  auto gain = 16.4_dB - half;
  std::cout << "gain = " << gain << '\n';

  auto nloss = 54_dB;
  nloss += half;
  std::cout << "nloss = " << nloss << '\n';

  auto ngain = 16.4_dB;
  ngain -= half;
  std::cout << "ngain = " << ngain << '\n';

  bel<double> w{6.66};
  std::cout << "w   = " << w << " B\n";
  decibel<double> dw{w};
  std::cout << "dw  = " << dw << " dB\n";
  level<double, std::centi> cw{w};
  std::cout << "cw  = " << cw << " cB\n";
  level<double, std::milli> mw{w};
  std::cout << "mw  = " << mw << " mB\n";

  decibel<double> big{9999.0};
  std::cout << "big = " << big << '\n';

  centibel<float> cB{30.0};
  std::cout << "cB  = " << cB << " cB\n";
  std::cout << "cB (power) = " << cB.power() << "\n";
  std::cout << "3 dB (power) = " << 3_dB .power() << "\n";
  cB -= 3_dB;
  std::cout << "cB  = " << cB << " cB\n";
  cB = centibel<float>{30.0};
  std::cout << "cB (power) = " << cB.power() << "\n";
  std::cout << "-3 dB (power) = " << -3_dB .power() << "\n";
  cB += -3_dB;
  std::cout << "cB  = " << cB << " cB\n";
}
