#ifndef LU_DECOMP_TCC
#define LU_DECOMP_TCC 1


#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#include "matrix_util.h"

namespace matrix
{


/**
 *  This class represents an LU decomposition.
 */
template<typename Type, typename SquareMatrix>
  class lu_decomposition
  {

  public:

    template<typename SquareMatrix2>
      lu_decomposition(std::size_t n, const SquareMatrix2 & a);

    void backsubstitute(VectorOut & b) const;

    template<typename SquareMatrix2, typename Vector2>
      void improve(std::size_t n, const SquareMatrix2 & a,
                   const Vector2 & b, Vector2 & x) const;

    template<typename SquareMatrix2>
      void
      inverse(SquareMatrix2 & a_inv) const;

    Type lu_determinant() const;

    Type trace() const;

  private:

    std::size_t m_n,
    SquareMatrix m_a,
    std::vector<std::size_t> m_index;
    int m_parity;
  };


//
//  Given an n*n matrix a[0..n-1][0..n-1], this routine replaces it by the LU (Lower-triangular Upper-triangular) 
//  decomposition of a rowwise permutation of itself.  n and a[][] are input.  a[][] is output, index[] is an 
//  output vector which row permutation effected by the partial pivoting; d is output as the parity of the row 
//  permutation
//
template<typename Type, typename SquareMatrix, typename Vector>
  bool
  lu_decomp(const std::size_t n, SquareMatrix & a,
            Vector & index, Type & parity)
  {
    const Type TINY = Type(1.0e-20L);

    std::vector<Type> scale(n);

    parity = Type(1);

    //  Loop over rows to get the implicit scaling information.    
    for (std::size_t i = 0; i < n; ++i)
      {
        promote_t<Type> big(0), temp;
        for (std::size_t j = 0; j < n; ++j)
          {
            temp = std::abs(a[i][j]);
            if (temp > big)
              big = temp;
          }
        if (big == Type(0))
          {
            throw std::logic_error("matrix::lu_decomp: Singular matrix in routine lu_decomp.");
            return false;
          }

        //  Save the scaling for the row.
        scale[i] = Type(1) / big;
      }

    //  This is the loop over columns of Crout's method.
    for (std::size_t j = 0; j < n; ++j)
      {
        for (std::size_t i = 0; i < j; ++i)
          {
            Type sum = a[i][j];
            for (std::size_t k = 0; k < i; ++k)
              sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
          }

        //  Initialize for the search for the largest pivot point.
        auto imax = std::numeric_limits<std::size_t>::max();
        Type big = Type(0);
        for (std::size_t i = j; i < n; ++i)
          {
            auto sum = a[i][j];
            for (std::size_t k = 0; k < j; ++k)
              sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
            Type dummy = scale[i] * std::abs(sum);
            if (dummy >= big)
              {
                big = dummy;
                imax = i;
              }
          }
        if (imax == std::numeric_limits<std::size_t>::max())
          {
            throw std::logic_error("matrix::lu_decomp: Singular matrix in routine lu_decomp.");
            return false;
          }

        //  Interchange rows if required.
        if (j != imax)
          {
            for (std::size_t k = 0; k < n; ++k)
              {
                Type dummy = a[imax][k];
                a[imax][k] = a[j][k];
                a[j][k] = dummy;
              }

            //  Change parity.
            parity = -parity;

            //  Interchange the scale factor.
            scale[imax] = scale[j];
          }
        index[j] = imax;
        if (a[j][j] == Type(0))
          a[j][j] = TINY;

        //
        //     Now finally divide by the pivot element
        //
        if (j != n - 1)
          {
            auto scale = Type(1) / std::abs(a[j][j]);  //  Changed this to 1/abs(a) from simple 1/a.
            for (std::size_t i = j + 1; i < n; ++i)
              a[i][j] *= scale;
          }
    }  //  Go back for the next column in the reduction.

    return true;
  }



//
//  Solve the set of n linear equations a.x = b.  Here a[0..n-1][0..n-1] is input, not as the original matrix a but as 
//  its LU decomposition, determined by the routine lu_decomp().  b[0..n-1] is input as the right hand side vector b 
//  and returns with the left-hand solution vector x.  a, n, and index are not modified by this routine and can be left 
//  in place for successive calls with different right hand sides b[0..n-1].  This routine takes into account the 
//  possibility that b will begin with a lot of zeros so that it is efficient for use in matrix inversion.
//
template<typename SquareMatrix, typename Vector, typename VectorOut>
  void
  lu_backsub(const std::size_t n,
             const SquareMatrix & a,
             const Vector & index,
             VectorOut & b)
  {
    using Type = std::remove_reference_t<decltype(a[0][0])>;

    //  When i_start is set to a non-negative value, it will become the index
    //  of the first nonvanishing element of b[0..n-1].
    //  Do the forward substitution unsrambling the permutation as we go.
    int i_start = -1;
    for (std::size_t i = 0; i < n; ++i)
      {
        auto i_perm = index[i];
        auto sum = b[i_perm];
        b[i_perm] = b[i];
        if (i_start > -1)
          for (std::size_t j = i_start; j <= i - 1; ++j)
            sum -= a[i][j] * b[j];
        else if (sum != Type(0))
          i_start = i;
        b[i] = sum;
      }

    //  Now do the backsubstitution.
    for (std::ptrdiff_t i = n - 1; i >= 0; --i)
      {
        auto sum = b[i];
        for (std::size_t j = i + 1; j < n; ++j)
          sum -= a[i][j] * b[j];
        b[i] = sum / a[i][i];
      }

    return;
  }



//
//  Improves a solution vector x of the linear set A.x = b.  The matrix a and the
//  LU decomposition of a a_lu (with its row permutation vector index) and the
//  right-hand side vector are input along with the solution vector x.  
//  The solution vector x is improved and modified on output.
//
template<typename SquareMatrix, typename Vector>
  void
  lu_improve(const std::size_t n, const SquareMatrix & a,
             const SquareMatrix & a_lu,
             const Vector & index, const Vector & b, Vector & x)
  {
    using Type = std::remove_reference_t<decltype(a[0][0])>;

    std::vector<Type> r(n);

    for (std::size_t i = 0; i < n; ++i)
      {
        r[i] = -b[i];
        for (std::size_t j = 0; j < n; ++j)
          r[i] += a[i][j] * x[j];
      }

    lu_backsub(a_lu, n, index, r);

    for (std::size_t i = 0; i < n; ++i)
      x[i] -= r[i];

    return;
  }


//
//  Inverts a matrix given the LU decomposed matrix.
//
//  The inverse matrix is NOT in LU form.
//
template<typename SquareMatrix, typename Vector>
  void
  lu_invert(const std::size_t n, const SquareMatrix & a_lu,
            const Vector & index, SquareMatrix & a_inv)
  {
    using Type = std::remove_reference_t<decltype(a_inv[0][0])>;

    for (std::size_t j = 0; j < n; ++j)
      {
        std::vector<Type> col(n);
        col[j] = Type(1);

        lu_backsub(n, a_lu, index, col);

        for (std::size_t i = 0; i < n; ++i)
          a_inv[i][j] = col[i];
      }

    return;
  }


//
//  Compute determinant of LU decomposed matrix.
//
template<typename Type, typename SquareMatrix>
  Type
  lu_determinant(const std::size_t n, const SquareMatrix& a_lu, const Type parity)
  {
    auto det = parity;

    for (std::size_t i = 0; i < n; ++i)
      det *= a_lu[i][i];

    return det;
  }


//
//  Compute trace of LU decomposed matrix.
//
template<typename SquareMatrix>
  auto
  lu_trace(const std::size_t n, const SquareMatrix& a_lu)
  {
    using Type = std::remove_reference_t<decltype(a_lu[0][0])>;

    auto trace = Type(0);

    for (std::size_t i = 0; i < n; ++i)
      {
        trace += a_lu[i][i];
        for (std::ptrdiff_t j = i - 1; j >= 0; --j)
          trace += a_lu[i][j] * a_lu[j][i];
      }

    return trace;
  }

}  //  namespace matrix

#endif  //  LU_DECOMP_TCC
