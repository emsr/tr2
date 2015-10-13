#ifndef GAUSS_JORDAN_TCC
#define GAUSS_JORDAN_TCC 1


#include <vector>
#include <cmath>
#include <stdexcept>


/**
 *  Linear equation solution by Gauss-Jordan elimination. a[1..n][1..n] is an input matrix.
 *  b[1..n][1..m] is the input matrix of m right-hand side vectors.
 *  On output, a is replaced by its inverse, and b is replaced by the corresponding set of
 *  solution vectors.
 */
  void
  gauss_jordan( double **a, int n, double **b, int m )
  {
    int icol, irow;

    std::vector<int> indxc(n);
    std::vector<int> indxr(n);
    std::vector<int> ipiv(n);

    for (int i = 0; i < n; ++i )
      {
        //  Loop over columns to be reduced.
        auto big = 0.0;
        for (int j = 0; j < n; ++j)
          //  Loop over rows looking for the pivot elements.
          if (ipiv[j] != 1)
            for (int k = 0; k < n; ++k)
              {
                if (ipiv[k] == 0)
                  {
                    if (std::fabs(a[j][k]) >= big)
                      {
                        big = std::fabs(a[j][k]);
                        irow = j;
                        icol = k;
                      }
                  }
                else if (ipiv[k] > 1)
                  throw std::logic_error( "Singular matrix in gauss_jordan" );
              }
        ++ipiv[icol];

        //  With the pivot elements in hand, we swap rows to put the pivot elements on the diagonal.
        //  The columns are not physically moved, only relabeled: ipiv[i], the column of the
        //  original ith pivot element, is the ith column that is reduced, while indxr[i] is the
        //  row in which that pivot element was originally located.
        //  If indxr[i] != indxc[i] an implied column interchange.
        if (irow != icol)
          {
            for (int l = 0; l < n; ++l)
              std::swap(a[irow][l], a[icol][l] );
            for (int l = 0; l < m; ++l)
              std::swap(b[irow][l], b[icol][l] );
          }
        indxr[i] = irow;
        indxc[i] = icol;
        if (a[icol][icol] == 0.0)
          throw std::logic_error( "Singular matrix error 2 in gauss_jordan" );

        auto pivinv = 1 / a[icol][icol];
        a[icol][icol] = 1.0;

        for (int l = 0; l < n; ++l )
          a[icol][l] *= pivinv;

        for (int l = 0; l < m; ++l )
          b[icol][l] *= pivinv;

        for (int ll = 0; ll < n; ++ll )
          if ( ll != icol )
            {
              auto dum = a[ll][icol];
              a[ll][icol] = 0.0;
              for ( l = 0; l < n; ++l )
                a[ll][l] -= a[icol][l] * dum;
              for ( l = 0; l < m; ++l )
                b[ll][l] -= b[icol][l] * dum;
            }
      }
    for (int l = n - 1; l >= 0; --l)
      {
        if (indxr[l] != indxc[l])
          for (int k = 0; k < n; ++k )
            std::swap(a[k][indxr[l]], a[k][indxc[l]]);
      }
  }


#endif  //  GAUSS_JORDAN_TCC
