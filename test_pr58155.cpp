// $HOME/bin/bin/g++ -std=c++11 -o test_pr58155 test_pr58155.cpp

#define BAZ "baz"

#if 0
"bar"BAZ

R"(
  bar
)"BAZ
#endif

int
main()
{ }
