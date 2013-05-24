#include <limits>
#include <iostream>
#include <tr2/ratio>

//  I added the new ratios to gcc trunk in 2011.

int
main()
{
  std::cout << "kibi: " << std::tr2::kibi::num << std::endl;
  std::cout << "mebi: " << std::tr2::mebi::num << std::endl;
  std::cout << "gibi: " << std::tr2::gibi::num << std::endl;
  std::cout << "tebi: " << std::tr2::tebi::num << std::endl;
  std::cout << "pebi: " << std::tr2::pebi::num << std::endl;
  std::cout << "exbi: " << std::tr2::exbi::num << std::endl;
}
