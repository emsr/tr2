// /home/ed/bin/bin/g++ -std=c++1y -o test_aggregate_nsdmi test_aggregate_nsdmi.cpp 

struct S
{
  int a;
  const char* b;
  int c;
  int d = b[a];
};

S ss = { 1, "asdf" };
assert(a == 1);
//assert(b == "asdf");
assert(c == 0);
assert(d == 's');

struct X
{
  int i, j, k = 42;
};

X a[] = { 1, 2, 3, 4, 5, 6 };
X b[2] = { { 1, 2, 3 }, { 4, 5, 6 } };
