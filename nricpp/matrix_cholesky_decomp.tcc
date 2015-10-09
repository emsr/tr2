#ifndef CHOLESKY_DECOMP_TCC
#define CHOLESKY_DECOMP_TCC 1


#include <cstdlib>
#include <stdexcept>
#include <cmath>


namespace matrix
{

template<typename SquareMatrix, typename Vector>
  class cholesky_decomposition
  {

  public:

    using value_type = decltype(SquareMatrix{}[0][0]);

    template<typename SquareMatrix2, typename Vector2>
      cholesky_decomposition(std::size_t n, const SquareMatrix2 & a, Vector2 & d);

    void backsubstitute(const Vector & b, Vector & x) const;

    template<typename SquareMatrix2>
      void inverse(SquareMatrix2 & a_inv) const;

  private:

    std::size_t m_n;
    SquareMatrix & m_a;
    std::vector<value_type> m_d;
  };

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
                  throw std::logic_error("Failure in matrix::cholesky_decomp.");
        	d[i] = std::sqrt(sum);
              }
            else
              a[j][i] = sum / d[i];
          }
      }
  }


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


template<typename SquareMatrix, typename Vector>
  void
  cholesky_invert(std::size_t n, SquareMatrix & a, const Vector & d)
  {
    for (std::size_t i = 0; i < n; ++i)
      {
	a[i][i] = RealTp(1) / d[i];
	for (std::size_t j = i + 1; j < n; ++j)
          {
            auto sum = RealTp(0);
            for (std::size_t k = i; k < j; ++k)
              sum -= a[j][k] * a[k][i];
            a[j][i] = sum / d[j];
          }
      }
  }

}  //  namespace matrix

#endif  //  CHOLESKY_DECOMP_TCC



