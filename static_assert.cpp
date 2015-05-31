
// /home/ed/bin/bin/g++ -std=c++1z -o static_assert static_assert.cpp

// /home/ed/bin/bin/g++ -std=c++11 -o static_assert static_assert.cpp

// /home/ed/bin/bin/g++ -pedantic -std=c++11 -o static_assert static_assert.cpp

// /home/ed/bin_concepts/bin/g++ -std=c++1z -o static_assert static_assert.cpp

#include <type_traits>

template<typename _Tp>
  _Tp
  real_thing(_Tp __x)
  {
    static_assert(std::is_floating_point<_Tp>::value, L"");
    static_assert(std::is_floating_point<_Tp>::value, u8"");
    static_assert(std::is_floating_point<_Tp>::value, L"Important message");
    static_assert(std::is_floating_point<_Tp>::value, u8"Important message");
    static_assert(std::is_floating_point<_Tp>::value, "Important message");
    static_assert(std::is_floating_point<_Tp>::value, "");
    static_assert(std::is_floating_point<_Tp>::value);
  }

int
main()
{
  real_thing(1);
}
