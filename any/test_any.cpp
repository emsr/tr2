// /home/ed/bin/bin/g++ -g -std=c++11 -o test_any test_any.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_any

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_any > test.txt 2>&1

#include "any"

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>

template<typename Cont>
  void
  append_int(Cont & values, int value)
  { values.push_back(value); }

template<typename Cont>
  void
  append_string(Cont & values, const std::string & value)
  { values.push_back(value); }

template<typename Cont>
  void
  append_char_ptr(Cont & values, const char * value)
  { values.push_back(value); }

template<typename Cont>
  void
  append_any(Cont & values, const std::any & value)
  { values.push_back(value); }

template<typename Cont>
  void
  append_nothing(Cont & values)
  { values.push_back(std::any()); }


bool
is_empty(const std::any & operand)
{ return operand.empty(); }

bool
is_int(const std::any & operand)
{ return operand.type() == typeid(int); }

bool
is_char_ptr(const std::any & operand)
{ return operand.type() == typeid(const char *); }

bool
is_string(const std::any & operand)
{ return operand.type() == typeid(std::string); }

template<typename Cont>
  void
  count_all(const Cont & values, std::ostream & out)
  {
    out << "# empty == "
	<< std::count_if(values.begin(), values.end(), is_empty) << std::endl;
    out << "# int == "
	<< std::count_if(values.begin(), values.end(), is_int) << std::endl;
    out << "# const char * == "
	<< std::count_if(values.begin(), values.end(), is_char_ptr) << std::endl;
    out << "# string == "
	<< std::count_if(values.begin(), values.end(), is_string) << std::endl;
  }

int
main()
{
  std::any a1(1);
  //std::cout << "a1 == " << a1 << std::endl;
  //std::any a2("poop");

  std::list<std::any> values;
  append_int(values, 42);
  append_string(values, std::string("howdy"));
  append_char_ptr(values, "poop");
  append_any(values, std::any(5));
  append_any(values, std::any("wadd"));
  append_nothing(values);
  append_any(values, std::any());
  append_any(values, std::any(std::string("ules")));
  count_all(values, std::cout);

  //for (auto v : values)
  //  std::cout << *v << std::endl;
}
