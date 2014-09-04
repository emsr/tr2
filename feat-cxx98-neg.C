// { dg-do compile { target c++98_only } }
// { dg-options "-ansi" }

#ifdef __cpp_binary_literals
#  error "__cpp_binary_literals" // { dg-error "error" }
#endif

#ifdef __cpp_runtime_arrays
#  error "__cpp_runtime_arrays" // { dg-error "error" }
#endif
