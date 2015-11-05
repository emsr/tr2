#ifndef HANKEL_H
#define HANKEL_H 1

template<typename _Tp>
  std::complex<_Tp>
  cyl_hankel_h1(std::complex<_Tp> __nu, std::complex<_Tp> __z)
  {
    std::complex<_Tp> __h1, __h1p, __h2, __h2p;
    int __error;
    hankel(__nu, __z, __h1, __h1p, __h2, __h2p, __error);
    return __h1;
  }

template<typename _Tp>
  std::complex<_Tp>
  cyl_hankel_h2(std::complex<_Tp> __nu, std::complex<_Tp> __z)
  {
    std::complex<_Tp> __h1, __h1p, __h2, __h2p;
    int __error;
    hankel(__nu, __z, __h1, __h1p, __h2, __h2p, __error);
    return __h2;
  }

template<typename _Tp>
  std::complex<_Tp>
  sph_hankel_h1(int __n, std::complex<_Tp> __z)
  {
    std::complex<_Tp> __nu(n + _Tp(0.5));
    std::complex<_Tp> __h1, __h1p, __h2, __h2p;
    int __error;
    hankel(__nu, __z, __h1, __h1p, __h2, __h2p, __error);
    return std::sqrt(__pi / (2 * __z)) * __h1;
  }

template<typename _Tp>
  std::complex<_Tp>
  sph_hankel_h2(int __n, std::complex<_Tp> __z)
  {
    std::complex<_Tp> __nu(n + _Tp(0.5));
    std::complex<_Tp> __h1, __h1p, __h2, __h2p;
    int __error;
    hankel(__nu, __z, __h1, __h1p, __h2, __h2p, __error);
    return std::sqrt(__pi / (2 * __z)) * __h2;
  }

template<typename _Tp>
  std::complex<_Tp>
  airy_ai(std::complex<_Tp> __z)
  {
    std::complex<_Tp> __ai, __aip;
    int __error;
    airy(__z, __ai, __aip, __error);
    return __ai;
  }

//template<typename _Tp>
//  std::complex<_Tp>
//  airy_bi(std::complex<_Tp> __z)
//  {
//    std::complex<_Tp> __ai, __aip, __bi, __bip;
//    int __error;
//    airy(__z, __ai, __aip, __bi, __bip, __error);
//    return __bi;
//  }

#endif // HANKEL_H
