// $HOME/bin/bin/g++ -std=c++1y -o bintest bintest.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./bintest

#include <iostream>
#include <iomanip>

int
main()
{
  std::cout << __gnu_cxx::__bin << 123 << '\n';
  int n;
  std::cout << "Enter n in binary: ";
  std::cin >> __gnu_cxx::__bin >> n;
  std::cout << std::showbase << __gnu_cxx::__bin << "bin: " << n << '\n';
  std::cout << std::showbase << std::oct << "oct: " << n << '\n';
  std::cout << std::showbase << std::dec << "dec: " << n << '\n';
  std::cout << std::showbase << std::hex << "hex: " << n << '\n';
}
