
// /home/ed/bin/bin/gcc -o has_attribute has_attribute.c

#include <assert.h>

int
main()
{
  long ok = __has_attribute__(deprecated);
  assert(ok);

  long gnuok = __has_attribute__(gnu::deprecated);
  assert(gnuok);

  long oldok = __has_attribute__(__deprecated__);
  assert(oldok);

  long notok = __has_attribute__(urectum);
  assert(!notok);

#ifdef __has_attribute
# if __has_attribute(fartknocker)
  long [[fartknocker]] wtf;
# endif
#endif
}
