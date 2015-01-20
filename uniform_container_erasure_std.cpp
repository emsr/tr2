
// /home/ed/bin/bin/g++ -std=c++11 -o uniform_container_erasure_std uniform_container_erasure_std.cpp

// /home/ed/bin/bin/g++ -std=c++14 -o uniform_container_erasure_std uniform_container_erasure_std.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./uniform_container_erasure_std > ./uniform_container_erasure_std.out

#include <algorithm>
#include <experimental/deque>
#include <experimental/forward_list>
#include <experimental/list>
#include <experimental/map>
#include <experimental/set>
#include <experimental/string>
#include <experimental/unordered_map>
#include <experimental/unordered_set>
#include <experimental/vector>


#include <iostream>
#include <utility>
using namespace std;
using namespace std::experimental;

template<typename _Tp>
  void
  print_elem(const _Tp& __el)
  {
    cout << __el;
  }

template<typename _A, typename _B>
  void
  print_elem(const pair<_A, _B>& __p)
  {
    cout << __p.first << "/" << __p.second;
  }

template<typename _Cont>
  void
  print(const string& __str, const _Cont& __cont)
  {
    cout << __str << ": [";

    for (auto __first = __cont.begin(), __last = __cont.end(),
	 __iter = __first; __iter != __last; ++__iter)
    {
      if (__iter != __first)
	cout << " ";
      print_elem(*__iter);
    }

    cout << "]" << endl;
  }

int
main()
{
  auto is_vowel = [](const char c)
  {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
  };

  auto is_odd = [](const int i) { return i % 2 != 0; };

  auto is_odd_pair = [](const pair<const int, string>& p)
  {
    return p.first % 2 != 0;
  };

  string str("cute fluffy kittens");
  cout << "string before: " << str << endl;
  erase_if(str, is_vowel);
  cout << "string  after: " << str << endl;

  deque<int> d = { 10, 11, 12, 14, 15, 17, 18, 19 };
  print("deque before", d);
  erase_if(d, is_odd);
  print("deque  after", d);

  forward_list<int> fl = { 10, 11, 12, 14, 15, 17, 18, 19 };
  print("forward_list before", fl);
  erase_if(fl, is_odd);
  print("forward_list  after", fl);

  list<int> l = { 10, 11, 12, 14, 15, 17, 18, 19 };
  print("list before", l);
  erase_if(l, is_odd);
  print("list  after", l);

  vector<int> v = { 10, 11, 12, 14, 15, 17, 18, 19 };
  print("vector before", v);
  erase_if(v, is_odd);
  print("vector  after", v);

  map<int, string> m = { { 10, "A" }, { 11, "B" }, { 12, "C" },
      { 14, "D" }, { 15, "E" }, { 17, "F" }, { 18, "G" }, { 19, "H" } };
  print("map before", m);
  erase_if(m, is_odd_pair);
  print("map  after", m);

  multimap<int, string> mm = { { 20, "S" }, { 21, "T" }, { 22, "U" },
      { 22, "V" }, { 23, "W" }, { 23, "X" }, { 24, "Y" }, { 25, "Z" } };
  print("multimap before", mm);
  erase_if(mm, is_odd_pair);
  print("multimap  after", mm);

  set<int> s = { 10, 11, 12, 14, 15, 17, 18, 19 };
  print("set before", s);
  erase_if(s, is_odd);
  print("set  after", s);

  multiset<int> ms = { 20, 21, 22, 22, 23, 23, 24, 25 };
  print("multiset before", ms);
  erase_if(ms, is_odd);
  print("multiset  after", ms);

  unordered_map<int, string> um = { { 10, "A" }, { 11, "B" }, { 12, "C" },
      { 14, "D" }, { 15, "E" }, { 17, "F" }, { 18, "G" }, { 19, "H" } };
  print("unordered_map before", um);
  erase_if(um, is_odd_pair);
  print("unordered_map  after", um);

  unordered_multimap<int, string> umm = { { 20, "S" }, { 21, "T" },
      { 22, "U" }, { 22, "V" }, { 23, "W" }, { 23, "X" }, { 24, "Y" },
      { 25, "Z" } };
  print("unordered_multimap before", umm);
  erase_if(umm, is_odd_pair);
  print("unordered_multimap  after", umm);

  unordered_set<int> us = { 10, 11, 12, 14, 15, 17, 18, 19 };
  print("unordered_set before", us);
  erase_if(us, is_odd);
  print("unordered_set  after", us);

  unordered_multiset<int> ums = { 20, 21, 22, 22, 23, 23, 24, 25 };
  print("unordered_multiset before", ums);
  erase_if(ums, is_odd);
  print("unordered_multiset  after", ums);

  v = { 17, 29, 1729 };
  print("vector before erasing everything", v);
  erase_if(v, is_odd);
  print("vector  after erasing everything", v);
  v = { 256, 512, 1024 };
  print("vector before erasing nothing", v);
  erase_if(v, is_odd);
  print("vector  after erasing nothing", v);
  v.clear();
  print("empty vector before", v);
  erase_if(v, is_odd);
  print("empty vector  after", v);

  l = { 17, 29, 1729 };
  print("list before erasing everything", l);
  erase_if(l, is_odd);
  print("list  after erasing everything", l);
  l = { 256, 512, 1024 };
  print("list before erasing nothing", l);
  erase_if(l, is_odd);
  print("list  after erasing nothing", l);
  l.clear();
  print("empty list before", l);
  erase_if(l, is_odd);
  print("empty list  after", l);

  s = { 17, 29, 1729 };
  print("set before erasing everything", s);
  erase_if(s, is_odd);
  print("set  after erasing everything", s);
  s = { 256, 512, 1024 };
  print("set before erasing nothing", s);
  erase_if(s, is_odd);
  print("set  after erasing nothing", s);
  s.clear();
  print("empty set before", s);
  erase_if(s, is_odd);
  print("empty set  after", s);

  cout << boolalpha;
  auto is_true = [](const bool b) { return b; };
  vector<bool> vb = { false, true, false, false, true, true, false, true };
  print("vector<bool> before", vb);
  erase_if(vb, is_true);
  print("vector<bool>  after", vb);

  str = "cute fluffy kittens";
  cout << "string before erasing 'f': " << str << endl;
  erase(str, 'f');
  cout << "string  after erasing 'f': " << str << endl;
  erase(str, 'z');
  cout << "string  after erasing 'z': " << str << endl;

  d = { 10, 11, 12, 14, 15, 17, 18, 19 };
  print("deque before erasing 14", d);
  erase(d, 14);
  print("deque  after erasing 14", d);
  erase(d, 20);
  print("deque  after erasing 20", d);

  fl = { 10, 11, 12, 14, 15, 17, 18, 19 };
  print("forward_list before erasing 14", fl);
  erase(fl, 14);
  print("forward_list  after erasing 14", fl);
  erase(fl, 20);
  print("forward_list  after erasing 20", fl);

  l = { 0, 11, 0, 0, 22, 33, 0, 0, 44, 0 };
  print("list before erasing 0", l);
  erase(l, 0);
  print("list  after erasing 0", l);
  erase(l, 55);
  print("list  after erasing 55", l);

  v = { 0, 11, 0, 0, 22, 33, 0, 0, 44, 0 };
  print("vector before erasing 0", v);
  erase(v, 0);
  print("vector  after erasing 0", v);
  erase(v, 55);
  print("vector  after erasing 55", v);

  vb = { false, true, false, false, true, true, false, true };
  print("vector<bool> before erasing false", vb);
  erase(vb, false);
  print("vector<bool>  after erasing false", vb);
}

