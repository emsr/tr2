// /home/ed/bin/bin/g++ -std=c++1y -o binary_literal_orig binary_literal_orig.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./binary_literal_orig

#include <iostream>

int
main()
{
  int i = 0b01010101010101010101010101010101;
  std::cout << std::bin << std::showbase << i << std::endl;
  std::cout << "flags: " << std::cout.flags() << std::endl;
  std::cout << "bin: " << std::boolalpha << (std::cout.flags() & std::ios_base::bin) << std::endl;
  std::cout << "oct: " << std::boolalpha << (std::cout.flags() & std::ios_base::oct) << std::endl;
  std::cout << "dec: " << std::boolalpha << (std::cout.flags() & std::ios_base::dec) << std::endl;
  std::cout << "hex: " << std::boolalpha << (std::cout.flags() & std::ios_base::hex) << std::endl;
  std::cout << std::bin << std::showbase << &i << std::endl;
  std::cout << std::bin << std::showbase << bool(i) << std::endl;

  int j;
  std::cout << "Enter binary integer: ";
  std::cin >> std::bin >> j;
  std::cout << std::hex << "hex: " << j << std::endl;
  std::cout << std::dec << "dec: " << j << std::endl;
  std::cout << std::oct << "oct: " << j << std::endl;
  std::cout << std::bin << "bin: " << j << std::endl;

  int k;
  std::cout << "Enter binary integer with base: ";
  std::cin >> std::showbase >> std::bin >> k;
  std::cout << std::hex << "hex: " << k << std::endl;
  std::cout << std::dec << "dec: " << k << std::endl;
  std::cout << std::oct << "oct: " << k << std::endl;
  std::cout << std::bin << "bin: " << k << std::endl;
}
