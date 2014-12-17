
// /home/ed/bin/bin/g++ -g -std=c++14 -DCHECK=true -o test_sized_dealloc test_sized_dealloc.cpp

// /home/ed/bin/bin/g++ -g -std=c++14 -DCHECK=false -fno-sized-deallocation -o test_sized_dealloc test_sized_dealloc.cpp

// /home/ed/bin/bin/g++ -g -std=c++11 -DCHECK=false -o test_sized_dealloc test_sized_dealloc.cpp

// /home/ed/bin/bin/g++ -g -std=c++11 -DCHECK=true -fsized-deallocation -o test_sized_dealloc test_sized_dealloc.cpp

extern "C" void abort();
typedef __SIZE_TYPE__ size_t;
#include <new>

bool called = false;

void
operator delete[] (void *p, size_t s) throw()
{
  called = true;
  operator delete[] (p);
}

void
operator delete (void *p, size_t s) throw()
{
  called = true;
  operator delete (p);
}

void
operator delete[] (void *p, size_t s, const std::nothrow_t &) throw()
{
  called = true;
  operator delete[] (p);
}

void
operator delete (void *p, size_t s, const std::nothrow_t &) throw()
{
  called = true;
  operator delete (p);
}

struct A { ~A(){} };

struct B { };

struct C;

struct D { ~D(){}; D() { throw 1; } };

int
main()
{
  bool check = CHECK;

  delete new int;
  if (called != check)
    abort();
  called = false;

  delete new A;
  if (called != check)
    abort();
  called = false;

  delete[] new A[2];
  if (called != check)
    abort();
  called = false;

  delete new B;
  if (called != check)
    abort();
  called = false;

  /* N3778 added the sized placement deallocation functions, but the core
     language rules don't provide any way they would be called.  */
  try { new (std::nothrow) D; } catch (int) {}
  if (called) abort();

  try { new (std::nothrow) D[2]; } catch (int) {}
  if (called) abort();

  /* Make sure we don't try to use the size of an array that doesn't have a
     cookie.  */
  delete[] new B[2];
  if (called) abort();
}
