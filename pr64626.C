// /home/ed/bin/bin/g++ -std=c++14 -o pr64626 pr64626.C

#define STR(s) #s
int
main()
{
  int i = 1'2;
  const char *s[3]
  {
    STR(1' '),
    STR(..),
    STR(%:%),
  };
}
#if 0
1' '
..
%:%
#endif
