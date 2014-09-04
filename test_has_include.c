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
