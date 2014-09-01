// clang++ -std=c++1y -Itestinc -o features_test_cxx14_clang features_test_cxx14_clang.cpp

// Begin C++11 tests which should still pass.
/*
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

#ifndef __cpp_constexpr
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

// Begin C++14 tests.

#ifndef __cpp_binary_literals
#  error "__cpp_binary_literals"
#endif

#ifndef __cpp_init_captures
#  error "__cpp_init_captures"
#endif

#ifndef __cpp_generic_lambdas
#  error "__cpp_generic_lambdas"
#endif

#ifdef __cpp_constexpr
#  if __cpp_constexpr < 201304
#    error "__cpp_constexpr"
#  endif
#endif

#ifndef __cpp_decltype_auto
#  error "__cpp_decltype_auto"
#endif

#ifndef __cpp_return_type_deduction
#  error "__cpp_return_type_deduction"
#endif

#ifndef __cpp_runtime_arrays
#  error "__cpp_runtime_arrays"
#endif

//  Aggregate initializers not in yet.
#ifdef __cpp_aggregate_nsdmi
#  error "__cpp_aggregate_nsdmi"
#endif

//  Variable templates not in yet.
#ifdef __cpp_variable_templates
#  error "__cpp_variable_templates"
#endif

//  Check undefined macro.
#if defined(gnu_poo)
#  error "defined(gnu_poo)"
#endif
*/
//  Check for __has_include macro.
#ifndef __has_include
#  error "__has_include"
#endif

//  Try known bracket header (use operator).
#if __has_include__(<complex>)
#else
#  error "<complex>"
#endif

//  Define and use a macro to invoke the operator.
#define sluggo(TXT) __has_include__(TXT)

#if sluggo(<complex>)
#else
#  error "<complex>"
#endif

#if ! sluggo(<complex>)
#  error "<complex>"
#else
#endif

//  Quoted complex.h should find at least the bracket version.
#if __has_include("complex.h")
#else
#  error "complex.h"
#endif

//  Try known local quote header.
#if __has_include("complex_literals.h")
#else
#  error "\"complex_literals.h\""
#endif

//  Try nonexistent bracket header.
#if __has_include(<stuff>)
#  error "<stuff>"
#else
#endif

//  Try nonexistent quote header.
#if __has_include("phlegm")
#  error "\"phlegm\""
#else
#endif

//  Test __has_include_next.
//#if __has_include("phoobhar.h")
//#  include "phoobhar.h"
//#else
//#  error "phoobhar.h"
//#endif

//  Try a macro.
#define COMPLEX_INC "complex.h"
#if __has_include(COMPLEX_INC)
#else
#  error COMPLEX_INC
#endif

//  Realistic use of __has_include.
#if __has_include(<array>)
#  define STD_ARRAY 1
#  include <array>
  template<typename _Tp, size_t _Num>
    using array = std::array<_Tp, _Num>;
#elif __has_include(<tr1/array>)
#  define TR1_ARRAY 1
#  include <tr1/array>
  template<typename _Tp, size_t _Num>
    typedef std::tr1::array<_Tp, _Num> array;
#endif

int
main()
{
}
