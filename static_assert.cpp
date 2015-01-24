
// /home/ed/bin/bin/g++ -std=c++1z -o static_assert static_assert.cpp

#include <type_traits>

template<typename _Tp>
  _Tp
  real_thing(_Tp __x)
  {
    static_assert(std::is_floating_point<_Tp>::value, "");
    //static_assert(std::is_floating_point<_Tp>::value);
  };

int
main()
{
  real_thing(1);
}
