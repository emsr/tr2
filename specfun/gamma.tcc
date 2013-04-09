// Special functions -*- C++ -*-

// Copyright (C) 2006-2013 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file tbd/gamma.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{tbd/cmath}
 */

#ifndef _GLIBCXX_GAMMA_TCC
#define _GLIBCXX_GAMMA_TCC 1


///  We'll pull this into the main (TR1) gamma.tcc ultimately.

namespace __detail {

/**
 *   @brief This returns Bernoulli numbers from a table or by summation
 *          for larger values.
 *
 *   Recursion is unstable.
 *
 *   @param __n the order n of the Bernoulli number.
 *   @return  The Bernoulli number of order n.
 */
template <typename _Tp>
  _Tp
  __bernoulli_series(unsigned int __n)
  {

    static const _Tp __num[28] = {
      _Tp(1UL),                        -_Tp(1UL) / _Tp(2UL),
      _Tp(1UL) / _Tp(6UL),             _Tp(0UL),
      -_Tp(1UL) / _Tp(30UL),           _Tp(0UL),
      _Tp(1UL) / _Tp(42UL),            _Tp(0UL),
      -_Tp(1UL) / _Tp(30UL),           _Tp(0UL),
      _Tp(5UL) / _Tp(66UL),            _Tp(0UL),
      -_Tp(691UL) / _Tp(2730UL),       _Tp(0UL),
      _Tp(7UL) / _Tp(6UL),             _Tp(0UL),
      -_Tp(3617UL) / _Tp(510UL),       _Tp(0UL),
      _Tp(43867UL) / _Tp(798UL),       _Tp(0UL),
      -_Tp(174611) / _Tp(330UL),       _Tp(0UL),
      _Tp(854513UL) / _Tp(138UL),      _Tp(0UL),
      -_Tp(236364091UL) / _Tp(2730UL), _Tp(0UL),
      _Tp(8553103UL) / _Tp(6UL),       _Tp(0UL)
    };

    if (__n == 0)
      return _Tp(1);

    if (__n == 1)
      return -_Tp(1) / _Tp(2);

    //  Take care of the rest of the odd ones.
    if (__n % 2 == 1)
      return _Tp(0);

    //  Take care of some small evens that are painful for the series.
    if (__n < 28)
      return __num[__n];


    _Tp __fact = _Tp(1);
    if ((__n / 2) % 2 == 0)
      __fact *= _Tp(-1);
    for (unsigned int __k = 1; __k <= __n; ++__k)
      __fact *= __k / (_Tp(2) * __numeric_constants<_Tp>::__pi());
    __fact *= _Tp(2);

    _Tp __sum = _Tp(0);
    for (unsigned int __i = 1; __i < 1000; ++__i)
      {
        _Tp __term = std::pow(_Tp(__i), -_Tp(__n));
        if (__term < std::numeric_limits<_Tp>::epsilon())
          break;
        __sum += __term;
      }

    return __fact * __sum;
  }


/**
 *   @brief This returns Bernoulli number \f$B_n\f$.
 *
 *   @param __n the order n of the Bernoulli number.
 *   @return  The Bernoulli number of order n.
 */
template<typename _Tp>
  inline _Tp
  __bernoulli(int __n)
  { return __bernoulli_series<_Tp>(__n); }


template<typename _Tp>
  std::pair<_Tp, _Tp>
  __gamma_series(_Tp __a, _Tp __x)
  {
    const double __eps = 3.0 * std::numeric_limits<_Tp>::epsilon();
    const unsigned int __itmax = 10 * (10 + std::sqrt(std::abs(__a)));

    _Tp __lngam = std::lgamma(__a);

    if (__x < _Tp(0))
      throw std::domain_error("Argument less than 0 in routine gamma_series().");
    else if (__x == _Tp(0))
      return std::make_pair(_Tp(0), __lngam);
    else
      {
        _Tp __aa = __a;
        _Tp __term, __sum;
        __term = __sum = _Tp(1) / __a;
        for (unsigned int __n = 1; __n <= __itmax; ++__n)
          {
            __aa += _Tp(1);
            __term *= __x / __aa;
            __sum += __term;
            if (std::abs(__term) < __eps * std::abs(__sum))
              {
                _Tp __gamser = std::exp(-__x + __a * std::log(__x) - __lngam) * __sum;
                return std::make_pair(__gamser, __lngam);
              }
          }
        throw std::logic_error("__gamma_series: a too large, itmax too small in routine.");
      }
  }


template<typename _Tp>
  std::pair<_Tp, _Tp>
  __gamma_cont_frac(_Tp __a, _Tp __x)
  {
    const _Tp __fpmin = 3.0 * std::numeric_limits<_Tp>::min();
    const _Tp __eps = 3.0 * std::numeric_limits<_Tp>::epsilon();
    const unsigned int __itmax = 10 * (10 + std::sqrt(std::abs(__a)));

    _Tp __lngam = std::lgamma(__a);

    _Tp __b = __x + _Tp(1) - __a;
    _Tp __c = _Tp(1) / __fpmin;
    _Tp __d = _Tp(1) / __b;
    _Tp __h = __d;
    for (unsigned int __n = 1; __n <= __itmax; ++__n)
      {
        _Tp __an = -_Tp(__n) * (_Tp(__n) - __a);
        __b += _Tp(2);
        __d = __an * __d + __b;
        if (std::abs(__d) < __fpmin)
          __d = __fpmin;
        __c = __b + __an / __c;
        if (std::abs(__c) < __fpmin)
          __c = __fpmin;
        __d = _Tp(1) / __d;
        _Tp __del = __d * __c;
        __h *= __del;
        if (std::abs(__del - _Tp(1)) < __eps)
          {
            _Tp __gamcf = std::exp(-__x + __a * std::log(__x) - __lngam) * __h;
            return std::make_pair(__gamcf, __lngam);
          }
      }
    throw std::logic_error("__gamma_cont_fraction: a too large, itmax too small in routine.");
  }


template<typename _Tp>
  _Tp
  __gamma_p(_Tp __a, _Tp __x)
  {
    if (__x < 0.0 || __a <= 0.0)
      throw std::domain_error("Invalid arguments in routine gamma_p()");

    if (__x < __a + _Tp(1))
      return __gamma_series(__a, __x).first;
    else
      return _Tp(1) - __gamma_cont_frac(__a, __x).first;
  }


template<typename _Tp>
  _Tp
  __gamma_q(_Tp __a, _Tp __x)
  {
    if (__x < 0.0 || __a <= 0.0)
      throw std::domain_error("Invalid arguments in routine gamma_q().");

    if (__x < __a + _Tp(1))
      return _Tp(1) - __gamma_series(__a, __x).first;
    else
      return __gamma_cont_frac(__a, __x).first;
  }


/**
 *   @brief  Return the lower incomplete gamma function.
 *   The lower incomplete gamma function is defined by
 *   @f[
 *     \gamma(a,x) = \int_0^x e^{-t}t^{a-1}dt  (a > 0)
 *   @f]
 */
template<typename _Tp>
  _Tp
  __gamma_l(_Tp __a, _Tp __x)
  {
    if (__x < 0.0 || __a <= 0.0)
      throw std::domain_error("Invalid arguments in routine gamma_l()");

    if (__x < __a + _Tp(1))
    {
      std::pair<_Tp, _Tp> __gp = __gamma_series(__a, __x);
      return std::exp(__gp.second) * __gp.first;
    }
    else
    {
      std::pair<_Tp, _Tp> __gp = __gamma_cont_frac(__a, __x);
      return std::exp(__gp.second) * (_Tp(1) - __gp.first);
    }
  }


/**
 *   @brief  Return the lower incomplete gamma function.
 *   The lower incomplete gamma function is defined by
 *   @f[
 *     \gamma(a,x) = \int_0^x e^{-t}t^{a-1}dt  (a > 0)
 *   @f]
 */
template<typename _Tp>
  _Tp
  __gamma_u(_Tp __a, _Tp __x)
  {
    if (__x < 0.0 || __a <= 0.0)
      throw std::domain_error("Invalid arguments in routine gamma_u()");

    if (__x < __a + _Tp(1))
    {
        std::pair<_Tp, _Tp> __gp = __gamma_series(__a, __x);
        return std::exp(__gp.second) * (_Tp(1) - __gp.first);
    }
    else
    {
        std::pair<_Tp, _Tp> __gp = __gamma_cont_frac(__a, __x);
        return std::exp(__gp.second) * __gp.first;
    }
  }


template<typename _Tp>
  _Tp
  __log_pochhammer_u(_Tp __n, _Tp __x)
  {
    if (__isnan(__n) || __isnan(__x))
      return std::numeric_limits<_Tp>::quiet_NaN();
    else if (__n == _Tp(0))
      return _Tp(0);
    else
      return std::lgamma(__x + __n) - std::lgamma(__x);
  }


template<typename _Tp>
  _Tp
  __pochhammer_u(_Tp __n, _Tp __x)
  {
    static const _Tp __log10(2.3025850929940456840179914546843642L);
    if (__isnan(__n) || __isnan(__x))
      return std::numeric_limits<_Tp>::quiet_NaN();
    else if (__n == _Tp(0))
      return _Tp(1);
    else
      {
        _Tp __logpoch = std::lgamma(__x + __n) - std::lgamma(__x);
        if (std::abs(__logpoch)
            > std::numeric_limits<_Tp>::max_digits10 * __log10)
          return std::numeric_limits<_Tp>::infinity();
        else
          return std::exp(__logpoch);
      }
  }


template<typename _Tp>
  _Tp
  __log_pochhammer_l(_Tp __n, _Tp __x)
  {
    if (__isnan(__n) || __isnan(__x))
      return std::numeric_limits<_Tp>::quiet_NaN();
    else if (__n == _Tp(0))
      return _Tp(0);
    else
      return std::lgamma(__x + 1) - std::lgamma(__x - __n + 1);
  }


template<typename _Tp>
  _Tp
  __pochhammer_l(_Tp __n, _Tp __x)
  {
    static const _Tp __log10(2.3025850929940456840179914546843642L);
    if (__isnan(__n) || __isnan(__x))
      return std::numeric_limits<_Tp>::quiet_NaN();
    else if (__n == _Tp(0))
      return _Tp(1);
    else
      {
        _Tp __logpoch = std::lgamma(__x + 1) - std::lgamma(__x - __n + 1);
        if (std::abs(__logpoch)
            > std::numeric_limits<_Tp>::max_digits10 * __log10)
          return std::numeric_limits<_Tp>::infinity();
        else
          return std::exp(__logpoch);
      }
  }


/**
 *   @brief Return the logarithm of the binomial coefficient.
 *   The binomial coefficient is given by:
 *   @f[
 *   \left(  \right) = \frac{n!}{(n-k)! k!}
 *   @f]
 *
 *   @param __n The first argument of the binomial coefficient.
 *   @param __k The second argument of the binomial coefficient.
 *   @return  The logarithm of the binomial coefficient.
 */
template<typename _Tp>
  _Tp
  __log_bincoef(unsigned int __n, unsigned int __k)
  {
    _Tp __coeff = std::lgamma(_Tp(1 + __n))
                - std::lgamma(_Tp(1 + __k))
                - std::lgamma(_Tp(1 + __n - __k));
  }


/**
 *   @brief Return the binomial coefficient.
 *   The binomial coefficient is given by:
 *   @f[
 *   \left(  \right) = \frac{n!}{(n-k)! k!}
 *   @f]
 *
 *   @param __n The first argument of the binomial coefficient.
 *   @param __k The second argument of the binomial coefficient.
 *   @return  The binomial coefficient.
 */
template<typename _Tp>
  _Tp
  __bincoef(unsigned int __n, unsigned int __k)
  {
    //  Max e exponent before overflow.
    static const _Tp __max_bincoeff
                    = std::numeric_limits<_Tp>::max_exponent10
                    * std::log(_Tp(10)) - _Tp(1);

    const _Tp __log_coeff = __log_bincoef<_Tp>(__n, __k);
    if (__log_coeff > __max_bincoeff)
      return std::numeric_limits<_Tp>::quiet_NaN();
    else
      return std::exp(__log_coeff);
  }


/**
 *   @brief  Return the digamma function by series expansion.
 *   The digamma or @f$ \psi(x) @f$ function is defined by
 *   @f[
 *     \psi(x) = \frac{\Gamma'(x)}{\Gamma(x)}
 *   @f]
 *
 *   The series is given by:
 *   @f[
 *     \psi(x) = -\gamma_E - \frac{1}{x}
 *              \sum_{k=1}^{\infty} \frac{x}{k(x + k)}
 *   @f]
 */
template<typename _Tp>
  _Tp
  __psi_series(_Tp __x)
  {
    _Tp __sum = -__numeric_constants<_Tp>::__gamma_e() - _Tp(1) / __x;
    const unsigned int __max_iter = 100000;
    for (unsigned int __k = 1; __k < __max_iter; ++__k)
      {
        const _Tp __term = __x / (__k * (__k + __x));
        __sum += __term;
        if (std::abs(__term / __sum) < std::numeric_limits<_Tp>::epsilon())
          break;
      }
    return __sum;
  }


/**
 *   @brief  Return the digamma function for large argument.
 *   The digamma or @f$ \psi(x) @f$ function is defined by
 *   @f[
 *     \psi(x) = \frac{\Gamma'(x)}{\Gamma(x)}
 *   @f]
 *
 *   The asymptotic series is given by:
 *   @f[
 *     \psi(x) = \ln(x) - \frac{1}{2x}
 *             - \sum_{n=1}^{\infty} \frac{B_{2n}}{2 n x^{2n}}
 *   @f]
 */
template<typename _Tp>
  _Tp
  __psi_asymp(_Tp __x)
  {
    _Tp __sum = std::log(__x) - _Tp(0.5L) / __x;
    const _Tp __xx = __x * __x;
    _Tp __xp = __xx;
    const unsigned int __max_iter = 100;
    for (unsigned int __k = 1; __k < __max_iter; ++__k)
      {
        const _Tp __term = __bernoulli<_Tp>(2 * __k) / (2 * __k * __xp);
        __sum -= __term;
        if (std::abs(__term / __sum) < std::numeric_limits<_Tp>::epsilon())
          break;
        __xp *= __xx;
      }
    return __sum;
  }


/**
 *   @brief  Return the digamma function.
 *   The digamma or @f$ \psi(x) @f$ function is defined by
 *   @f[
 *     \psi(x) = \frac{\Gamma'(x)}{\Gamma(x)}
 *   @f]
 *   For negative argument the reflection formula is used:
 *   @f[
 *     \psi(x) = \psi(1-x) - \pi \cot(\pi x)
 *   @f]
 */
template<typename _Tp>
  _Tp
  __psi(_Tp __x)
  {
    const int __n = static_cast<int>(__x + 0.5L);
    const _Tp __eps = _Tp(4) * std::numeric_limits<_Tp>::epsilon();
    if (__n <= 0 && std::abs(__x - _Tp(__n)) < __eps)
      return std::numeric_limits<_Tp>::quiet_NaN();
    else if (__x < _Tp(0))
      {
        const _Tp __pi = __numeric_constants<_Tp>::__pi();
        return __psi(_Tp(1) - __x)
             - __pi * std::cos(__pi * __x) / std::sin(__pi * __x);
      }
    else if (__x > _Tp(100))
      return __psi_asymp(__x);
    else
      return __psi_series(__x);
  }


/**
 *   @brief  Return the polygamma function @f$ \psi^{(n)}(x) @f$.
 * 
 *   The polygamma function is related to the Hurwitz zeta function:
 *   @f[
 *     \psi^{(n)}(x) = (-1)^{n+1} m! \zeta(m+1,x)
 *   @f]
 */
template<typename _Tp>
  _Tp
  __psi(unsigned int __n, _Tp __x)
  {
    if (__x <= _Tp(0))
      std::__throw_domain_error(__N("Argument out of range "
                                    "in __psi"));
    else if (__n == 0)
      return __psi(__x);
    else
      {
        const _Tp __hzeta = __hurwitz_zeta(_Tp(__n + 1), __x);
        const _Tp __ln_nfact = std::lgamma(_Tp(__n + 1));
        _Tp __result = std::exp(__ln_nfact) * __hzeta;
        if (__n % 2 == 1)
          __result = -__result;
        return __result;
      }
  }


} // namespace __detail

#endif // _GLIBCXX_GAMMA_TCC
