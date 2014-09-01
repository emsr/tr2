// /home/ed/bin/bin/g++ -std=c++1y -o test_digit_sep test_digit_sep.cpp

#include <iostream>

int
main()
{
  std::cout << std::boolalpha << (1048576 == 1'048'576) << std::endl;
  std::cout << std::boolalpha << (1048576 == 0X100000) << std::endl;
  std::cout << std::boolalpha << (1048576 == 0x10'0000) << std::endl;
  std::cout << std::boolalpha << (1048576 == 0'004'000'000) << std::endl;
  std::cout << std::boolalpha << (1048576 == 0B100000000000000000000) << std::endl;
  std::cout << std::boolalpha << (1048576 == 0b0001'0000'0000'0000'0000'0000) << std::endl;

  std::cout << (1.602'176'565e-19 == 1.602176565e-19) << std::endl;

  std::cout.precision(10);
  double x = 3.141593654;
  std::cout << x << std::endl;
  double y = 3.141'593'654;
  std::cout << y << std::endl;

  unsigned int i = 3141593654;
  std::cout << i << std::endl;
  unsigned int j = 3'141'593'654;
  std::cout << j << std::endl;
}
