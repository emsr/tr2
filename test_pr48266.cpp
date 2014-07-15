typedef int bar __attribute__((deprecated("message")));
typedef int foo __attribute__((deprecated));

foo f1() { return 0; }
int f1(foo) { return 0; }

bar f2() { return 0; }
int f2(bar) { return 0; }

void f3(foo, bar) { }

// /home/ed/bin/bin/g++ -c test_pr48266.cpp
