// /home/ed/bin/bin/g++ -std=c++14 -o test_pr61663 test_pr61663.cpp

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61663

// http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#976

struct F 
{
  template<class T>
    operator const T&() { static T t; return t; }
};

int
main() 
{
  F f;
  int i = f;   // error: cannot convert 'F' to 'int' in initialization
}
