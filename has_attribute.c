
// $HOME/bin/bin/gcc -o has_attribute_c has_attribute.c

#include <assert.h>

#ifndef __has_attribute
#  error "__has_attribute"
#endif

//  I gave this to C also since scoped enumerators are allowed for C as an extension.
#ifndef __has_cpp_attribute
#  error "__has_cpp_attribute"
#endif

#if !__has_attribute(noinline)
#  error "noinline"
#endif

#if __has_attribute(putrescent)
#  error "putrescent"
#endif

int
main()
{
  long
#ifdef __has_attribute
# if __has_attribute(peripatetic)
    __attribute__((peripatetic))
# endif
#endif
  life;
}
