// PR c++/61038
// { dg-do compile { target c++11 } }
// { dg-options "-E" }

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
  QUOTE("hello"_s);

  QUOTE('"'_t);
  QUOTE('\''_t);
  QUOTE('\\'_t);
}
