#ifndef SV_DECOMP_TCC
#define SV_DECOMP_TCC


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>


namespace matrix
{


/**
 *  This class represents an singular value decomposition of a matrix.
 */
template<typename NumTp, typename Matrix>
  class sv_decomposition
  {

  public:

    using value_type = decltype(Matrix{}[0][0]);

    template<typename Matrix2>
      sv_decomposition(std::size_t m_n_rows, std::size_t n_cols,
                       Matrix2 & a);

    template<typename Vector2, typename VectorOut>
      void
      backsubstitution(const Vector2 & b, VectorOut & x) const;

    template<typename InVecIter, typename OutVecIter>
      void
      backsubstitution(InVecIter b_begin, InVecIter b_end,
                       OutVecIter x_begin) const;

    template<typename Matrix2, typename Vector2, typename VectorOut>
      void
      improve(const Matrix2 a_orig,
              const Vector2 & b, VectorOut & x) const;

    template<typename Matrix2, typename InVecIter, typename OutVecIter>
      void
      improve(const Matrix2 a_orig,
              InVecIter b_begin, InVecIter b_end,
              OutVecIter x_begin) const;

  private:

    std::size_t m_n_rows;

    std::size_t m_n_cols;

    Matrix m_a;

    std::vector<std::size_t> m_w;

