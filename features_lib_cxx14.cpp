// /home/ed/bin_feature/bin/g++ -std=c++1y -c features_lib_cxx14.cpp

#include <utility>
#include <tuple>
#include <memory>
#include <functional>
#include <type_traits>
#include <chrono>
#include <string>
#include <complex>
#include <iomanip>

#ifndef  __cpp_lib_integer_sequence
#  error "__cpp_lib_integer_sequence"
#endif
#ifndef  __cpp_lib_exchange_function
#  error "__cpp_lib_exchange_function"
#endif
#ifndef  __cpp_lib_tuples_by_type
#  error "__cpp_lib_tuples_by_type"
#endif
#ifndef  __cpp_lib_make_unique
#  error "__cpp_lib_make_unique"
#endif
#ifndef  __cpp_lib_transparent_operators
#  error "__cpp_lib_transparent_operators"
#endif
#ifndef  __cpp_lib_result_of_sfinae
#  error "__cpp_lib_result_of_sfinae"
#endif
#ifndef  __cpp_lib_integral_constant_callable
#  error "__cpp_lib_integral_constant_callable"
#endif
#ifndef  __cpp_lib_transformation_trait_aliases
#  error "__cpp_lib_transformation_trait_aliases"
#endif
#ifndef  __cpp_lib_chrono_udls
#  error "__cpp_lib_chrono_udls"
#endif
#ifndef  __cpp_lib_string_udls
#  error "__cpp_lib_string_udls"
#endif
#ifndef __cpp_lib_complex_udls
#  error "__cpp_lib_complex_udls"
#endif
#ifndef  __cpp_lib_generic_associative_lookup
#  error "__cpp_lib_generic_associative_lookup"
#endif
//#ifndef  __cpp_lib_null_iterators
//#  error "__cpp_lib_null_iterators"
//#endif
#ifndef  __cpp_lib_robust_nonmodifying_seq_ops
#  error "__cpp_lib_robust_nonmodifying_seq_ops"
#endif
#ifndef  __cpp_lib_quoted_string_io
#  error "__cpp_lib_quoted_string_io"
#endif

#if !__has_include(<experimental/optional>)
#  error "<experimental/optional>"
#endif
#if !__has_include(<shared_mutex>)
#  error "<shared_mutex>"
#endif
