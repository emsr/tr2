#ifndef CHOLESKY_DECOMP_TCC
#define CHOLESKY_DECOMP_TCC 1


#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <vector>


namespace matrix
{


/**
 *  This class represents a Cholesky decomposition of a square matrix.
 */
template<typename SquareMatrix, typename Vector>
  class cholesky_decomposition
  {

  public:

    using value_type = decltype(SquareMatrix{}[0][0]);

    template<typename SquareMatrix2, typename Vector2>
      cholesky_decomposition(std::size_t n, const SquareMatrix2 & a, Vector2 & d);

    template<typename Vector2, typename VectorOut>
      void backsubstitute(const Vector2 & b, VectorOut & x) const;

    template<typename InVecIter, typename OutVecIter>
      void
      backsubstitution(InVecIter b_begin, InVecIter b_end,
                       OutVecIter x_begin) const;

    template<typename SquareMatrix2>
      void inverse(SquareMatrix2 & a_inv) const;

  private:

    std::size_t m_n;

    SquareMatrix m_a;

    std::vector<value_type> m_d;
  };


/**
 *  
 */
template<typename SquareMatrix, typename Vector>
  void
  cholesky_decomp(std::size_t n, SquareMatrix & a, Vector & d)
  {
    for (std::size_t i = 0; i < n; ++i)
      {
	for (std::size_t j = 0; j < n; ++j)
          {
            auto sum = a[i][j];
            for (std::ptrdiff_t k = i - 1; k >= 0; --k)
              sum -= a[i][k] * a[j][k];
            if (i == j)
              {
        	if (sum <= 0)
                  throw std::logic_error("cholesky_decomp: Failure in matrix");
        	d[i] = std::sqrt(sum);
              }
            else
              a[j][i] = sum / d[i];
          }
      }
  }


/**
 *  
 */
template<typename SquareMatrix, typename Vector>
  void
  cholesky_backsub(std::size_t n, const SquareMatrix & a, const Vector & d, const Vector & b, Vector & x)
  {
    for (std::size_t i = 0; i < n; ++i)
      {
	auto sum = b[i];
	for (std::ptrdiff_t k = i - 1; k >= 0; --k)
          sum -= a[i][k] * x[k];
	x[i] = sum / d[i];
      }
    for (std::ptrdiff_t i = n - 1; i >= 0; --i)
      {
	auto sum = x[i];
	for (std::size_t k = i + 1; k < n; ++k)
          sum -= a[k][i] * x[k];
	x[i] = sum / d[i];
      }
  }


/**
 *  
 */
template<typename SquareMatrix, typename Vector>
  void
  cholesky_invert(std::size_t n, SquareMatrix & a, const Vector & d)
  {
    using NumTp = decltype(SquareMatrix{}[0][0]);

    for (std::size_t i = 0; i < n; ++i)
      {
	a[i][i] = NumTp(1) / d[i];
	for (std::size_t j = i + 1; j < n; ++j)
          {
            auto sum = NumTp(0);
            for (std::size_t k = i; k < j; ++k)
              sum -= a[j][k] * a[k][i];
            a[j][i] = sum / d[j];
          }
      }
  }

}  //  namespace matrix

#endif  //  CHOLESKY_DECOMP_TCC



