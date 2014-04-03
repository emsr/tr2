// http://www.open-std.org/JTC1/sc22/WG21/docs/papers/2013/n3532.html

// /home/ed/bin/bin/g++ -std=c++1y -o test_dynarray test_dynarray.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_dynarray

#include "dynarray"
#include <iostream>
#include <algorithm>
#include <complex>

template<typename Tp>
  void
  dump(const std::string& name, const std::dynarray<Tp> & source)
  {
    std::cout << name << ": ";
    for (auto src = source.begin(); src != source.end(); ++src)
      std::cout << " " << *src;
    std::cout << std::endl;
  }

template<typename Tp>
  void
  lowrap(const std::string& target_name, std::dynarray<Tp>& target,
         const std::string& source_name, const std::dynarray<Tp>& source)
  {
    dump(source_name, source);

    std::dynarray<Tp> sorted{source};
    dump("sorted_" + source_name, sorted);

    std::sort(sorted.begin(), sorted.end());
    dump("sorted_" + source_name, sorted);

    const Tp* srt = &sorted.front();
    typename std::dynarray<Tp>::iterator tgt{target.begin()};
    for (; tgt != target.end(); ++tgt)
    {
      *tgt = *srt;
      if (srt == &sorted.back())
        srt = &sorted.front();
      else
        ++srt;
    }

    dump(target_name, target);
  }

int
main()
{
  using namespace std::literals::complex_literals;

  std::dynarray<int> alpha(8);
  dump("alpha", alpha);
  std::dynarray<int> gamma(3);
  dump("gamma", gamma);

  for (std::dynarray<int>::size_type i = 0; i < gamma.size(); ++i)
    gamma[i] = 4 - i;

  lowrap("alpha", alpha, "gamma", gamma);

  std::cout << std::boolalpha;
  std::cout << "alpha == gamma: " << (alpha == gamma) << std::endl;
  std::cout << "alpha != gamma: " << (alpha != gamma) << std::endl;
  std::cout << "alpha <  gamma: " << (alpha <  gamma) << std::endl;
  std::cout << "alpha >= gamma: " << (alpha >= gamma) << std::endl;
  std::cout << "alpha >  gamma: " << (alpha >  gamma) << std::endl;
  std::cout << "alpha <= gamma: " << (alpha <= gamma) << std::endl;

  int sum = 0;
  for (std::dynarray<int>::size_type i = 0; i < alpha.size(); ++i)
    sum += alpha.at(i);

  std::dynarray<float> fda{0.0F, 1.1F, 2.2F, 3.3F, 4.4F, 5.5F};
  dump("fda", fda);

  std::dynarray<std::complex<double>> beta(1000, 1.0i);
  for (std::dynarray<std::complex<double>>::size_type i = 0; i < beta.size(); i += 100)
    std::cout << ' ' << beta[i];
  std::cout << std::endl;

  std::dynarray<int> alpha2{alpha};
  dump("alpha2", alpha2);
  alpha2.fill(6);
  dump("alpha2", alpha2);

  std::dynarray<int> gamma2{gamma};
  dump("gamma2", gamma2);
  std::cout << "gamma2 == gamma: " << (gamma2 == gamma) << std::endl;
  std::cout << "gamma2 != gamma: " << (gamma2 != gamma) << std::endl;
  std::cout << "gamma2 <  gamma: " << (gamma2 <  gamma) << std::endl;
  std::cout << "gamma2 >= gamma: " << (gamma2 >= gamma) << std::endl;
  std::cout << "gamma2 >  gamma: " << (gamma2 >  gamma) << std::endl;
  std::cout << "gamma2 <= gamma: " << (gamma2 <= gamma) << std::endl;

  std::dynarray<int> zero(0);
  dump("zero", zero);
  std::cout << "zero.begin() == zero.end(): " << (zero.begin() == zero.end()) << std::endl;

  return sum;
}
