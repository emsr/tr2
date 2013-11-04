
// /home/ed/bin/bin/g++ -std=c++1y -o test_string_view test_string_view.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_string_view 

#include "string_view"
#include <iostream>
#include <stdexcept>

int
main()
{
  std::string_view sv{"Hello, World!"};
  std::cout << "string_view   = " << sv << std::endl;
  std::cout << "empty()       = " << std::boolalpha << sv.empty() << std::endl;
  std::cout << "size()        = " << sv.size() << std::endl;
  std::cout << "length()      = " << sv.length() << std::endl;
  std::cout << "front()       = " << sv.front() << std::endl;
  std::cout << "back()        = " << sv.back() << std::endl;
  try
  {
    std::cout << "at(16)        = " << sv.at(16) << std::endl;
  }
  catch(std::out_of_range err)
  {
    std::cerr << err.what() << std::endl;
  }
  std::cout << "operator[](5) = " << sv[5] << std::endl;
  sv.remove_prefix(3);
  std::cout << "remove_prefix(3) = " << sv << std::endl;
  sv.remove_prefix(1);
  std::cout << "remove_suffix(1) = " << sv << std::endl;
  std::cout << "substr(1, 5) = " << sv.substr(1, 5) << std::endl;

  // I added literals.

  using namespace std::literals::string_view_literals;

  auto uh = "Uranus-Hertz"sv;
  std::cout << "empty()       = " << std::boolalpha << uh.empty() << std::endl;
  std::cout << "size()        = " << uh.size() << std::endl;
  std::cout << "length()      = " << uh.length() << std::endl;
  std::cout << "front()       = " << uh.front() << std::endl;
  std::cout << "back()        = " << uh.back() << std::endl;

}
