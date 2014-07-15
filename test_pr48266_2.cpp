typedef int bar [[deprecated("message")]];
typedef int foo [[deprecated]];

foo f1() { return 0; }
int f1(foo) { return 0; }

bar f2() { return 0; }
int f2(bar) { return 0; }

void f3(foo, bar) { }

// /home/ed/bin/bin/g++ -std=c++14 -c test_pr48266_2.cpp
