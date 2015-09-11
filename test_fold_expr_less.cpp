//  clang++ -std=c++1z -o test_fold_expr_less test_fold_expr_less.cpp

//  /home/ed/bin_git/bin/g++ -std=c++1z -o test_fold_expr_less test_fold_expr_less.cpp

//  ./test_fold_expr_less

#include <iostream>
#include <utility>

std::uint64_t arr1[8] = {1, 7, 2, 4, 8, 9, 3, 6};
std::uint64_t arr2[8] = {1, 7, 2, 4, 8, 9, 3, 6};
std::uint64_t arr3[8] = {1, 7, 2, 5, 8, 9, 3, 6};
std::uint64_t arr4[8] = {1, 7, 2, 3, 8, 9, 3, 6};
std::uint64_t arr5[8] = {1, 7, 2, 3, 8, 9, 3, 2};

struct less_t
{
  template<typename T, std::size_t N, std::size_t... I>
    bool
    impl(T const (&lhs)[N], T const (&rhs)[N], std::index_sequence<I...>) const
    {
      std::size_t i{0};
      if (((i = I, (lhs[I] < rhs[I]) ? true : lhs[I] != rhs[I]) || ...))
        return lhs[i] < rhs[i];
      else
        return false;
    }

  template<typename T, std::size_t N>
    bool
    operator()(T const (&lhs)[N], T const (&rhs)[N]) const
    {
      return impl(lhs, rhs, std::make_index_sequence<N>());
    }
};

int
main()
{
  less_t const less{};

  std::cout << std::boolalpha;
  std::cout << "=== less ===" << std::endl;

  std::cout << less(arr1, arr2) << std::endl;
  std::cout << less(arr1, arr3) << std::endl;
  std::cout << less(arr1, arr4) << std::endl;
  std::cout << less(arr1, arr5) << std::endl;

  std::cout << less(arr2, arr1) << std::endl;
  std::cout << less(arr2, arr3) << std::endl;
  std::cout << less(arr2, arr4) << std::endl;
  std::cout << less(arr2, arr5) << std::endl;

  std::cout << less(arr3, arr1) << std::endl;
  std::cout << less(arr3, arr2) << std::endl;
  std::cout << less(arr3, arr4) << std::endl;
  std::cout << less(arr3, arr5) << std::endl;

  std::cout << less(arr4, arr1) << std::endl;
  std::cout << less(arr4, arr2) << std::endl;
  std::cout << less(arr4, arr3) << std::endl;
  std::cout << less(arr4, arr5) << std::endl;

  std::cout << less(arr5, arr1) << std::endl;
  std::cout << less(arr5, arr2) << std::endl;
  std::cout << less(arr5, arr3) << std::endl;
  std::cout << less(arr5, arr4) << std::endl;
}

