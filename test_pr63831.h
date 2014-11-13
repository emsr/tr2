#ifndef __has_attribute
#    define __has_attribute(x) 0
#endif

#if  __has_attribute(alloc_size)
#   define U_ALLOC_SIZE_ATTR(X) __attribute__ ((alloc_size(X)))
#endif
