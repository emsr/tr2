
// $HOME/bin/bin/g++ -std=c++1y -o mem_init mem_init.cpp

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3653.html

int
main()
{
  // Initialize ss.a with 1,
  //            ss.b with "asdf",
  //            ss.c with the value of an expression of the form int{} (that is, 0),
  //        and ss.d with the value of ss.b[ss.a] (that is, 's')
  struct S
  {
    int a;
    const char* b;
    int c;
    int d = b[a];
  };
  S ss = { 1, "asdf" };

  // a and b have the same value.
  struct X
  {
    int i, j, k = 42;
  };
  X a[] = { 1, 2, 3, 4, 5, 6 };
  X b[2] = { { 1, 2, 3 }, { 4, 5, 6 } };
}
