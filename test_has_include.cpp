// /home/ed/bin_feature/bin/g++ -std=c++14 -c test_has_include.cpp

// /home/ed/bin/bin/g++ -std=c++14 -c test_has_include.cpp

#if __has_include__(<complex>)
#else
#  error "<complex>"
#endif

//  Try nonexistent bracket header.
#if __has_include__(<stuff>)
#  error "<stuff>"
#else
#endif

#if __has_include__("complex.h")
#else
#  error "complex.h"
#endif

//  Try nonexistent quote header.
#if __has_include__("stuff.h")
#  error "stuff.h"
#else
#endif

//  Try quote header.
#if __has_include__("complex_literals.h")
#  include "complex_literals.h"
#else
#  include <complex>
#endif

//#include "hidden.h"

//  Try header that exists but we can't read.
#if __has_include("hidden.h")
#  warning "found hidden.h"
#  include "hidden.h"
#else
#  error "hidden.h"
#endif
