
// /home/ed/bin/bin/g++ -std=c++14 -o has_attribute has_attribute.cpp

#include <cassert>

#define __has_cpp_attribute(THING) __has_attribute(THING)

int
main()
{
  bool ok = __has_attribute(deprecated);
  assert(ok);

  bool gnuok = __has_attribute(gnu::deprecated);
  assert(gnuok);

  bool oldok = __has_attribute(__deprecated__);
  assert(oldok);

  bool notok = __has_attribute(urectum);
  assert(!notok);

#ifdef __has_cpp_attribute
# if __has_cpp_attribute(fartknocker)
  bool [[fartknocker]] wtf;
# endif
#endif
}
