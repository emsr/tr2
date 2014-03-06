// N3644 Null Forward Iterators

// /home/ed/bin/bin/g++ -std=c++1y -o null_iterators null_iterators.cpp

#include <vector>
#include <deque>
#include <cassert>

template<typename _Tp, template<typename, typename> class _Cont>
  void
  test(_Cont<_Tp, std::allocator<_Tp>>)
  {
    typename _Cont<_Tp, std::allocator<_Tp>>::iterator vi1{};
    typename _Cont<_Tp, std::allocator<_Tp>>::iterator vi2{};
    assert(vi1 == vi2);
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

  test(std::vector<long>{});

  //assert(vi1 != vd1); // Won't compile.

  //vi1 == vd1; // Won't compile.
}
