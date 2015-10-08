
// $HOME/bin/bin/g++ -o test_matrix test_matrix.cpp 

#include "matrix"

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
  double a[M][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double ainv[M][N];
  int index[M];
  double parity;
  matrix::lu_decomp(3, a, index, parity);
  matrix::lu_invert(3, a, index, ainv);

  //algo(a);
  //matrix::matrix<double[M][N]> ma(a);
  //matrix::matrix<array<double,N*M> > mb;

  return 0;
}
