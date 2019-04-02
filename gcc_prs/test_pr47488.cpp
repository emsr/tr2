//  $HOME/bin/bin/g++ -std=c++11 -o test_pr47488 test_pr47488.cpp

template<typename T>
  int
  f(const T &, const char *);

template<typename T>
  decltype(f(T(), ""))
  g(const T &t);

void
h()
{
  g( 0 );
}
