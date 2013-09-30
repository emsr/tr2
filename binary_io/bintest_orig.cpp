// /home/ed/bin/bin/g++ -std=c++1y -o bintest_orig bintest_orig.cpp 

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./bintest_orig

#include <iostream>
#include <iomanip>

int
main()
{
  std::cout << std::bin << 123 << '\n';
  int n;
  std::cout << "Enter n in binary: ";
  std::cin >> std::bin >> n;
  std::cout << std::showbase << std::bin << "bin: " << n << '\n';
  std::cout << std::showbase << std::oct << "oct: " << n << '\n';
  std::cout << std::showbase << std::dec << "dec: " << n << '\n';
  std::cout << std::showbase << std::hex << "hex: " << n << '\n';
}
