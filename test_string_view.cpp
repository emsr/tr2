
// /home/ed/bin/bin/g++ -std=c++1y -o test_string_view test_string_view.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_string_view 

#include "string_view"
#include <iostream>
#include <stdexcept>

int
main()
{
  std::cout << std::endl;
  std::cout << "std::string_view::npos = " << std::string_view::npos << std::endl;

  std::string_view sv{"Hello, World!"};
  std::cout << std::endl;
  std::cout << "sv            = " << sv << std::endl;
  std::cout << "sv.empty()    = " << std::boolalpha << sv.empty() << std::endl;
  std::cout << "sv.size()     = " << sv.size() << std::endl;
  std::cout << "sv.length()   = " << sv.length() << std::endl;
  std::cout << "sv.front()    = " << sv.front() << std::endl;
  std::cout << "sv.back()     = " << sv.back() << std::endl;
  std::cout << "sv.max_size() = " << sv.max_size() << std::endl;

  std::cout << std::endl;
  std::cout << "range for loop         : ";
  for (auto c : sv)
    std::cout << char(c);
  std::cout << std::endl;

  std::cout << "iterator loop          : ";
  for (auto c = sv.begin(); c != sv.end(); ++c)
    std::cout << char(*c);
  std::cout << std::endl;

  std::cout << "const iter loop        : ";
  for (auto c = sv.cbegin(); c != sv.cend(); ++c)
    std::cout << char(*c);
  std::cout << std::endl;

  std::cout << "reverse iter loop      : ";
  for (auto c = sv.rbegin(); c != sv.rend(); ++c)
    std::cout << char(*c);
  std::cout << std::endl;

  std::cout << "const reverse iter loop: ";
  for (auto c = sv.crbegin(); c != sv.crend(); ++c)
    std::cout << char(*c);
  std::cout << std::endl;

  std::cout << std::endl;
  try
  {
    std::cout << "at(16)        = " << sv.at(16) << std::endl;
  }
  catch(std::out_of_range err)
  {
    std::cerr << err.what() << std::endl;
  }

  std::cout << std::endl;
  std::cout << "operator[](5) = " << sv[5] << std::endl;

  std::string_view pre_prefix = sv;
  std::cout << std::endl;
  std::cout << "pre_prefix = " << pre_prefix << std::endl;

  sv.remove_prefix(3);
  std::cout << "remove_prefix(3) = " << sv << std::endl;
  std::cout << "length()         = " << sv.length() << std::endl;

  std::string_view pre_suffix = sv;
  std::cout << std::endl;
  std::cout << "pre_suffix = " << pre_suffix << std::endl;

  sv.remove_suffix(1);
  std::cout << "remove_suffix(1) = " << sv << std::endl;
  std::cout << "length()         = " << sv.length() << std::endl;

  std::cout << std::endl;
  std::cout << "pre_prefix = " << pre_prefix << std::endl;
  std::cout << "pre_suffix = " << pre_suffix << std::endl;

  std::cout << std::endl;
  std::cout << "substr(1, 5)     = " << sv.substr(1, 5) << std::endl;

  sv.clear();
  std::cout << std::endl;
  std::cout << "clear()       = " << sv << std::endl;
  std::cout << "sv.empty()    = " << std::boolalpha << sv.empty() << std::endl;
  std::cout << "sv.size()     = " << sv.size() << std::endl;
  std::cout << "sv.length()   = " << sv.length() << std::endl;
  std::cout << "sv.front()    = " << sv.front() << std::endl;
  std::cout << "sv.back()     = " << sv.back() << std::endl;
  std::cout << "sv.max_size() = " << sv.max_size() << std::endl;

  std::string_view hw1{"Hello, World!"};
  std::cout << std::endl;
  std::cout << "hw1 = " << hw1 << std::endl;
  hw1.remove_suffix(3);
  std::cout << "hw1.remove_suffix(3) = " << hw1 << std::endl;

  std::string_view ca{"boogerand"};
  std::string_view cb{"boogerbear"};
  std::cout << std::endl;
  std::cout << "ca             = " << ca << std::endl;
  std::cout << "cb             = " << cb << std::endl;
  std::cout << "ca.compare(cb) = " << ca.compare(cb) << std::endl;
  std::cout << "ca == cb: " << std::boolalpha << bool{ca == cb} << std::endl;
  std::cout << "ca != cb: " << std::boolalpha << bool{ca != cb} << std::endl;
  std::cout << "ca < cb : " << std::boolalpha << bool{ca < cb} << std::endl;
  std::cout << "ca >= cb: " << std::boolalpha << bool{ca >= cb} << std::endl;
  std::cout << "ca <= cb: " << std::boolalpha << bool{ca <= cb} << std::endl;
  std::cout << "ca > cb : " << std::boolalpha << bool{ca > cb} << std::endl;

  std::cout << std::endl;
  std::cout << "ca == \"boo\": " << std::boolalpha << bool{ca == "boo"} << std::endl;
  std::cout << "ca != \"boo\": " << std::boolalpha << bool{ca != "boo"} << std::endl;
  std::cout << "ca < \"boo\" : " << std::boolalpha << bool{ca < "boo"} << std::endl;
  std::cout << "ca >= \"boo\": " << std::boolalpha << bool{ca >= "boo"} << std::endl;
  std::cout << "ca <= \"boo\": " << std::boolalpha << bool{ca <= "boo"} << std::endl;
  std::cout << "ca > \"boo\" : " << std::boolalpha << bool{ca > "boo"} << std::endl;

  std::cout << std::endl;
  std::cout << "\"boo\" == cb: " << std::boolalpha << bool{"boo" == cb} << std::endl;
  std::cout << "\"boo\" != cb: " << std::boolalpha << bool{"boo" != cb} << std::endl;
  std::cout << "\"boo\" <  cb: " << std::boolalpha << bool{"boo" < cb} << std::endl;
  std::cout << "\"boo\" >= cb: " << std::boolalpha << bool{"boo" >= cb} << std::endl;
  std::cout << "\"boo\" <= cb: " << std::boolalpha << bool{"boo" <= cb} << std::endl;
  std::cout << "\"boo\" >  cb: " << std::boolalpha << bool{"boo" > cb} << std::endl;

  // I added literals.

  using namespace std::literals::string_view_literals;

  auto uh = "Uranus-Hertz"sv;
  std::cout << std::endl;
  std::cout << "uh            = " << uh << std::endl;
  std::cout << "uh.empty()    = " << std::boolalpha << uh.empty() << std::endl;
  std::cout << "uh.size()     = " << uh.size() << std::endl;
  std::cout << "uh.length()   = " << uh.length() << std::endl;
  std::cout << "uh.front()    = " << uh.front() << std::endl;
  std::cout << "uh.back()     = " << uh.back() << std::endl;
  std::cout << "uh.max_size() = " << uh.max_size() << std::endl;

  auto uhs = std::to_string(uh);
  std::cout << std::endl;
  std::cout << "uhs            = " << uhs << std::endl;
  std::cout << "uhs.empty()    = " << std::boolalpha << uhs.empty() << std::endl;
  std::cout << "uhs.size()     = " << uhs.size() << std::endl;
  std::cout << "uhs.capacity() = " << uhs.capacity() << std::endl;
  std::cout << "uhs.length()   = " << uhs.length() << std::endl;
  std::cout << "uhs.front()    = " << uhs.front() << std::endl;
  std::cout << "uhs.back()     = " << uhs.back() << std::endl;
  std::cout << "uhs.max_size() = " << uhs.max_size() << std::endl;
  std::cout << "uhs.append(\"x\")= " << uhs.append("x") << std::endl;

  char arr[10]{};
  uh.copy(arr, 10, 1);
  std::cout << std::endl;
  for (auto c : arr)
    std::cout << c;
  std::cout << std::endl;

  std::string_view haystack{"Buy the commemorative boogerand today!"};
  std::string_view mm{"mm"};
  std::cout << std::endl;
  std::cout << "haystack                = " << haystack << std::endl;
  std::cout << "haystack.find(mm)       = " << haystack.find(mm) << std::endl;
  std::cout << "haystack.find('m',2)    = " << haystack.find('m') << std::endl;
  std::cout << "haystack.find(\"mm\",0,2) = " << haystack.find("mm",0,2) << std::endl;
  std::cout << "haystack.find(\"mm\")     = " << haystack.find("mm") << std::endl;
  std::cout << "haystack.rfind(mm)       = " << haystack.rfind(mm) << std::endl;
  std::cout << "haystack.rfind('m',2)    = " << haystack.rfind('m') << std::endl;
  std::cout << "haystack.rfind(\"mm\",20,2) = " << haystack.rfind("mm",20,2) << std::endl;
  std::cout << "haystack.rfind(\"mm\")     = " << haystack.rfind("mm") << std::endl;

  std::string haystring{"Buy the commemorative boogerand today!"};
  std::cout << std::endl;
  std::cout << "haystring.rfind('m',2)    = " << haystring.rfind('m') << std::endl;
}
