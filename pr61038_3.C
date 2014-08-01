// PR c++/61038
// { dg-do compile { target c++11 } }

#include <cstring>
#include <cstdlib>

void
operator "" _s(const char *, unsigned long)
{ }

void
operator "" _t(const char)
{ }

#define QUOTE(s) #s

int
main()
{
  const char *s = QUOTE("hello"_s);
  const char *t = "\"hello\"_s";
  if (strcmp(s, t) != 0)
    abort();

  const char *c = QUOTE('"'_t);
  const char *d = "'\"'_t";
  if (strcmp(c, d) != 0)
    abort();

  const char *e = QUOTE('\''_t);
  const char *f = "'\\''_t";
  if (strcmp(e, f) != 0)
    abort();

  const char *g = QUOTE('\\'_t);
  const char *h = "'\\\\'_t";
  if (strcmp(g, h) != 0)
    abort();
}
