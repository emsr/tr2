class SomeClass {};

#define ASSERT( cnd, ... ) SomeClass(),##__VA_ARGS__
#define FAIL( ... )        SomeClass(),##__VA_ARGS__

void
test()
{
  ASSERT( false );
  FAIL();
}

// /home/ed/bin_tr2/bin/g++ -c -pedantic pr44317.C
