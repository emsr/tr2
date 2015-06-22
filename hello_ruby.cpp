// $HOME/bin/bin/g++ -std=c++14 -o hello_ruby hello_ruby.cpp

#include <string>

int
main()
{
  using namespace std::literals::string_literals;
  auto len = "Hello, World!!!"s.length();
}

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61057
// This is about numbers though.
