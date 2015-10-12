
// $HOME/bin/bin/g++ -o test_matrix test_matrix.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./test_matrix

#include <iostream>
#include <iomanip>

#include "matrix.h"

//template<typename Tp, size_t M, size_t N>
//  void
//  algo(matrix<Tp,M,N> m)
//  {
//    ;
//  }

int
main()
{
  const std::size_t M = 3, N = 3;
  double a[M][N]{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  auto detA = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
            + a[0][1] * (a[1][2] * a[2][0] - a[1][0] * a[2][2])
            + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);

  std::cout << "\n  det(a) = " << std::setw(10) << detA << '\n';

  std::cout << "\nA\n";
  for (auto & row : a)
    {
      for (auto & col : row)
        std::cout << ' ' << std::setw(10) << col;
      std::cout << '\n';
    }
  std::cout << '\n';

  double a_inv[M][N];
  int index[M];
  double parity;
  matrix::lu_decomp(3, a, index, parity);

  std::cout << "\nA_LU\n";
  for (auto & row : a)
    {
      for (auto & col : row)
        std::cout << ' ' << std::setw(10) << col;
      std::cout << '\n';
    }
  std::cout << '\n';

  std::cout << "\n  det(a) = " << std::setw(10) << matrix::lu_determinant(3, a, parity) << '\n';

  std::cout << "\n  trace(a) = " << std::setw(10) << matrix::lu_trace(3, a) << '\n';

  matrix::lu_invert(3, a, index, a_inv);

  std::cout << "\nA^-1\n";
  for (auto & row : a_inv)
    {
      for (auto & col : row)
        std::cout << ' ' << std::setw(10) << col;
      std::cout << '\n';
    }
  std::cout << '\n';

  double I[M][N];
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      {
        I[i][j] = 0.0;
        for (int k  = 0; k < 3; ++k)
          I[i][j] += a[i][k] * a_inv[k][j];
      }

  std::cout << "\nA.A^-1\n";
  for (auto & row : I)
    {
      for (auto & col : row)
        std::cout << ' ' << std::setw(10) << col;
      std::cout << '\n';
    }
  std::cout << '\n';

  //algo(a);
  //matrix::matrix<double[M][N]> ma(a);
  //matrix::matrix<array<double,N*M> > mb;

  return 0;
}
