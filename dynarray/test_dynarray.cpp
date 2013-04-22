// http://www.open-std.org/JTC1/sc22/WG21/docs/papers/2013/n3532.html

// /home/ed/bin/bin/g++ -std=c++1y -o test_dynarray test_dynarray.cpp

#include "dynarray"
#include <iostream>
#include <algorithm>

void
dump(const std::dynarray< int > & source)
{
  for (auto src = source.begin(); src != source.end(); ++src)
    std::cout << " " << *src;
  std::cout << std::endl;
}

void
lowrap(std::dynarray<int>& target,
       const std::dynarray<int>& source)
{
  dump(source);

  std::dynarray<int> sorted(source);
  dump(sorted);

  std::sort(sorted.begin(), sorted.end());
  dump(sorted);

  const int* srt = &sorted.front();
  std::dynarray< int >::iterator tgt(target.begin());
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
  lowrap( alpha, gamma );
  int sum = 0;
  for (std::dynarray<int>::size_type i = 0; i < alpha.size(); ++i)
    sum += alpha.at(i);
  return sum;
}
