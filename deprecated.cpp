// /home/ed/bin/bin/g++ -std=c++1y -o deprecated deprecated.cpp

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
