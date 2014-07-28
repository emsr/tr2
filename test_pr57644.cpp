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

// /home/ed/bin/bin/g++ -std=c++14 -o test_pr57644 test_pr57644.cpp
