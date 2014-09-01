
// /home/ed/bin/bin/g++ -std=c++11 -o test_cuchar test_cuchar.cpp

#include <cwchar>
#include <cuchar>
#include <iostream>
#include <bits/c++config.h>

int
test_c16rtomb()
{
  const char16_t* pt = u"Juan Souli\u00e9";

  std::mbstate_t mbs{};

  size_t total = 0;
  char buffer[MB_CUR_MAX];
  while (*pt)
  {
    size_t length = std::c16rtomb(buffer, *pt, &mbs);
    total += length;
    if (length == 0 || length > MB_CUR_MAX)
      break;
    for (int i = 0; i < length; ++i)
      std::cout.put(buffer[i]);
    ++pt;
  }
  std::cout << '\n';
  std::cout << "total: " << total << '\n';

  return 0;
}

int
test_c32rtomb()
{
  const char32_t* pt = U"Juan Souli\u00e9";

  std::mbstate_t mbs{};

  size_t total = 0;
  char buffer[MB_CUR_MAX];
  while (*pt)
  {
    size_t length = std::c32rtomb(buffer, *pt, &mbs);
    total += length;
    if (length == 0 || length > MB_CUR_MAX)
      break;
    for (int i = 0; i < length; ++i)
      std::cout.put(buffer[i]);
    ++pt;
  }
  std::cout << '\n';
  std::cout << "total: " << total << '\n';

  return 0;
}


int
main()
{
#ifdef HAVE_UCHAR_H
  std::cout << "HAVE_UCHAR_H : " << HAVE_UCHAR_H << std::endl;
#endif
#ifdef _GLIBCXX_USE_C11_UCHAR_CXX11
  std::cout << "_GLIBCXX_USE_C11_UCHAR_CXX11 : " << _GLIBCXX_USE_C11_UCHAR_CXX11 << std::endl;
#endif
#ifdef _GLIBCXX_HAVE_UCHAR_H
  std::cout << "_GLIBCXX_HAVE_UCHAR_H : " << _GLIBCXX_HAVE_UCHAR_H << std::endl;
#endif
#ifdef _GLIBCXX_HAVE_MBSTATE_T
  std::cout << "_GLIBCXX_HAVE_MBSTATE_T : " << _GLIBCXX_HAVE_MBSTATE_T << std::endl;
#endif

#ifdef __STDC_UTF_16__
  std::cout << "__STDC_UTF_16__ : " << __STDC_UTF_16__ << std::endl;
#endif
  test_c16rtomb();

#ifdef __STDC_UTF_32__
  std::cout << "__STDC_UTF_32__ : " << __STDC_UTF_32__ << std::endl;
#endif
  test_c32rtomb();
}
