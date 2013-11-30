// /home/ed/bin_feature/bin/g++ -std=c++11 -o features_test_cxx11 features_test_cxx11.cpp

// Begin C++11 tests.

#ifndef __cpp_unicode_characters
#  error "__cpp_unicode_characters"
#endif

#ifndef __cpp_raw_strings
#  error "__cpp_raw_strings"
#endif

#ifndef __cpp_unicode_literals
#  error "__cpp_unicode_literals"
#endif

#ifndef __cpp_user_defined_literals
#  error "__cpp_user_defined_literals"
#endif

#ifndef __cpp_lambdas
#  error "__cpp_lambdas"
#endif

#ifdef __cpp_constexpr
#  if __cpp_constexpr != 200704
#    error "__cpp_constexpr"
#  endif
#else
#  error "__cpp_constexpr"
#endif

#ifndef __cpp_static_assert
#  error "__cpp_static_assert"
#endif

#ifndef __cpp_decltype
#  error "__cpp_decltype"
#endif

#ifndef __cpp_attributes
#  error "__cpp_attributes"
#endif

#ifndef __cpp_rvalue_reference
#  error "__cpp_rvalue_reference"
#endif

#ifndef __cpp_variadic_templates
#  error "__cpp_variadic_templates"
#endif

int
main()
{
}
