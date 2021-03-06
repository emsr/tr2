/*
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=23827

$HOME/bin/bin/gcc -std=c89 -pedantic-errors -o test_pr23827 test_pr23827.c

$HOME/bin/bin/gcc -std=c11 -pedantic-errors -o test_pr23827 test_pr23827.c

./test_pr23827

*/

#define f (
#define l )
#define str(x) #x
#define xstr(x) str(x)

/* C90 and C++98: "0x1p+( 0x1p+)"; C99 and C++11: "0x1p+f 0x1p+l" */
const char *s = xstr(0x1p+f 0x1p+l);

#ifdef __cplusplus
extern "C" {
#endif

void abort ();
int strcmp (const char *, const char *);

#ifdef __cplusplus
}
#endif

int
main ()
{
  extern const char *s;
  if (strcmp (s, "0x1p+( 0x1p+)"))
    abort ();
  else
    return 0; /* Correct C90 and C++98 behavior.  */
}
