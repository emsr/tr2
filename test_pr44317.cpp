class SomeClass {};
#define ASSERT( cnd, ... ) SomeClass(),##__VA_ARGS__
#define FAIL( ... )        SomeClass(),##__VA_ARGS__
#define TEST(id ...)       SomeClass(),##__VA_ARGS__

void test()
{
  ASSERT( false );
  FAIL(); // { dg-error "expected primary-expression before" }
}

// $HOME/bin/bin/g++ -std=c++98 -c test_pr44317.cpp

// $HOME/bin/bin/g++ -std=c++98 -Wpedantic -c test_pr44317.cpp

// $HOME/bin/bin/g++ -std=c++11 -c test_pr44317.cpp

// $HOME/bin/bin/g++ -std=c++11 -Wpedantic -c test_pr44317.cpp

// $HOME/bin/bin/g++ -std=gnu++11 -c test_pr44317.cpp

// $HOME/bin/bin/g++ -std=gnu++11 -Wpedantic -c test_pr44317.cpp
