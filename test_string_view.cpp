
// /home/ed/bin/bin/g++ -std=c++14 -o test_string_view test_string_view.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_string_view 

// /home/ed/bin_literal/bin/g++ -std=c++14 -o test_string_view test_string_view.cpp

// LD_LIBRARY_PATH=/home/ed/bin_literal/lib64:$LD_LIBRARY_PATH ./test_string_view 

//#include "string_view"
#include <string>
#include <experimental/string_view>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>

namespace std
{
  //  Try a more generic quoted manipulator.
  template<typename _String, typename _CharT = typename _String::value_type>
    inline auto
    quoted(const _String& __string,
           _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
      return __detail::_Quoted_string<const _String&, _CharT>
                               (__string, __delim, __escape);
    }
}

int
main()
{
  std::cout << std::endl;
  std::cout << "sizeof(std::string) = " << sizeof(std::string) << std::endl;
  std::cout << "sizeof(std::experimental::string_view) = " << sizeof(std::experimental::string_view) << std::endl;
  std::cout << "std::experimental::string_view::npos   = " << std::experimental::string_view::npos << std::endl;

  std::experimental::string_view snull{nullptr};
  std::cout << std::endl;
  std::cout << "snull            = " << snull << std::endl;
  std::cout << "snull.empty()    = " << std::boolalpha << snull.empty() << std::endl;
  std::cout << "snull.size()     = " << snull.size() << std::endl;
  std::cout << "snull.length()   = " << snull.length() << std::endl;
  std::cout << "snull.max_size() = " << snull.max_size() << std::endl;

  std::experimental::string_view sview{"Hello, World!"};
  std::cout << std::endl;
  std::cout << "sview            = " << sview << std::endl;
  std::cout << "sview.empty()    = " << std::boolalpha << sview.empty() << std::endl;
  std::cout << "sview.size()     = " << sview.size() << std::endl;
  std::cout << "sview.length()   = " << sview.length() << std::endl;
  std::cout << "sview.front()    = " << sview.front() << std::endl;
  std::cout << "sview.back()     = " << sview.back() << std::endl;
  std::cout << "sview.max_size() = " << sview.max_size() << std::endl;

  snull.swap(sview);
  std::swap(snull, sview);
/*
  std::experimental::string_view bad{nullptr, 5};
  std::cout << std::endl;
  std::cout << "bad              = " << bad << std::endl;
  std::cout << "bad.empty()      = " << std::boolalpha << bad.empty() << std::endl;
  std::cout << "bad.size()       = " << bad.size() << std::endl;
  std::cout << "bad.length()     = " << bad.length() << std::endl;
*/
  unsigned long long ullptr[] = {1ULL, 2ULL, 3ULL, 0ULL};
  std::experimental::basic_string_view<unsigned long long> ullsv{ullptr};
  std::cout << std::endl;
  //std::cout << "ullsv            = " << ullsv << std::endl;
  std::cout << "ullsv.empty()    = " << std::boolalpha << ullsv.empty() << std::endl;
  std::cout << "ullsv.size()     = " << ullsv.size() << std::endl;
  std::cout << "ullsv.length()   = " << ullsv.length() << std::endl;
  std::cout << "ullsv.max_size() = " << ullsv.max_size() << std::endl;

  using char128_t = unsigned __int128;
  char128_t u128ptr[] = {1ULL, 2ULL, 3ULL, 4ULL, 5ULL, 0ULL};
  std::experimental::basic_string_view<char128_t> u128sv{u128ptr};
  std::cout << std::endl;
  //std::cout << "u128sv            = " << u128sv << std::endl;
  std::cout << "u128sv.empty()    = " << std::boolalpha << u128sv.empty() << std::endl;
  std::cout << "u128sv.size()     = " << u128sv.size() << std::endl;
  std::cout << "u128sv.length()   = " << u128sv.length() << std::endl;
  std::cout << "u128sv.max_size() = " << u128sv.max_size() << std::endl;

  std::cout << std::endl;
  std::cout << "std::quoted(sv) = " << std::quoted(sview) << std::endl;

  std::cout << std::endl;
  std::cout << "range for loop         : ";
  for (auto c : sview)
    std::cout << char(c);
  std::cout << std::endl;

  std::cout << "iterator loop          : ";
  for (auto c = sview.begin(); c != sview.end(); ++c)
    std::cout << char(*c);
  std::cout << std::endl;

  std::cout << "const iter loop        : ";
  for (auto c = sview.cbegin(); c != sview.cend(); ++c)
    std::cout << char(*c);
  std::cout << std::endl;

  std::cout << "reverse iter loop      : ";
  for (auto c = sview.rbegin(); c != sview.rend(); ++c)
    std::cout << char(*c);
  std::cout << std::endl;

  std::cout << "const reverse iter loop: ";
  for (auto c = sview.crbegin(); c != sview.crend(); ++c)
    std::cout << char(*c);
  std::cout << std::endl;

  //snull.swap(sview);

  std::cout << "snull            = " << snull << std::endl;
  std::cout << "snull.data()     = " << snull.data() << std::endl;
  std::cout << "snull.length()   = " << snull.length() << std::endl;
  std::cout << "snull.begin()    = " << snull.begin() << std::endl;
  std::cout << "snull.end()      = " << snull.end() << std::endl;
  std::cout << "sview            = " << sview << std::endl;
  std::cout << "sview.data()     = " << sview.data() << std::endl;
  std::cout << "sview.length()   = " << sview.length() << std::endl;
  std::cout << "sview.begin()    = " << sview.begin() << std::endl;
  std::cout << "sview.end()      = " << sview.end() << std::endl;

  std::cout << std::endl;
  try
  {
    std::cout << "sview.at(16)   = " << sview.at(16) << std::endl;
  }
  catch(std::out_of_range err)
  {
    std::cerr << err.what() << std::endl;
  }

  std::cout << std::endl;
  try
  {
    std::cout << "snull.at(16)  = " << snull.at(16) << std::endl;
  }
  catch(std::out_of_range err)
  {
    std::cerr << err.what() << std::endl;
  }

  std::cout << std::endl;
  std::cout << "operator[](5) = " << sview[5] << std::endl;

  std::experimental::string_view pre_prefix = sview;
  std::cout << std::endl;
  std::cout << "pre_prefix = " << pre_prefix << std::endl;

  sview.remove_prefix(3);
  std::cout << "remove_prefix(3) = " << sview << std::endl;
  std::cout << "length()         = " << sview.length() << std::endl;

  std::experimental::string_view pre_suffix = sview;
  std::cout << std::endl;
  std::cout << "pre_suffix = " << pre_suffix << std::endl;

  sview.remove_suffix(1);
  std::cout << "remove_suffix(1) = " << sview << std::endl;
  std::cout << "length()         = " << sview.length() << std::endl;

  std::cout << std::endl;
  std::cout << "pre_prefix = " << pre_prefix << std::endl;
  std::cout << "pre_suffix = " << pre_suffix << std::endl;

  std::cout << std::endl;
  std::cout << "substr(1, 5)     = " << sview.substr(1, 5) << std::endl;

  std::vector<int> iv;
  //std::cout << "iv.front()     = " << iv.front() << std::endl;
  //std::cout << "iv.back()      = " << iv.back() << std::endl;

  std::cout << std::endl;
  std::cout << "sview.empty()    = " << std::boolalpha << sview.empty() << std::endl;
  std::cout << "sview.size()     = " << sview.size() << std::endl;
  std::cout << "sview.length()   = " << sview.length() << std::endl;
  //std::cout << "sview.front()    = " << sview.front() << std::endl;
  //std::cout << "sview.back()     = " << sview.back() << std::endl;
  std::cout << "sview.max_size() = " << sview.max_size() << std::endl;

  std::experimental::string_view hw1{"Hello, World!"};
  std::cout << std::endl;
  std::cout << "hw1 = " << hw1 << std::endl;
  hw1.remove_suffix(3);
  std::cout << "hw1.remove_suffix(3) = " << hw1 << std::endl;

  std::experimental::string_view ca{"boogerrand"};
  std::experimental::string_view cb{"boogerbear"};
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

  using namespace std::experimental::literals::string_view_literals;

  auto uh = "Uranus-Hertz"sv;
  std::cout << std::endl;
  std::cout << "uh            = " << uh << std::endl;
  std::cout << "uh.empty()    = " << std::boolalpha << uh.empty() << std::endl;
  std::cout << "uh.size()     = " << uh.size() << std::endl;
  std::cout << "uh.length()   = " << uh.length() << std::endl;
  std::cout << "uh.front()    = " << uh.front() << std::endl;
  std::cout << "uh.back()     = " << uh.back() << std::endl;
  std::cout << "uh.max_size() = " << uh.max_size() << std::endl;

  auto uhs = uh.to_string();
  std::cout << std::endl;
  std::cout << "uhs             = " << uhs << std::endl;
  std::cout << "uhs.empty()     = " << std::boolalpha << uhs.empty() << std::endl;
  std::cout << "uhs.size()      = " << uhs.size() << std::endl;
  std::cout << "uhs.capacity()  = " << uhs.capacity() << std::endl;
  std::cout << "uhs.length()    = " << uhs.length() << std::endl;
  std::cout << "uhs.front()     = " << uhs.front() << std::endl;
  std::cout << "uhs.back()      = " << uhs.back() << std::endl;
  std::cout << "uhs.max_size()  = " << uhs.max_size() << std::endl;
  std::cout << "uhs.append(\"x\") = " << uhs.append("x") << std::endl;

  char arr[10]{};
  std::cout << std::endl;
  std::cout << "arr = ";
  for (auto c : arr)
    std::cout << c;
  uh.copy(arr, 10, 1);
  std::cout << std::endl;
  std::cout << "arr = ";
  for (auto c : arr)
    std::cout << c;
  std::cout << std::endl;

  //                                                 11111111112222222222333333333
  //                                       012345678901234567890123456879012345678
  std::experimental::string_view haystack{"Buy the commemorative boogerrand today!"};
  std::experimental::string_view mm{"mm"};
  std::cout << std::endl;
  std::cout << "haystack                    = " << haystack << std::endl;

  std::experimental::string_view csv{"booger"}, csu{"boogeq"}, csw{"booges"};
  std::cout << "csu                         = " << csu << std::endl;
  std::cout << "csv                         = " << csv << std::endl;
  std::cout << "csw                         = " << csw << std::endl;
  std::cout << "csv.compare(csu)            = " << csv.compare(csu) << std::endl;
  std::cout << "csv.compare(csv)            = " << csv.compare(csv) << std::endl;
  std::cout << "csv.compare(csw)            = " << csv.compare(csw) << std::endl;
  std::cout << "csv.compare(22,6,csv)       = " << haystack.compare(22,6,csv) << std::endl;
  std::cout << "csv.compare(22,6,csv,0,6)   = " << haystack.compare(22,6,csv,0,6) << std::endl;
  std::cout << "csv.compare(22,6,csv,0,7)   = " << haystack.compare(22,6,csv,0,7) << std::endl;
  std::cout << "csv.compare(22,7,csv,0,6)   = " << haystack.compare(22,7,csv,0,6) << std::endl;
  std::cout << "csv.compare(22,5,csv,0,6)   = " << haystack.compare(22,5,csv,0,6) << std::endl;
  std::cout << "csv.compare(\"boogeq\")       = " << csv.compare("boogeq") << std::endl;
  std::cout << "csv.compare(\"booger\")       = " << csv.compare("booger") << std::endl;
  std::cout << "csv.compare(\"booges\")       = " << csv.compare("booges") << std::endl;
  std::cout << "csv.compare(22,6,\"booger\")  = " << haystack.compare(22,6,"booger") << std::endl;
  std::cout << "csv.compare(22,6,\"booger\",6)= " << haystack.compare(22,6,"booger",6) << std::endl;
  std::cout << "csv.compare(22,6,\"booger\",5)= " << haystack.compare(22,6,"booger",5) << std::endl;
  std::cout << "csv.compare(22,6,\"boogers\",6)= " << haystack.compare(22,6,"boogers",6) << std::endl;
  std::cout << "csv.compare(22,6,\"boogers\",7)= " << haystack.compare(22,6,"boogers",7) << std::endl;

  std::cout << std::endl;
  std::cout << "haystack.find(mm)           = " << haystack.find(mm) << std::endl;
  std::cout << "haystack.find('m',2)        = " << haystack.find('m',2) << std::endl;
  std::cout << "haystack.find('m',12)       = " << haystack.find('m',12) << std::endl;
  std::cout << "haystack.find(\"mm\",0,2)     = " << haystack.find("mm",0,2) << std::endl;
  std::cout << "haystack.find(\"mm\",12,2)    = " << haystack.find("mm",12,2) << std::endl;
  std::cout << "haystack.find(\"mm\")         = " << haystack.find("mm") << std::endl;

  std::cout << std::endl;
  std::cout << "haystack.rfind(mm)           = " << haystack.rfind(mm) << std::endl;
  std::cout << "haystack.rfind('m',2)        = " << haystack.rfind('m',2) << std::endl;
  std::cout << "haystack.rfind(\"m\",2)        = " << haystack.rfind("m",2) << std::endl;
  std::cout << "haystack.rfind('m',12)       = " << haystack.rfind('m',12) << std::endl;
  std::cout << "haystack.rfind(\"m\",12)       = " << haystack.rfind("m",12) << std::endl;
  std::cout << "haystack.rfind(\"mm\",20,2)    = " << haystack.rfind("mm",20,2) << std::endl;
  std::cout << "haystack.rfind(\"mm\")         = " << haystack.rfind("mm") << std::endl;

  std::experimental::string_view myo{"myo"};

  std::cout << std::endl;
  std::cout << "haystack.find_first_of(mm)            = " << haystack.find_first_of(mm) << std::endl;
  std::cout << "haystack.find_first_of('m',2)         = " << haystack.find_first_of('m',2) << std::endl;
  std::cout << "haystack.find_first_of(\"m\",2)         = " << haystack.find_first_of("m",2) << std::endl;
  std::cout << "haystack.find_first_of('m',12)        = " << haystack.find_first_of('m',12) << std::endl;
  std::cout << "haystack.find_first_of(\"m\",12)        = " << haystack.find_first_of("m",12) << std::endl;
  std::cout << "haystack.find_first_of(\"mm\",20,2)     = " << haystack.find_first_of("mm",20,2) << std::endl;
  std::cout << "haystack.find_first_of(\"mm\")          = " << haystack.find_first_of("mm") << std::endl;
  std::cout << "haystack.find_first_of(myo)           = " << haystack.find_first_of(myo) << std::endl;
  std::cout << "haystack.find_first_of(\"myo\",2)       = " << haystack.find_first_of("myo",2) << std::endl;
  std::cout << "haystack.find_first_of(\"myo\",2,3)     = " << haystack.find_first_of("myo",2,3) << std::endl;
  std::cout << "haystack.find_first_of(\"myo\",20,3)    = " << haystack.find_first_of("myo",20,3) << std::endl;

  std::cout << std::endl;
  std::cout << "haystack.find_last_of(mm)             = " << haystack.find_last_of(mm) << std::endl;
  std::cout << "haystack.find_last_of('m',2)          = " << haystack.find_last_of('m',2) << std::endl;
  std::cout << "haystack.find_last_of(\"m\",2)          = " << haystack.find_last_of("m",2) << std::endl;
  std::cout << "haystack.find_last_of('m',12)         = " << haystack.find_last_of('m',12) << std::endl;
  std::cout << "haystack.find_last_of(\"m\",12)         = " << haystack.find_last_of("m",12) << std::endl;
  std::cout << "haystack.find_last_of(\"mm\",20,2)      = " << haystack.find_last_of("mm",20,2) << std::endl;
  std::cout << "haystack.find_last_of(\"mm\")           = " << haystack.find_last_of("mm") << std::endl;
  std::cout << "haystack.find_last_of(myo)            = " << haystack.find_last_of(myo) << std::endl;
  std::cout << "haystack.find_last_of(\"myo\",2)        = " << haystack.find_last_of("myo",2) << std::endl;
  std::cout << "haystack.find_last_of(\"myo\",2,3)      = " << haystack.find_last_of("myo",2,3) << std::endl;
  std::cout << "haystack.find_last_of(\"myo\",20,3)     = " << haystack.find_last_of("myo",20,3) << std::endl;
  std::cout << "haystack.find_last_of(\"myo\",20,1)     = " << haystack.find_last_of("myo",20,1) << std::endl;

  std::experimental::string_view myoer_{"myoer "};

  std::cout << std::endl;
  std::cout << "haystack.find_first_not_of(mm)         = " << haystack.find_first_not_of(mm) << std::endl;
  std::cout << "haystack.find_first_not_of('m',2)      = " << haystack.find_first_not_of('m',2) << std::endl;
  std::cout << "haystack.find_first_not_of(\"m\",2)      = " << haystack.find_first_not_of("m",2) << std::endl;
  std::cout << "haystack.find_first_not_of('m',12)     = " << haystack.find_first_not_of('m',12) << std::endl;
  std::cout << "haystack.find_first_not_of(\"m\",12)     = " << haystack.find_first_not_of("m",12) << std::endl;
  std::cout << "haystack.find_first_not_of(\"mm\",20,2)  = " << haystack.find_first_not_of("mm",20,2) << std::endl;
  std::cout << "haystack.find_first_not_of(\"mm\")       = " << haystack.find_first_not_of("mm") << std::endl;
  std::cout << "haystack.find_first_not_of(myo)        = " << haystack.find_first_not_of(myo) << std::endl;
  std::cout << "haystack.find_first_not_of(\"myo\",2)    = " << haystack.find_first_not_of("myo",2) << std::endl;
  std::cout << "haystack.find_first_not_of(\"myo\",2,3)  = " << haystack.find_first_not_of("myo",2,3) << std::endl;
  std::cout << "haystack.find_first_not_of(\"myo\",20,3) = " << haystack.find_first_not_of("myo",20,3) << std::endl;
  std::cout << "haystack.find_first_not_of(myoer_)       = " << haystack.find_first_not_of(myoer_) << std::endl;
  std::cout << "haystack.find_first_not_of(\"myoer \",2)   = " << haystack.find_first_not_of("myoer ",2) << std::endl;
  std::cout << "haystack.find_first_not_of(\"myoer \",2,6) = " << haystack.find_first_not_of("myoer ",2,6) << std::endl;
  std::cout << "haystack.find_first_not_of(\"myoer \",20,6)= " << haystack.find_first_not_of("myoer ",20,6) << std::endl;
  std::cout << "haystack.find_first_not_of(\"myoer \",20,4)= " << haystack.find_first_not_of("myoer ",20,4) << std::endl;

  std::cout << std::endl;
  std::cout << "haystack.find_last_not_of(mm)         = " << haystack.find_last_not_of(mm) << std::endl;
  std::cout << "haystack.find_last_not_of('m',2)      = " << haystack.find_last_not_of('m',2) << std::endl;
  std::cout << "haystack.find_last_not_of(\"m\",2)      = " << haystack.find_last_not_of("m",2) << std::endl;
  std::cout << "haystack.find_last_not_of('m',12)     = " << haystack.find_last_not_of('m',12) << std::endl;
  std::cout << "haystack.find_last_not_of(\"m\",12)     = " << haystack.find_last_not_of("m",12) << std::endl;
  std::cout << "haystack.find_last_not_of(\"mm\",20,2)  = " << haystack.find_last_not_of("mm",20,2) << std::endl;
  std::cout << "haystack.find_last_not_of(\"mm\")       = " << haystack.find_last_not_of("mm") << std::endl;
  std::cout << "haystack.find_last_not_of(myo)        = " << haystack.find_last_not_of(myo) << std::endl;
  std::cout << "haystack.find_last_not_of(\"myo\",2)    = " << haystack.find_last_not_of("myo",2) << std::endl;
  std::cout << "haystack.find_last_not_of(\"myo\",2,3)  = " << haystack.find_last_not_of("myo",2,3) << std::endl;
  std::cout << "haystack.find_last_not_of(\"myo\",20,3) = " << haystack.find_last_not_of("myo",20,3) << std::endl;
  std::cout << "haystack.find_last_not_of(\"myo\",20,1) = " << haystack.find_last_not_of("myo",20,1) << std::endl;
  std::cout << "haystack.find_last_not_of(myoer_)        = " << haystack.find_last_not_of(myoer_) << std::endl;
  std::cout << "haystack.find_last_not_of(\"myoer \",2)    = " << haystack.find_last_not_of("myoer ",2) << std::endl;
  std::cout << "haystack.find_last_not_of(\"myoer \",2,6)  = " << haystack.find_last_not_of("myoer ",2,6) << std::endl;
  std::cout << "haystack.find_last_not_of(\"myoer \",20,6) = " << haystack.find_last_not_of("myoer ",20,6) << std::endl;
  std::cout << "haystack.find_last_not_of(\"myoer \",20,4) = " << haystack.find_last_not_of("myoer ",20,4) << std::endl;

  std::string haystring{"Buy the commemorative boogerrand today!"};
  std::cout << std::endl;
  std::cout << "haystring.rfind('m',2)    = " << haystring.rfind('m',2) << std::endl;
  std::cout << "haystring.rfind(\"m\",2)    = " << haystring.rfind("m",2) << std::endl;
  std::cout << "haystring.rfind('m',12)    = " << haystring.rfind('m',12) << std::endl;
  std::cout << "haystring.rfind(\"m\",12)    = " << haystring.rfind("m",12) << std::endl;

  std::cout << std::endl;
  std::cout << "Literally!"sv << std::endl;
  std::wcout << L"Literally!"sv << std::endl;
}
