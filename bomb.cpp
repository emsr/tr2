
// /home/ed/bin/bin/g++ -std=c++1y -o bomb bomb.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./bomb 

#include <iostream>
#include <stdexcept>
#include "string_view"

int
main()
{
  std::experimental::string_view sview{"Hello, World!"};
  std::experimental::string_view snull{nullptr};
  try
  {
    sview.at(15);
  }
  catch(std::out_of_range err)
  {
    std::cerr << err.what() << std::endl;
  }
  try
  {
    snull.at(15);
  }
  catch(std::out_of_range err)
  {
    std::cerr << err.what() << std::endl;
  }

  snull.swap(sview);
  try
  {
    sview.at(15);
  }
  catch(std::out_of_range err)
  {
    std::cerr << err.what() << std::endl;
  }
  try
  {
    snull.at(15);
  }
  catch(std::out_of_range err)
  {
    std::cerr << err.what() << std::endl;
  }
}
