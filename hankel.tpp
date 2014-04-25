#include <complex>
#include <iostream>
#include <limits>
/**
     Purpose
       This routine uses the old uniform_hankel subroutine of
       Whitaker (now called uniform_hankel_olver) to compute the
       uniform asymptotic approximations of the Hankel functions
       and their derivatives including a patch for the case when
       the order equals or nearly equals the argument.  At such 
       points, Olver's expressions have zero denominators (and
       numerators) resulting in numerical problems.  This routine
       averages four surrounding points in the complex order plane
       to obtain the result in such cases.
//
//  @param[in]  nu  The order for which the Hankel functions are to be evaluated.
//  @param[in]  z   The argument at which the Hankel functions are to be evaluated.
//  @param[out] h1  The computed value for the Hankel function of the first kind.
//  @param[out] h1p The computed value for the derivative of the Hankel function of the first kind.
//  @param[out] h2  The computed value for the Hankel function of the second kind.
//  @param[out] h2p The computed value for the derivative of the Hankel function of the second kind.
 */
template<typename _Tp>
  void
  uniform_hankel(std::complex<_Tp> & h1, std::complex<_Tp> & h2,
                 std::complex<_Tp> & h1p, std::complex<_Tp> & h2p,
                 std::complex<_Tp> z, std::complex<_Tp> nu)
  {
    _Tp test = std::pow(std::abs(nu), 1.0/3.0) / 5.0;

    if (std::abs(z - nu) > test)
      uniform_hankel_olver(h1, h2, h1p, h2p, z, nu);
    else
    {
      _Tp r = 2 * test;
      std::complex<_Tp> anu[4]{nu + std::complex<_Tp>{r, 0},
                               nu + std::complex<_Tp>{0, r},
                               nu - std::complex<_Tp>{r, 0},
                               nu - std::complex<_Tp>{0, r}};

      for (auto tnu : anu)
      {
        std::complex<_Tp> th1, th2, th1p, th2p;
        uniform_hankel_olver(th1, th2, th1p, th2p, z, tnu);
        h1  += th1;
        h2  += th2;
        h1p += th1p;
        h2p += th2p;
      }
      h1  /= _Tp(4);
      h2  /= _Tp(4);
      h1p /= _Tp(4);
      h2p /= _Tp(4);
    }

    return;
  }



/**
 *  Use the uniform asymptotic expansion to compute approximate
 *  values for the Hankel functions of the first and second kinds
 *  of order @c nu along with their derivatives.
 *
 *  @param[in]  nu  The order for which the Hankel functions are to be evaluated.
 *  @param[in]  z   The argument at which the Hankel functions are to be evaluated.
 *  @param[out] h1  The computed value for the Hankel function of the first kind.
 *  @param[out] h1p The computed value for the derivative of the Hankel function of the first kind.
 *  @param[out] h2  The computed value for the Hankel function of the second kind.
 *  @param[out] h2p The computed value for the derivative of the Hankel function of the second kind.
 */
template<typename _Tp>
  void
  uniform_hankel_olver(std::complex<_Tp> & h1, std::complex<_Tp> & h2,
                       std::complex<_Tp> & h1p, std::complex<_Tp> & h2p,
                       std::complex<_Tp> z, std::complex<_Tp> nu)
  {
    using namespace std::literals::complex_literals;

    std::complex<_Tp> t, tsq,
                      _1dnsq, etm3h, aip, o4dp, aim, o4dm,
                      od2p, od0dp, od0dm, tmp, zhat, nm1d3,
                      nm2d3, etrat, od2m, wksp(50), r_factor;
    int ier, nwksp = 50; // > some function of nterms

    static const _Tp pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;
    static const _Tp pi_3 = 1.0471'97551'19659'77461'54214'46109'31676'28063e+0L;
    static const std::complex<_Tp> con1p{ 1.0, 1.732050807568877}; // 2*exp(dpi*i/3)
    static const std::complex<_Tp> con1m{ 1.0,-1.732050807568877}; // 2*exp(-dpi*i/3)
    static const std::complex<_Tp> con2p{-2.0, 3.464101615137755}; // 4*exp(2*dpi*i/3)
    static const std::complex<_Tp> con2m{-2.0,-3.464101615137755}; // 4*exp(-2*dpi*i/3)
    static const _Tp eps = 1.0e-06;
    static const _Tp epsai = 1.0e-12;
    int nterms = 4;

    //  Extended to accommodate negative real orders.
    bool nuswitch = false;
    if (std::real(nu) < _Tp(0))
    {
      nuswitch = true;
      nu = -nu;
    }

    // Compute outer factors in the uniform asymptotic expansions
    // for the Hankel functions and their derivatives along with
    // other important functions of nu and z.
    uniform_hankel_outer(nu, z, epsai, zhat, _1dnsq, nm1d3, nm2d3, t, tsq,
                         etm3h, etrat, aip, o4dp, aim, o4dm, od2p,
                         od0dp, od2m, od0dm, ier);

    // Check for successful completion
    if (ier == 0)
    {
      // Compute further terms in the expansions in their appropriate linear combinations.

      uniform_hankel_sum(t, tsq, _1dnsq, etm3h, aip, o4dp, aim, o4dm,
                         od2p, od0dp, od2m, od0dm, eps, nterms, wksp,
                         nwksp, h1, h1p, h2, h2p, ier);

      // Assemble approximations.
      tmp = etrat * nm1d3;
      h1 = con1m * tmp * h1;
      h2 = con1p * tmp * h2;
      tmp = nm2d3 / (zhat * etrat);
      h1p = con2p * tmp * h1p;
      h2p = con2m * tmp * h2p;

      // Check for successful completion
      if (ier != 0)
      {
        std::cerr << " *** WARNING ***\n"
                  << " ier = " << ier << " from summation subroutine uniform_hankel_sum.\n"
                  << " z  = " << z << "\n"
                  << " nu = " << nu << "\n";
      }

    }
    else
    {
      std::cerr << " *** WARNING ***\n"
                << " ier = " << ier << " from outer factors subroutine uniform_hankel_outer.\n"
                << " z  = " << z << "\n"
                << " nu = " << nu << "\n";
    }

    if (nuswitch)
    {
      r_factor = std::exp(1.0il * nu * pi);
      h1  = h1  * r_factor;
      h1p = h1p * r_factor;
      h2  = h2  / r_factor;
      h2p = h2p / r_factor;
      nu  = -nu;
    }

    return;
  }


