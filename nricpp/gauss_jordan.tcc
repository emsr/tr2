#ifndef GAUSS_JORDAN_TCC
#define GAUSS_JORDAN_TCC 1


#include <vector>
#include <cmath>
#include <stdexcept>


/**
 *  Linear equation solution by Gauss-Jordan elimination.
 *  a[1..n][1..n] is the input matrix and b[1..n][1..m] is the input matrix
 *  of m right-hand side vectors.
 *  On output, a is replaced by its inverse, and b is replaced by the corresponding set of
 *  solution vectors.
 */
template<typename _Tp>
  void
  gauss_jordan(_Tp **a, int n, _Tp **b, int m)
  {

    std::vector<int> index_col(n);
    std::vector<int> index_row(n);
    std::vector<int> pivot(n);

    for (int i = 0; i < n; ++i)
      {
	int icol, irow;
	//  Loop over columns to be reduced.
	auto big = _Tp{0};
	for (int j = 0; j < n; ++j)
	  //  Look for the pivot elements.
	  if (pivot[j] != 1)
	    for (int k = 0; k < n; ++k)
	      {
		if (pivot[k] == 0)
		  {
		    if (std::fabs(a[j][k]) >= big)
		      {
			big = std::fabs(a[j][k]);
			irow = j;
			icol = k;
		      }
		  }
		else if (pivot[k] > 1)
		  throw std::logic_error("gauss_jordan: singular matrix");
	      }
	++pivot[icol];

	//  With the pivot elements in hand, we swap rows to put the pivot elements on the diagonal.
	//  The columns are not physically moved, only relabeled: pivot[i], the column of the
	//  original ith pivot element, is the ith column that is reduced, while index_row[i] is the
	//  row in which that pivot element was originally located.
	//  If index_row[i] != index_col[i] an implied column interchange.
	if (irow != icol)
	  {
	    for (int l = 0; l < n; ++l)
	      std::swap(a[irow][l], a[icol][l]);
	    for (int l = 0; l < m; ++l)
	      std::swap(b[irow][l], b[icol][l]);
	  }
	index_row[i] = irow;
	index_col[i] = icol;
	if (a[icol][icol] == _Tp{0})
	  throw std::logic_error("gauss_jordan: singular matrix");

	auto pivinv = _Tp{1} / a[icol][icol];
	a[icol][icol] = _Tp{1};

	for (int l = 0; l < n; ++l)
	  a[icol][l] *= pivinv;

	for (int l = 0; l < m; ++l)
	  b[icol][l] *= pivinv;

	for (int ll = 0; ll < n; ++ll)
	  if (ll != icol)
	    {
	      auto dum = a[ll][icol];
	      a[ll][icol] = _Tp{0};
	      for (l = 0; l < n; ++l)
		a[ll][l] -= a[icol][l] * dum;
	      for (l = 0; l < m; ++l)
		b[ll][l] -= b[icol][l] * dum;
	    }
      }
    for (int l = n - 1; l >= 0; --l)
      {
	if (index_row[l] != index_col[l])
	  for (int k = 0; k < n; ++k)
	    std::swap(a[k][index_row[l]], a[k][index_col[l]]);
      }
  }


#endif  //  GAUSS_JORDAN_TCC
