#ifndef HANKEL_H
#define HANKEL_H 1

template<typename _Tp>
  std::complex<_Tp>
  hankel_h1(std::complex<_Tp> __nu, std::complex<_Tp> __z)
  {
    std::complex<_Tp> __h1, __h1p, __h2, __h2p;
    int __error;
    hankel(__nu, __z, __h1, __h1p, __h2, __h2p, __error);
    return __h1;
  }

template<typename _Tp>
  std::complex<_Tp>
  hankel_h2(std::complex<_Tp> __nu, std::complex<_Tp> __z)
  {
    std::complex<_Tp> __h1, __h1p, __h2, __h2p;
    int __error;
    hankel(__nu, __z, __h1, __h1p, __h2, __h2p, __error);
    return __h2;
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
#endif // HANKEL_H
