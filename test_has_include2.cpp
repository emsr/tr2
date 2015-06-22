// $HOME/bin_feature/bin/g++ -std=c++14 -c -o test_has_include2 test_has_include2.cpp

//  Try header that exists but we can't read.
#if __has_include("hidden.h")
#  warning "found hidden.h"
#  include "hidden.h"
#else
#  error "hidden.h"
#endif

#if 1
#  include "hidden.h"
#endif
