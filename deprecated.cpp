// /home/ed/bin/bin/g++ -std=c++1y -o deprecated_cxx1y deprecated.cpp

// /home/ed/bin/bin/g++ -std=c++11 -o deprecated_cxx11 deprecated.cpp

class [[gnu::deprecated]] gA
{
};

[[gnu::deprecated]]
int
gfoo(int n)
{
  return 42 + n;
}

class [[deprecated]] A
{
};

[[deprecated]]
int
foo(int n)
{
  return 42 + n;
}

class [[deprecated("A has been superceded by B")]] B
{
};

[[deprecated("bar is unsafe; use poobar instead")]]
int
bar(int n)
{
  return 42 + n;
}

#if __cplusplus > 201103L

//  Deprecate C for C++14 onwards.
class [[deprecated]] C;

//  Deprecate foobar for C++14 onwards.
[[deprecated]]
int
foobar(int n);

#endif

class C
{
};

int
foobar(int n)
{
  return 43 + n - 1;
}

int
main()
{
  gA gaaa;
  int gn = gfoo(12);

  A aaa;
  int n = foo(12);

  B bbb;
  int m = bar(666);
}
