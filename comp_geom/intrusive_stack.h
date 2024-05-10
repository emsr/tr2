
// Maybe this would help with Graham scan.

#include <stack>
#include <vector>

template<typename Tp, typename Sequence = std::vector<Tp> >
  class intrusive_stack : public std::stack<Tp, Sequence>
  {
    using Base = std::stack<Tp, Sequence>;

  public:

    const typename Base::container_type&
    container() const
    { return this->c; }

    typename Base::container_type&
    container()
    { return this->c; }
  };
