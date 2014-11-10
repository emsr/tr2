// /home/ed/bin_feature/bin/gcc -c test_has_include.c

#if __has_include__(<complex.h>)
#else
#  error "<complex.h>"
#endif

//  Try nonexistent bracket header.
#if __has_include__(<stuff.h>)
#  error "<stuff.h>"
#else
#endif

//  Try header that exists but we can't read.
#if __has_include("hidden.h")
#  warning "found hidden.h"
#  include "hidden.h"
#else
#  error "hidden.h"
#endif
