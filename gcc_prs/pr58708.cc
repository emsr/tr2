// $HOME/bin/bin/g++ -std=c++1y -o pr58708 pr58708.cc

#include <iostream>
#include <type_traits>

template<typename CharT, CharT... str>
void
operator""_foo()
{
   CharT arr[]{str...};
   std::cout << '\n';
   std::cout << ' ' << std::is_same<CharT, char>::value 
             << ' ' << std::is_same<CharT, wchar_t>::value
             << ' ' << std::is_same<CharT, char16_t>::value
             << ' ' << std::is_same<CharT, char32_t>::value << '\n';
   std::cout << ' ' << std::is_same<CharT, const char>::value 
             << ' ' << std::is_same<CharT, const wchar_t>::value
             << ' ' << std::is_same<CharT, const char16_t>::value
             << ' ' << std::is_same<CharT, const char32_t>::value << '\n';
   for(CharT c : arr)
     std::cout << ' ' << (int)c;
   std::cout << '\n';
}

int
main()
{
   U"\x10000\x10001\x10002"_foo;

   "abcdefgh"_foo;

   L"abcdefgh"_foo;

   L"\x01020304\x05060708"_foo;

   u"\x0102\x0304\x0506\x0708"_foo;

   U"\x01020304\x05060708"_foo;
}
