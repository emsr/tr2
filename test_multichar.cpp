
// $HOME/bin/bin/g++ -std=c++14 -o test_multichar test_multichar.cpp

int
operator""_foo(int ch)
{ return ch; }

int
operator""_foo(char ch)
{ return ch; }

int
main()
{
  auto x = 'abcd'_foo;
}
