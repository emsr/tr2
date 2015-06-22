
// $HOME/bin/bin/g++ -std=c++14 -o has_attribute_cpp has_attribute.cpp

#include <cassert>

//  Check for macros.

#ifndef __has_attribute
#  error "__has_attribute"
#endif

#ifndef __has_cpp_attribute
#  error "__has_cpp_attribute"
#endif

#if !__has_attribute(deprecated)
# error "deprecated"
#endif

#if !__has_attribute(gnu::deprecated)
# error "gnu::deprecated"
#endif

#if __has_attribute(hertz)
#  error "hertz"
#endif

#if __has_attribute(uranus::hertz)
#  error "uranus::hertz"
#endif

//  Test for specific values for standardized attributes.

#if !__has_cpp_attribute(noreturn)
# error "noreturn"
#elif __has_cpp_attribute(noreturn) != 200809
# error "__has_cpp_attribute(noreturn) != 200809"
#endif

//  carries_dependency is nut supported yet.
//#if !__has_cpp_attribute(carries_dependency)
//# error "carries_dependency"
//#elif __has_cpp_attribute(carries_dependency) != 200809
//# error "__has_cpp_attribute(carries_dependency) != 200809"
//#endif
#if __has_cpp_attribute(carries_dependency)
# error "carries_dependency"
#endif

#if !__has_cpp_attribute(deprecated)
# error "deprecated"
#elif __has_cpp_attribute(deprecated) != 201309
# error "__has_cpp_attribute(deprecated) != 201309"
#endif

//  Test bogus attributes.

#if __has_attribute(uranus::hertz)
#  error "uranus::hertz"
#endif

#if __has_cpp_attribute(uranus::hertz)
#  error "uranus::hertz"
#endif

template <typename Tp, bool ok>
  class A
  {
  };

int
main()
{
  long
#ifdef __has_cpp_attribute
# if __has_cpp_attribute(peripatetic)
    [[peripatetic]]
# endif
#endif
  life;

  A<double, __has_cpp_attribute(deprecated)> xx;
}