/**
 *  Compute outer factors and associated functions of @c z and @c nu
 *  appearing in Olver's uniform asymptotic expansions of the
 *  Hankel functions of the first and second kinds and their derivatives.
 *  The various functions of z and nu returned by @c uniform_hankel_outer
 *  are available for use in computing further terms in the expansions.
 */
template<typename _Tp>
  void
  uniform_hankel_outer(std::complex<_Tp> nu, std::complex<_Tp> z, _Tp eps,
                       std::complex<_Tp>& zhat, std::complex<_Tp>& _1dnsq,
                       std::complex<_Tp>& nm1d3, std::complex<_Tp>& nm2d3,
                       std::complex<_Tp>& t, std::complex<_Tp>& tsq,
                       std::complex<_Tp>& etm3h, std::complex<_Tp>& etrat,
                       std::complex<_Tp>& aip, std::complex<_Tp>& o4dp, std::complex<_Tp>& aim,
                       std::complex<_Tp>& o4dm, std::complex<_Tp>& od2p,
                       std::complex<_Tp>& od0dp, std::complex<_Tp>& od2m, std::complex<_Tp>& od0dm, int& ier)
  {
    std::complex<_Tp> nusq, eta,
                      etphf, etmhf,
                      argp, argm,
                      aidp, aidm;
    ier = 0;
    int ier1 = 0, ier2 = 0;

    static const std::complex<_Tp> e2pd3{-0.5L,  0.8660254037844386L};
    static const std::complex<_Tp> d2pd3{-0.5L, -0.8660254037844386L};

    if (lzdiv(z, nu, zhat))
    {
      //  Try to compute other nu and z dependent parameters except args to airy functions
      std::complex<_Tp> nm4d3;
      dparms(zhat, nu, t, tsq, nusq, _1dnsq, nm1d3, nm2d3, nm4d3,
             eta, etphf, etmhf, etm3h, etrat, ier);

      if (ier == 0)
      {
        //  Try to compute airy function arguments
        aryarg(nm2d3, eta, argp, argm, ier);

        if (ier == 0)
        {
          //  Compute airy functions and derivatives             
          airy(argp, eps, aip, aidp, ier1);
          airy(argm, eps, aim, aidm, ier2);
          if (ier1 == 0 && ier2 == 0)
          {
            //  Compute partial outer terms in expansions
            o4dp = -etmhf * nm4d3 * e2pd3 * aidp;
            o4dm = -etmhf * nm4d3 * d2pd3 * aidm;
            od2p = -etphf * nm2d3 * aip;
            od0dp = e2pd3 * aidp;
            od2m = -etphf * nm2d3 * aim;
            od0dm = d2pd3 * aidm;
          }
          else
          {
            //  Set completion code for error in evaluation of airy fns
            ier = 134;
          }
        }
        else
        {
          //  Set completion code for airy function args not computable
          ier = 133;
        }
      }
      else
      {
        //  Set completion code indicating factors not successfully computed
        ier = 135;
      }
    }
    else
    {
      //  Set completion code indicating z/nu not successfully computed
      ier = 130;
    }
    return;
  }


/**
 *  Carefully compute @c z1/z2 avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Tp>
  bool
  lzdiv(std::complex<_Tp> z1, std::complex<_Tp> z2, std::complex<_Tp>& z1dz2)
  {
    //  Note that dxhinf is a machine floating-point dependent constant
    //  set equal to half the largest available floating-point number.
    static const auto done = _Tp(1);
    static const auto dxhinf = 0.5L * std::numeric_limits<_Tp>::max();

    //  separate real and imaginary parts of arguments
    auto dz1r = std::real(z1);
    auto dz1i = std::imag(z1);
    auto dz2r = std::real(z2);
    auto dz2i = std::imag(z2);

    //  set up largest and smallest magnitudes needed
    auto dz1b = std::max(std::abs(dz1r), std::abs(dz1i));
    auto dz2b = std::abs(dz2r);
    auto dz2ub = std::abs(dz2i);

    if (dz2b < dz2ub)
    {
      auto dtemp = dz2b;
      dz2b = dz2ub;
      dz2ub = dtemp;
    }

    //  If overflow will occur, then abort
    if (dz2b < done) 
    {
      if (dz1b > dz2b * dxhinf)
      {
        return false;
      }
    }

    //  else compute the quotient
    dz1r = dz1r / dz1b;
    dz1i = dz1i / dz1b;
    dz2r = dz2r / dz2b;
    dz2i = dz2i / dz2b;
    auto term = dz2ub / dz2b;
    auto denom = done + term * term;
    auto dscale = dz1b / dz2b / denom;
    auto dqr = (dz1r * dz2r + dz1i * dz2i) * dscale;
    auto dqi = (dz2r * dz1i - dz1r * dz2i) * dscale;
    z1dz2 = std::complex<_Tp>{dqr, dqi};

    return true;
  }
