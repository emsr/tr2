// /home/ed/bin_feature/bin/g++ -std=c++1y -c test_has_include.cpp

// /home/ed/bin/bin/g++ -std=c++1y -c test_has_include.cpp

#if __has_include__(<complex>)
#else
#  error "<complex>"
#endif

//  Try nonexistent bracket header.
#if __has_include__(<stuff>)
#  error "<stuff>"
#else
#endif

//  Try quote header.
#if __has_include__("complex_literals.h")
#  include "complex_literals.h"
#else
#  include <complex>
#endif
