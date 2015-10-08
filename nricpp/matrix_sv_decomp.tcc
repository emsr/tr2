#ifndef SV_DECOMP_TCC
#define SV_DECOMP_TCC


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>


namespace matrix
{

template<typename Type>
  inline Type
  pythag( Type a, Type b )
  {
    Type at, bt, ct;
    return ((at = std::abs(a)) > (bt = std::abs(b)) ? (ct = bt / at, at * std::sqrt(Type(1) + ct * ct))
                                              : (bt ? (ct = at / bt, bt * std::sqrt(Type(1) + ct * ct)) : 0));
  }

template<typename Matrix, typename Vector>
  void
  sv_decomp(const std::size_t m, const std::size_t n,
            Matrix& a, Vector& w, Matrix& v)
  {
    using Type = std::remove_reference_t<decltype(a[0][0])>;

    int flag, its, j, jj, k, l, nm;
    Type c, f, h, s, x, y, z;

    const int ITS = 30;

    std::vector<Type> rv1(n);

    Type g = Type(0);
    Type scale = Type(0);
    Type anorm = Type(0);

    //  Householder reduction to bidiagonal form.
    for (std::size_t i = 0; i < n; ++i)
      {
        auto l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = Type(0);
        if (i <= m - 1)
          {
            for (std::size_t k = i; k < m; ++k)
              scale += std::abs(a[k][i]);
            if (scale)
              {
                for (std::size_t k = i; k < m; ++k)
                  {
                    a[k][i] /= scale;
                    s += a[k][i] * a[k][i];
                  }
                f = a[i][i];
                g = -copysign(std::sqrt(s),f);
                h = f * g - s;
                a[i][i] = f - g;
                for (std::size_t j = l; j < n; ++j)
                  {
                    s = Type(0);
                    for (std::size_t k = i; k < m; ++k)
                      s += a[k][i] * a[k][j];
                    f = s/h;
                    for (std::size_t k = i; k < m; ++k)
                      a[k][j] += f * a[k][i];
                  }
                for (int k = i; k < m; ++k)
                  a[k][i] *= scale;
              }
          }
        w[i] = scale * g;
        g = s = scale = Type(0);
        if (i <= m - 1 && i != n - 1)
          {
            for (std::size_t k = l; k < n; ++k)
              scale += std::abs(a[i][k]);
            if (scale)
              {
                for (std::size_t k = l; k < n; ++k)
                  {
                    a[i][k] /= scale;
                    s += a[i][k] * a[i][k];
                  }
                f = a[i][l];
                g = -copysign(std::sqrt(s),f);
                h = f * g - s;
                a[i][l] = f - g;
                for (std::size_t k = l; k < n; ++k)
                  rv1[k] = a[i][k]/h;
                for (std::size_t j = l; j < m; ++j)
                  {
                    s = Type(0);
                    for (std::size_t k = l; k < n; ++k)
                      s += a[j][k] * a[i][k];
                    for (std::size_t k = l; k < n; ++k)
                      a[j][k] += s * rv1[k];
                  }
                for (std::size_t k = l; k < n; ++k)
                  a[i][k] *= scale;
              }
          }
        anorm = std::max(anorm, std::abs(w[i]) + std::abs(rv1[i]));
      }

    //  Accumulation of right-hand decomposition V.
    for (std::ptrdiff_t i = n - 1; i >= 0; --i)
      {
        if (i < n - 1)
          {
            if (g)
              {
                for (std::size_t j = l; j < n; ++j)
                  v[j][i] = (a[i][j]/a[i][l])/g;
                for (std::size_t j = l; j < n; ++j)
                  {
                    s = Type(0);
                    for (std::size_t k = l; k < n; ++k)
                      s += a[i][k] * v[k][j];
                    for (std::size_t k = l; k < n; ++k)
                      v[k][j] += s * v[k][i];
                  }
              }
            for (std::size_t j = l; j < n; ++j)
              v[i][j] = v[j][i] = Type(0);
          }
        v[i][i] = Type(1);
        g = rv1[i];
        l = i;
      }

    //  Accumulation of left-hand decompositions.
    for (std::ptrdiff_t i = std::min(m, n) - 1; i >= 0; --i)
      {
        l = i + 1;
        g = w[i];
        for (std::size_t j = l; j < n; ++j)
          a[i][j] = Type(0);
        if (g)
          {
            g = Type(1) / g;
            for (std::size_t j = l; j < n; ++j)
              {
                s = Type(0);
                for (std::size_t k = l; k < m; ++k)
                  s += a[k][i] * a[k][j];
                f = (s/a[i][i]) * g;
                for (std::size_t k = i; k < m; ++k)
                  a[k][j] += f * a[k][i];
              }
            for (std::size_t j = i; j < m; ++j)
              a[j][i] *= g;
          }
        else
          for (std::size_t j = i; j < m; ++j)
            a[j][i] = Type(0);
        ++a[i][i];
      }

    //  Diagonalization of the bidiagonal form;
    for (std::ptrdiff_t k = n - 1; k >= 0; --k)
      {
        for (std::size_t its = 1; its <= ITS; ++its)
          {
            bool flag = 1;
            std::ptrdiff_t l;
            for (l = k; l >= 0; --l)
              {
                nm = l - 1;
                if (std::abs(rv1[l]) + anorm == anorm)
                  {
                    flag = 0;
                    break;
                  }
                if (std::abs(w[nm]) + anorm == anorm)
                  break;
              }
            if (flag)
              {
                auto c = Type(0);
                auto s = Type(1);
                for (std::size_t i = l; i < k; ++i)
                  {
                    auto f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if (std::abs(f) + anorm == anorm)
                      break;
                    auto g = w[i];
                    auto h = pythag(f, g);
                    w[i] = h;
                    h = Type(1) / h;
                    c = g * h;
                    s = -f * h;
                    for (std::size_t j = 0; j < m; ++j)
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
                if (z < Type(0))
                  {
                    //  Make singular value non negative.
                    w[k] = -z;
                    for (std::size_t j = 0; j < n; ++j)
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
            g = pythag(f, Type(1));
            f = ((x - z) * (x + z) + h * ((y / (f + copysign(g, f))) - h)) / x;

            //  Next QR transformation.
            c = s = Type(1);
            for (std::size_t j = l; j <= nm; ++j)
              {
                std::size_t i = j + 1;
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
                for (std::size_t jj = 0; jj < n; ++jj)
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
                    z = Type(1) / z;
                    c = f * z;
                    s = h * z;
                  }
                f = c * g + s * y;
                x = c * y - s * g;
                for (std::size_t jj = 0; jj < m; ++jj)
                  {
                    auto y = a[jj][j];
                    auto z = a[jj][i];
                    a[jj][j] = y * c + z * s;
                    a[jj][i] = z * c - y * s;
                  }
              }
            rv1[l] = Type(0);
            rv1[k] = f;
            w[k] = x;
          }
      }

    return;
  }




template<typename Matrix, typename Vector>
  void
  sv_backsub(std::size_t m, std::size_t n,
             const Matrix & u, const Vector & w,
             const Matrix & v, const Vector & b, Vector & x)
  {
    using Type = std::remove_reference_t<decltype(u[0][0])>;

    std::vector<Type> tmp(n);

    for (std::size_t j = 0; j < n; ++j)
      {
        Type s = Type(0);
        if (w[j] != Type(0))
          {
            for (std::size_t i = 0; i < m; ++i)
              s += u[i][j] * b[i];
            s /= w[j];
          }
        tmp[j] = s;
      }
    for (std::size_t j = 0; j < n; ++j)
      {
        Type s = Type(0);
        for (std::size_t jj = 0; jj < n; ++jj)
          s += v[j][jj] * tmp[jj];
        x[j] = s;
      }

    return;
  }



//
//  Improves a solution vector x of the linear set A.x = b.  The matrix a and the
//  SV decomposition of a -- u, w, v and the
//  right-hand side vector are input along with the solution vector x.
//  The solution vector x is improved and modified on output.
//
template<typename Matrix, typename Vector>
  void
  sv_improve(std::size_t m, std::size_t n,
             const Matrix & a, const Matrix & u, const Vector & w,
             const Matrix & v, const Vector & b, Vector & x)
  {
    using Type = std::remove_reference_t<decltype(a[0][0])>;

    std::vector<Type> r(m);
    std::vector<Type> dx(n);

    for (std::size_t i = 0; i < m; ++i)
      {
        r[i] = -b[i];
        for (std::size_t j = 0; j < n; ++j)
          r[i] += a[i][j] * x[j];
      }

    sv_backsub(m, n, u, w, v, r, dx);

    for (std::size_t i = 0; i < n; ++i)
      x[i] -= dx[i];

    return;
  }

}  //  namespace matrix


#endif  //  SV_DECOMP_TCC
