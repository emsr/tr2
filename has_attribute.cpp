
// /home/ed/bin/bin/g++ -std=c++14 -o has_attribute has_attribute.cpp

#include <cassert>

int
main()
{
  bool ok = __has_attribute(deprecated);
  assert(ok);
  bool notok = __has_attribute(urectum);
  assert(!notok);

  bool [[fartknocker]] wtf;
}
