// /home/ed/bin/bin/g++ -o factoid factoid.cpp

// LD_LIBRARY_PATH=/home/ed/bin/bin/lib64:$LD_LIBRARY_PATH ./factoid

#include <cstdlib>
#include <iostream>
#include <array>

template<typename _UIntTp>
  constexpr size_t
  fact_size()
  {
    size_t sz = 1;
    _UIntTp prev = 0, fact = 1, term = 1;
    while (prev < (fact *= ++term))
      ++sz;
    return sz;
  }


int
main()
{
  std::array<unsigned long long, fact_size<unsigned short>()> aus;
  std::cout << "\naus.size() = " << aus.size() << '\n';

  std::array<unsigned long long, fact_size<unsigned int>()> aui;
  std::cout << "\naui.size() = " << aui.size() << '\n';

  std::array<unsigned long long, fact_size<unsigned long>()> aul;
  std::cout << "\naul.size() = " << aul.size() << '\n';

  std::array<unsigned long long, fact_size<unsigned long long>()> aull;
  std::cout << "\naull.size() = " << aull.size() << '\n';
}
