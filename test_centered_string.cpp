// $HOME/bin/bin/g++ -o test_centered_string test_centered_string.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./test_centered_string

#include <iostream>
#include <iomanip>
#include "centered_string.h"

int
main()
{
  std::cout << std::setw(11) << "Hi!" << '\n';
  std::cout << std::setw(11) << centered("Hi!") << '\n';

  std::string str{"There!!"};
  std::cout << std::setw(22) << str << '\n';
  std::cout << std::setw(22) << centered(str) << '\n';

  std::experimental::string_view view{"Cowboy!!!"};
  std::cout << std::setw(33) << view << '\n';
  std::cout << std::setw(33) << centered(view) << '\n';
}
