// /home/ed/bin/bin/g++ -std=c++11 -c test_pr44317.cpp

// /home/ed/bin/bin/g++ -std=gnu++11 -c test_pr44317.cpp

class SomeClass {};

#define ASSERT( cnd, ... ) SomeClass(),##__VA_ARGS__
#define FAIL( ... )        SomeClass(),##__VA_ARGS__

void
test()
{
  ASSERT( false );
  FAIL(); // { dg-error "expected primary-expression before" }
}