/*
string before: cute fluffy kittens
string  after: ct flffy kttns
deque before: [10 11 12 14 15 17 18 19]
deque  after: [10 12 14 18]
forward_list before: [10 11 12 14 15 17 18 19]
forward_list  after: [10 12 14 18]
list before: [10 11 12 14 15 17 18 19]
list  after: [10 12 14 18]
vector before: [10 11 12 14 15 17 18 19]
vector  after: [10 12 14 18]
map before: [10/A 11/B 12/C 14/D 15/E 17/F 18/G 19/H]
map  after: [10/A 12/C 14/D 18/G]
multimap before: [20/S 21/T 22/U 22/V 23/W 23/X 24/Y 25/Z]
multimap  after: [20/S 22/U 22/V 24/Y]
set before: [10 11 12 14 15 17 18 19]
set  after: [10 12 14 18]
multiset before: [20 21 22 22 23 23 24 25]
multiset  after: [20 22 22 24]
unordered_map before: [18/G 10/A 19/H 11/B 12/C 14/D 15/E 17/F]
unordered_map  after: [18/G 10/A 12/C 14/D]
unordered_multimap before: [20/S 21/T 22/U 22/V 23/W 23/X 24/Y 25/Z]
unordered_multimap  after: [20/S 22/U 22/V 24/Y]
unordered_set before: [18 10 19 11 12 14 15 17]
unordered_set  after: [18 10 12 14]
unordered_multiset before: [20 21 22 22 23 23 24 25]
unordered_multiset  after: [20 22 22 24]
vector before erasing everything: [17 29 1729]
vector  after erasing everything: []
vector before erasing nothing: [256 512 1024]
vector  after erasing nothing: [256 512 1024]
empty vector before: []
empty vector  after: []
list before erasing everything: [17 29 1729]
list  after erasing everything: []
list before erasing nothing: [256 512 1024]
list  after erasing nothing: [256 512 1024]
empty list before: []
empty list  after: []
set before erasing everything: [17 29 1729]
set  after erasing everything: []
set before erasing nothing: [256 512 1024]
set  after erasing nothing: [256 512 1024]
empty set before: []
empty set  after: []
vector<bool> before: [false true false false true true false true]
vector<bool>  after: [false false false false]
vector before erasing 0: [0 11 0 0 22 33 0 0 44 0]
vector  after erasing 0: [11 22 33 44]
list before erasing 0: [0 11 0 0 22 33 0 0 44 0]
list  after erasing 0: [11 22 33 44]
set before erasing 0: [0 11 22 33 44]
set  after erasing 0: [11 22 33 44]
vector<bool> before erasing false: [false true false false true true false true]
vector<bool>  after erasing false: [true true true true]
*/
