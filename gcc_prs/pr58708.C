// { dg-options -std=c++1y }
// { dg-do run }

#include <array>
#include <vector>
#include <type_traits>
#include <cassert>

struct Foo
{
  std::array<bool, 4> type;
  std::array<bool, 4> const_type;
  std::vector<int> chars;
};

template<typename CharT, CharT... str>
Foo
operator""_foo()
{
  CharT arr[]{str...};

  Foo foo;

  foo.type[0] = std::is_same<CharT, char>::value;
  foo.type[1] = std::is_same<CharT, wchar_t>::value;
  foo.type[2] = std::is_same<CharT, char16_t>::value;
  foo.type[3] = std::is_same<CharT, char32_t>::value;
  foo.const_type[0] = std::is_same<CharT, const char>::value;
  foo.const_type[1] = std::is_same<CharT, const wchar_t>::value;
  foo.const_type[2] = std::is_same<CharT, const char16_t>::value;
  foo.const_type[3] = std::is_same<CharT, const char32_t>::value;

  for(CharT c : arr)
    foo.chars.push_back((int)c);

  return foo;
}

int
main()
{
  Foo foo;

  foo = U"\x10000\x10001\x10002"_foo;
  assert (foo.type[3] == true);
  assert (foo.chars.size() == 3);
  assert (foo.chars[0] == 65536);
  assert (foo.chars[1] == 65537);
  assert (foo.chars[2] == 65538);

  foo = "\x61\x62\x63"_foo;
  assert (foo.type[0] == true);
  assert (foo.chars.size() == 3);
  assert (foo.chars[0] == 97);
  assert (foo.chars[1] == 98);
  assert (foo.chars[2] == 99);

  foo = L"\x01020304\x05060708"_foo;
  assert (foo.type[1] == true);
  assert (foo.chars.size() == 2);
  assert (foo.chars[0] == 16909060);
  assert (foo.chars[1] == 84281096);

  foo = u"\x0102\x0304\x0506\x0708"_foo;
  assert (foo.type[2] == true);
  assert (foo.chars.size() == 4);
  assert (foo.chars[0] == 258);
  assert (foo.chars[1] == 772);
  assert (foo.chars[2] == 1286);
  assert (foo.chars[3] == 1800);
}
