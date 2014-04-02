// http://www.open-std.org/JTC1/sc22/WG21/docs/papers/2013/n3532.html

// /home/ed/bin/bin/g++ -std=c++1y -o test_dynarray test_dynarray.cpp

#include "dynarray"
#include <iostream>
#include <algorithm>

template<typename Tp>
  void
  dump(const std::dynarray<Tp> & source)
  {
    for (auto src = source.begin(); src != source.end(); ++src)
      std::cout << " " << *src;
    std::cout << std::endl;
  }

template<typename Tp>
  void
  lowrap(std::dynarray<Tp>& target,
         const std::dynarray<Tp>& source)
  {
    dump(source);

    std::dynarray<Tp> sorted(source);
    dump(sorted);

    std::sort(sorted.begin(), sorted.end());
    dump(sorted);

    const Tp* srt = &sorted.front();
    typename std::dynarray<Tp>::iterator tgt(target.begin());
    for (; tgt != target.end(); ++tgt)
    {
      *tgt = *srt;
      if (srt == &sorted.back())
        srt = &sorted.front();
      else
        ++srt;
    }

    dump(target);
  }

int
main()
{
  std::dynarray<int> alpha(8);
  std::dynarray<int> gamma(3);

  for (std::dynarray<int>::size_type i = 0; i < gamma.size(); ++i)
    gamma[i] = 4 - i;

  lowrap(alpha, gamma);

  int sum = 0;
  for (std::dynarray<int>::size_type i = 0; i < alpha.size(); ++i)
    sum += alpha.at(i);

  std::dynarray<float> fda{0.0F, 1.1F, 2.2F, 3.3F, 4.4F, 5.5F};
  dump(fda);

  return sum;
}