    std::vector<std::size_t> m_v;
  };


/**
 *
 */
template<typename NumTp>
  inline NumTp
  pythag(NumTp a, NumTp b)
  {
    NumTp at, bt, ct;
    return ((at = std::abs(a)) > (bt = std::abs(b)) ? (ct = bt / at, at * std::sqrt(NumTp(1) + ct * ct))
                                              : (bt ? (ct = at / bt, bt * std::sqrt(NumTp(1) + ct * ct)) : 0));
  }


/**
 *  
 */
template<typename Matrix, typename Vector>
  void
  sv_decomp(const std::size_t n_rows, const std::size_t n_cols,
            Matrix & a, Vector & w, Matrix & v)
  {
    using NumTp = std::remove_reference_t<decltype(a[0][0])>;

    int flag, its, j, jj, k, l, nm;
    NumTp c, f, h, s, x, y, z;

    const int ITS = 30;

    std::vector<NumTp> rv1(n_cols);

    NumTp g = NumTp(0);
    NumTp scale = NumTp(0);
    NumTp anorm = NumTp(0);

    //  Householder reduction to bidiagonal form.
    for (std::size_t i = 0; i < n_cols; ++i)
      {
        auto l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = NumTp(0);
        if (i <= n_rows - 1)
          {
            for (std::size_t k = i; k < n_rows; ++k)
              scale += std::abs(a[k][i]);
            if (scale)
              {
                for (std::size_t k = i; k < n_rows; ++k)
                  {
                    a[k][i] /= scale;
                    s += a[k][i] * a[k][i];
                  }
                f = a[i][i];
                g = -copysign(std::sqrt(s),f);
                h = f * g - s;
                a[i][i] = f - g;
                for (std::size_t j = l; j < n_cols; ++j)
                  {
                    s = NumTp(0);
                    for (std::size_t k = i; k < n_rows; ++k)
                      s += a[k][i] * a[k][j];
                    f = s / h;
                    for (std::size_t k = i; k < n_rows; ++k)
                      a[k][j] += f * a[k][i];
                  }
                for (int k = i; k < n_rows; ++k)
                  a[k][i] *= scale;
              }
          }
        w[i] = scale * g;
        g = s = scale = NumTp(0);
        if (i <= n_rows - 1 && i != n_cols - 1)
          {
            for (std::size_t k = l; k < n_cols; ++k)
              scale += std::abs(a[i][k]);
            if (scale)
              {
                for (std::size_t k = l; k < n_cols; ++k)
                  {
                    a[i][k] /= scale;
                    s += a[i][k] * a[i][k];
                  }
                f = a[i][l];
                g = -copysign(std::sqrt(s),f);
                h = f * g - s;
                a[i][l] = f - g;
                for (std::size_t k = l; k < n_cols; ++k)
                  rv1[k] = a[i][k] / h;
                for (std::size_t j = l; j < n_rows; ++j)
                  {
                    s = NumTp(0);
                    for (std::size_t k = l; k < n_cols; ++k)
                      s += a[j][k] * a[i][k];
                    for (std::size_t k = l; k < n_cols; ++k)
                      a[j][k] += s * rv1[k];
                  }
                for (std::size_t k = l; k < n_cols; ++k)
                  a[i][k] *= scale;
              }
          }
        anorm = std::max(anorm, std::abs(w[i]) + std::abs(rv1[i]));
      }

    //  Accumulation of right-hand decomposition V.
    for (std::ptrdiff_t i = n_cols - 1; i >= 0; --i)
      {
        if (i < n_cols - 1)
          {
            if (g)
              {
                for (std::size_t j = l; j < n_cols; ++j)
                  v[j][i] = (a[i][j]/a[i][l])/g;
                for (std::size_t j = l; j < n_cols; ++j)
                  {
                    s = NumTp(0);
                    for (std::size_t k = l; k < n_cols; ++k)
                      s += a[i][k] * v[k][j];
                    for (std::size_t k = l; k < n_cols; ++k)
                      v[k][j] += s * v[k][i];
                  }
              }
            for (std::size_t j = l; j < n_cols; ++j)
              v[i][j] = v[j][i] = NumTp(0);
          }
        v[i][i] = NumTp(1);
        g = rv1[i];
        l = i;
      }

    //  Accumulation of left-hand decompositions.
    for (std::ptrdiff_t i = std::min(n_rows, n_cols) - 1; i >= 0; --i)
      {
        l = i + 1;
        g = w[i];
        for (std::size_t j = l; j < n_cols; ++j)
          a[i][j] = NumTp(0);
        if (g)
          {
            g = NumTp(1) / g;
            for (std::size_t j = l; j < n_cols; ++j)
              {
                s = NumTp(0);
                for (std::size_t k = l; k < n_rows; ++k)
                  s += a[k][i] * a[k][j];
                f = (s / a[i][i]) * g;
                for (std::size_t k = i; k < n_rows; ++k)
                  a[k][j] += f * a[k][i];
              }
            for (std::size_t j = i; j < n_rows; ++j)
              a[j][i] *= g;
          }
        else
          for (std::size_t j = i; j < n_rows; ++j)
            a[j][i] = NumTp(0);
        ++a[i][i];
      }

    //  Diagonalization of the bidiagonal form;
    for (std::ptrdiff_t k = n_cols - 1; k >= 0; --k)
      {
        for (std::size_t its = 1; its <= ITS; ++its)
          {
            bool flag = true;
            std::ptrdiff_t l;
            for (l = k; l >= 0; --l)
              {
                nm = l - 1;
                if (std::abs(rv1[l]) + anorm == anorm)
                  {
                    flag = false;
                    break;
                  }
                if (std::abs(w[nm]) + anorm == anorm)
                  break;
              }
            if (flag)
              {
                auto c = NumTp(0);
                auto s = NumTp(1);
                for (std::size_t i = l; i < k; ++i)
                  {
                    auto f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if (std::abs(f) + anorm == anorm)
                      break;
                    auto g = w[i];
                    auto h = pythag(f, g);
                    w[i] = h;
                    h = NumTp(1) / h;
                    c = g * h;
                    s = -f * h;
                    for (std::size_t j = 0; j < n_rows; ++j)
                      {
                        auto y = a[j][nm];
                        auto z = a[j][i];
                        a[j][nm] = y * c + z * s;
                        a[j][i] = z * c - y * s;
                      }
                  }
              }
            z = w[k];
            if (l == k)
              {
                //  Convergence!!!
                if (z < NumTp(0))
                  {
                    //  Make singular value non negative.
                    w[k] = -z;
                    for (std::size_t j = 0; j < n_cols; ++j)
                      v[j][k] = -v[j][k];
                  }
                break;
              }
            if (its == ITS)
              {
                std::ostringstream msg;
                msg << "No convergence in " << ITS << " sv_decomp iterations.";
                throw std::logic_error(msg);
              }

            //  Shift from bottom 2x2 minor.
            x = w[l];
            nm = k - 1;
            y = w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2 * h * y);
            g = pythag(f, NumTp(1));
            f = ((x - z) * (x + z) + h * ((y / (f + copysign(g, f))) - h)) / x;

            //  Next QR transformation.
            c = s = NumTp(1);
            for (std::size_t j = l; j <= nm; ++j)
              {
                auto i = j + 1;
                g = rv1[i];
                y = w[i];
                h = s * g;
                g = c * g;
                z = pythag(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y *= c;
                for (std::size_t jj = 0; jj < n_cols; ++jj)
                  {
                    x = v[jj][j];
                    z = v[jj][i];
                    v[jj][j] = x * c + z * s;
                    v[jj][i] = z * c - x * s;
                  }
                z = pythag(f, h);
                w[j] = z;
                //  Rotation can be arbitrary if z = 0.
                if (z)
                  {
                    z = NumTp(1) / z;
                    c = f * z;
                    s = h * z;
                  }
                f = c * g + s * y;
                x = c * y - s * g;
                for (std::size_t jj = 0; jj < n_rows; ++jj)
                  {
                    auto y = a[jj][j];
                    auto z = a[jj][i];
                    a[jj][j] = y * c + z * s;
                    a[jj][i] = z * c - y * s;
                  }
              }
            rv1[l] = NumTp(0);
            rv1[k] = f;
            w[k] = x;
          }
      }

    return;
  }


/**
 *  
 */
template<typename Matrix, typename Vector>
  void
  sv_backsub(std::size_t n_rows, std::size_t n_cols,
             const Matrix & u,
             const Vector & w, const Matrix & v,
             const Vector & b, Vector & x)
  {
    using NumTp = std::remove_reference_t<decltype(u[0][0])>;

    std::vector<NumTp> tmp(n_cols);

    for (std::size_t j = 0; j < n_cols; ++j)
      {
        NumTp s = NumTp(0);
        if (w[j] != NumTp(0))
          {
            for (std::size_t i = 0; i < n_rows; ++i)
              s += u[i][j] * b[i];
            s /= w[j];
          }
        tmp[j] = s;
      }
    for (std::size_t j = 0; j < n_cols; ++j)
      {
        NumTp s = NumTp(0);
        for (std::size_t jj = 0; jj < n_cols; ++jj)
          s += v[j][jj] * tmp[jj];
        x[j] = s;
      }

    return;
  }



/**
 *  Improves a solution vector x of the linear set A.x = b.
 *  The matrix a and the SV decomposition of a -- u, w, v and the
 *  right-hand side vector are input along with the solution vector x.
 *  The solution vector x is improved and modified on output.
 */
template<typename Matrix, typename Vector>
  void
  sv_improve(std::size_t n_rows, std::size_t n_cols,
             const Matrix & a, const Matrix & u,
             const Vector & w, const Matrix & v,
             const Vector & b, Vector & x)
  {
    using NumTp = std::remove_reference_t<decltype(a[0][0])>;

    std::vector<NumTp> r(n_rows);
    std::vector<NumTp> dx(n_cols);

    for (std::size_t i = 0; i < n_rows; ++i)
      {
        r[i] = -b[i];
        for (std::size_t j = 0; j < n_cols; ++j)
          r[i] += a[i][j] * x[j];
      }

    sv_backsub(n_rows, n_cols, u, w, v, r, dx);

    for (std::size_t i = 0; i < n_cols; ++i)
      x[i] -= dx[i];

    return;
  }

}  //  namespace matrix


#endif  //  SV_DECOMP_TCC
