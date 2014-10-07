// /home/ed/bin/bin/g++ -std=c++14 -fdiagnostics-color=auto -o test_random test_random.cpp

#include <random>
#include "random" // n3742: Three <random>-related Proposals, v2
#include "random2"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <iterator>

int
main()
{
  std::vector<int> pop;
  for (int i = -1000; i <= +1000; ++i)
    pop.push_back(i);
  for (int i = -1000; i <= +1000; ++i)
    pop.push_back(2*i);
  for (int i = -1000; i <= +1000; ++i)
    pop.push_back(5*i);

  using std::experimental::sample;

  std::vector<int> out(200);
  auto stop = sample(std::begin(pop), std::end(pop), std::begin(out), 100, std::mt19937{});
  for (auto i : out)
    std::cout << ' ' << std::setw(5) << i;
  std::cout << '\n';

  for (auto i = std::begin(out); i != stop; ++i)
  {
    if ((i - std::begin(out)) % 5 == 0)
      std::cout << '\n';
    std::cout << ' ' << std::setw(5) << *i;
  }
  std::cout << '\n';

  std::vector<int> out2(200);
  std::ostringstream osmpstr{};
  for (int i = -1000; i <= +1000; ++i)
    osmpstr << ' ' << i;
  for (int i = -1000; i <= +1000; ++i)
    osmpstr << ' ' << i;
  for (int i = -1000; i <= +1000; ++i)
    osmpstr << ' ' << i;
  std::istringstream ismpstr{osmpstr.str()};

  auto stop2 = sample(std::istream_iterator<int>{ismpstr}, std::istream_iterator<int>{},
                      std::begin(out2), 100, std::mt19937{});
  for (auto&& i : out2)
    std::cout << ' ' << std::setw(5) << i;
  std::cout << '\n';

  for (auto&& i = std::begin(out2); i != stop2; ++i)
  {
    if ((i - std::begin(out2)) % 5 == 0)
      std::cout << '\n';
    std::cout << ' ' << std::setw(5) << *i;
  }
  std::cout << '\n';
}
