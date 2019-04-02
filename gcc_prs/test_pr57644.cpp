struct Foo
{
  unsigned i: 32;
};

int
main()
{
  Foo f{};
  return f.i;
}

// $HOME/bin/bin/g++ -std=c++14 -o test_pr57644 test_pr57644.cpp
