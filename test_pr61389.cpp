// /home/ed/bin/bin/g++ -std=c++11 -pedantic -c test_pr61389.cpp

// /home/ed/bin/bin/g++ -pedantic -c test_pr61389.cpp

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61389

#define V(A, ...) __VA_ARGS__
void f() {
 V(1);
}

#define PLUS+
