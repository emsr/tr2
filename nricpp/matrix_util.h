#if ! defined(MATRIX_UTIL_H)
#define MATRIX_UTIL_H

namespace matrix
{

  template<typename Numeric>
    struct promote
    {
      typedef Numeric type;
    };

  template<>
    struct promote<float>
    {
      typedef double type;
    };

  template<>
    struct promote<double>
    {
      typedef long double type;
    };

  template<typename Numeric>
    using promote_t = typename promote<Numeric>::type;

}

#endif // MATRIX_UTIL_H
