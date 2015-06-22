// N3644 Null Forward Iterators
// Discussion: gcc.gnu.org/ml/libstdc++/2013-11/msg00076.html

// $HOME/bin/bin/g++ -std=c++1y -o null_iterators null_iterators.cpp

#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <cassert>
#include <set>

template<typename _Tp, template<typename, typename> class _Cont>
  void
  test2(_Cont<_Tp, std::allocator<_Tp>>)
  {
    typename _Cont<_Tp, std::allocator<_Tp>>::iterator ci1{};
    typename _Cont<_Tp, std::allocator<_Tp>>::iterator ci2{};
    assert(ci1 == ci2);
  }

template<typename _Tp, template<typename, typename, typename> class _Cont>
  void
  test3(_Cont<_Tp, std::less<_Tp>, std::allocator<_Tp>>)
  {
    typename _Cont<_Tp, std::less<_Tp>, std::allocator<_Tp>>::iterator ci1{};
    typename _Cont<_Tp, std::less<_Tp>, std::allocator<_Tp>>::iterator ci2{};
    assert(ci1 == ci2);
  }

struct A
{
  virtual std::vector<int>::const_iterator
  begin() const
  { return std::vector<int>::const_iterator{}; }

  virtual std::vector<int>::const_iterator
  end() const
  { return std::vector<int>::const_iterator{}; }
};

A __gA{};

struct B
: public A
{
  virtual std::vector<int>::const_iterator
  begin() const override
  { return v.cbegin(); }

  virtual std::vector<int>::const_iterator
  end() const override
  { return v.cend(); }

  std::vector<int> v;
};

B __gB{};

const A&
get_an_A(int i)
{
  if (i % 2)
    return __gB;
  else
    return __gA;
}

void
test0()
{
  const A& ar = get_an_A(1);
  const A& as = get_an_A(2);

  for (int x : ar);
  //  do_something(x);
  //do_something_else(ar);
}

int
main()
{
  std::vector<int>::iterator vi1{};
  std::vector<int>::iterator vi2{};
  assert(vi1 == vi2);

  std::vector<double>::iterator vd1{};
  std::vector<double>::iterator vd2{};
  assert(vd1 == vd2);

  std::deque<int>::iterator di1{};
  std::deque<int>::iterator di2{};
  assert(di1 == di2);

  std::deque<double>::iterator dd1{};
  std::deque<double>::iterator dd2{};
  assert(dd1 == dd2);

  test2(std::vector<long>{});
  test2(std::deque<long>{});
  test2(std::deque<long>{});
  test2(std::list<long>{});
  test2(std::forward_list<long>{});
  test3(std::set<long>{});

  //vi1 == vd1; // Won't compile.
}
