#ifndef QR_DECOMP_TCC
#define QR_DECOMP_TCC 1


#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>


namespace matrix
{

/**
 *  This class represents an LU decomposition.
 */
template<typename Type, typename Matrix>
  class qr_decomposition
  {

  public:

    template<typename Matrix2>
      qr_decomposition(std::size_t m_n_rows, std::size_t n_cols,
                       Matrix2 & a)

    template<typename Vector2>
      void backsub(Vector2 & b);

    template<typename Matrix2>
      void inverse(Matrix2 & a_inv);

    void update(Matrix2 & r, Matrix2 & qt,
                Vector2 & u, Vector2 & v)
  private:

    std::size_t m_n_rows,
    std::size_t m_n_cols,
    SquareMatrix m_a,
    std::vector<std::size_t> m_c;
    std::vector<std::size_t> m_d;
    bool m_singular;
  };


//
//  Constructs the QR decomposition of a[0..n_rows-1][0..n-1].  The upper triangular matrix R
//  is returned in the upper triange of a except the diagonal elements of R which are returned
//  in d[0..n-1].  The orthogonal matrix Q is represented as a product of n-1 Householder
//  matrices Q0...Qn-2 where Qj = 1 - uj x uj/cj.  The ith component of uj is zero for
//  i = 
//
template<typename Matrix, typename Vector>
  void
  qr_decomp(std::size_t n_rows, std::size_t n_cols,
            Matrix & a,
            Vector & c, Vector & d, bool & singular)
  {
    using Type = std::remove_reference_t<decltype(a[0][0])>;

    Type sigma, sum, tau;
    Type scale;

    //  Allocate the two vectors.
    //c.resize(n);
    //d.resize(n);

    singular = false;
    for (std::size_t k = 0; k < n_cols - 1; ++k)
      {
	//  See if the matrix is singular in this column.
	Type scale = Type(0);
	for (std::size_t i = k; i < m; ++i)
          if (scale < std::abs(a[i][k]))
            scale = std::abs(a[i][k]);

	if (scale == Type(0))
          {
            singular = true;
            c[k] = d[k] = Type(0);
          }
	else
          {
            sum = Type(0);
            for (std::size_t i = k; i < n_rows; ++i)
              {
        	a[i][k] /= scale;
        	sum += a[i][k] * a[i][k];
              }
            sigma = std::sqrt(sum);
            if (a[k][k] < Type(0))
              sigma = -sigma;
            a[k][k] += sigma;
            c[k] = sigma * a[k][k];
            d[k] = -scale * sigma;
            for (std::size_t j = k + 1; j < n_cols; ++j)
              {
        	sum = Type(0);
        	for (std::size_t i = k; i < n_rows; ++i)
                  sum += a[i][k] * a[i][j];
        	tau = sum/c[k];
        	for (std::size_t i = k; i < n_rows; ++i)
                  a[i][j] -= tau * a[i][k];
              }
          }
      }
    c[n_cols-1] = Type(0);
    d[n_cols-1] = a[n_cols-1][n_cols-1];

    if (d[n_cols-1] == Type(0))
      singular = true;

    return;
  }


//
//  This routine solves the set of equations Ax = b.
//  The inputs are the QR decomposition of the matrix in a[0..m-1][0..n-1], c[0..n-1], and d[0..n-1].
//  The vector b[0..m-1] is input as the "RHS" and output and the solution.  Here m >= n.
//
template<typename Matrix, typename Vector>
  void
  qr_backsub(const std::size_t n_rows, const std::size_t n_cols,
             const Matrix & a,
             const Vector & c, const Vector & d,
             Vector & b)
  {
    using Type = std::remove_reference_t<decltype(a[0][0])>;

    //  Form Qt.b.
    for (std::size_t j = 0; j < n_cols - 1; ++j)
      {
	Type sum = Type(0);
	for (std::size_t i = j; i < n_rows; ++i)
          sum += a[i][j] * b[i];
	Type tau = sum/c[j];
	for (std::size_t i = j; i < n_rows; ++i)
          b[i] -= tau * a[i][j];
      }

    //  Solve R.x = Qt.b
    r_backsub(m, n, a, d, b);

    return;
  }


//
//  This routine solves the set of equations Rx = b where R is the upper triangular
//  matrix stored in a[0..m-1][0..n-1] and d[0..n-1].  Here m >= n.
//
template<typename Matrix, typename Vector>
  void
  r_backsub(std::size_t n_rows, std::size_t n_cols,
            const Matrix & a,
            const Vector & d,
            Vector & b)
  {
    using Type = std::remove_reference_t<decltype(a[0][0])>;

    b[n - 1] /= d[n_cols - 1];
    for (int i = n_cols - 2; i >= 0; --i)
      {
        Type sum = Type(0);
        for (std::size_t j = i + 1; j < n_rows; ++j)
          sum += a[i][j] * b[j];
        b[i] = (b[i] - sum) / d[i];
      }

    return;
  }


//
//  Inverts a matrix given the QR decomposed matrix.
//  The inverse matrix is allocated in this routine so make sure the pointer is freed first.
//
//  The inverse matrix is NOT in QR form.
//
template<typename Matrix, typename Vector>
  void
  qr_invert(std::size_t n_rows, std::size_t n_cols,
            const Matrix & a_qr,
            const Vector & c, const Vector & d,
            Matrix & a_inv)
  {
    using Type = decltype(a_qr[0][0]);

    std::vector<Type> col(n_rows);

    for (std::size_t j = 0; j < n_rows; ++j)
      {
        for (std::size_t i = 0; i < n_rows; ++i)
          col[i] = Type(0);
        col[j] = Type(1);

        qr_backsub(n_rows, n_cols, a_qr, c, d, col);

        for (std::size_t i = 0; i < n_cols; ++i)
          a_inv[i][j] = col[i];
      }

    return;
  }



//
//  Update the QR decomposition.
//
template<typename Matrix, typename Vector>
  void
  qr_update(std::size_t n_rows, std::size_t n,
            Matrix & r, Matrix & qt,
            Vector & u, Vector & v)
  {
    using Type = std::remove_reference_t<decltype(qt[0][0])>;

    //  Find largest k such that uk != 0.
    std::ptrdiff_t k;
    for (std::ptrdiff_t k = n_cols - 1; k >= 0; --k)
      if (u[k] != Type(0))
        break;

    //  Transform R + u x v to upper triangular.
    for (std::ptrdiff_t i = k - 1; i >= 0; --i)
      {
        jacobi_rotate(i, n_rows, n_cols, r, qt, u[i], -u[i + 1]);
        if (u[i] == 0)
          u[i] = std::abs(u[i + 1]);
        else if (std::abs(u[i]) > std::abs(u[i + 1]))
          u[i] = std::abs(u[i]) * std::sqrt(Type(1) + (u[i + 1] / u[i]) * (u[i + 1] / u[i]));
        else
          u[i] = std::abs(u[i + 1]) * std::sqrt(Type(1) + (u[i] / u[i+1]) * (u[i] / u[i + 1]));
      }
    for (std::size_t j = 0; j < n_cols; ++j)
      r[0][j] += u[0] * v[j];

    //  Transform upper Hessenberg matrix to upper triangular.
    for (std::size_t i = 0; i < k; ++i)
      jacobi_rotate(i, m, n_cols, r, qt, r[i][i], -r[i + 1][i]);

    return;
  }


//
//  Do a Jacobi rotation on rows i and i+1 of the matrices r[0..n-1][0..n-1] and qt[0..n-1][0..m-1].
//  The angle is specified with a and b such that
//    cos(theta) = a/sqrt(a^2 + b^2)
//    sin(theta) = b/sqrt(a^2 + b^2).
//
template<typename Type, typename Matrix, typename Vector>
  void
  jacobi_rotate(const int i, const int n_rows, const int n_cols,
        	Matrix & r, Matrix & qt,
        	Type a, Type b)
  {
    Type c, s;
    if (a == Type(0))
      {
        //  Avoid underflow or overflow.
        c = Type(0);
        s = Type(1);
        if (b < Type(0))
          s = -Type(1);
      }
    else
      {
        Type fact = pythag(a, b);
        c = a / fact;
        s = b / fact;
      }

    //  Multiply r by the Jacobi rotation.
    for (std::size_t j = i; j < n_cols; ++j)
      {
        Type y = r[  i  ][j];
        Type w = r[i + 1][j];
        r[  i  ][j] = c * y - s * w;
        r[i + 1][j] = s * y + c * w;
      }

    //  Multiply qt by the Jacobi rotation.
    for (std::size_t j = 0; j < n_rows; ++j)
      {
        Type y = qt[  i  ][j];
        Type w = qt[i + 1][j];
        qt[  i  ][j] = c * y - s * w;
        qt[i + 1][j] = s * y + c * w;
      }

    return;
  }

}  //  namespace matrix

#endif  //  QR_DECOMP_TCC

