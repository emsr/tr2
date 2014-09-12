
// /home/ed/bin/bin/g++ -std=c++14 -o has_attribute has_attribute.cpp

#include <cassert>

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

  bool cpp_ok = __has_cpp_attribute(deprecated);
  assert(cpp_ok);

  bool cpp_gnuok = __has_cpp_attribute(gnu::deprecated);
  assert(cpp_gnuok);

  bool cpp_oldok = __has_cpp_attribute(__deprecated__);
  assert(cpp_oldok);

  bool cpp_notok = __has_cpp_attribute(urectum);
  assert(!cpp_notok);

  bool
#ifdef __has_cpp_attribute
//# if __has_cpp_attribute(peripatetic)
# if __has_include("peripatetic")
    [[peripatetic]]
# endif
#endif
  life;
}
