
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
 */
void
uniform_hankel(std::complex<_Tp> & h1, std::complex<_Tp> & h2,
               std::complex<_Tp> & h1p, std::complex<_Tp> & h2p,
               std::complex<_Tp> z, std::complex<_Tp> nu)
{
  _Tp test = std::pow(std::abs(nu), 1.0/3.0) / 5.0;

  if (std::abs(z - nu) > ctest) then
    uniform_hankel_olver(h1, h2, h1p, h2p, z, nu);
  else
  {
    _Tp r = 2 * test;
    complex<_Tp> anu[4]{nu + complex<_Tp>{r, 0},
                        nu + complex<_Tp>{0, r},
                        nu - complex<_Tp>{r, 0},
                        nu - complex<_Tp>{0, r}}

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



//
//      Use the uniform asymptotic expansion to compute approximate
//      values for the Hankel functions of the first and second kinds
//      of order znu along with their derivatives.
//
//      nu    Theborder for which the Hankel functions are to be evaluated.
//      z     The argument at which the Hankel functions are to be evaluated.
//      h1    The computed value for the Hankel function of the first kind.
//      h1p   The computed value for the derivative of the Hankel function of the first kind.
//      h2    The computed value for the Hankel function of the second kind.
//      h2p   The computed value for the derivative of the Hankel function of the second kind.
//
void
uniform_hankel_olver(std::complex<_Tp> & h1, std::complex<_Tp> & h2,
                     std::complex<_Tp> & h1p, std::complex<_Tp> & h2p,
                     std::complex<_Tp> z, std::complex<_Tp> nu)
{

      std::complex<_Tp>  zt, ztsq,
                      z1dnsq, zetm3h, zaip, zo4dp, zaim, zo4dm,
                      zod2p, zod0dp, zod0dm, ztmp, zhat, znm1d3,
                      znm2d3, zetrat, zod2m, zwksp(50), r_factor
      int ier, nzwksp = 50;

//     data statements defining constants used herein
//       note that zcon1p = 2*exp(dpi*i/3)
//                 zcon1m = 2*exp(-dpi*i/3)
//                 zcon2p = 4*exp(2*dpi*i/3)
//                 zcon2m = 4*exp(-2*dpi*i/3)
//                 nzwksp .gt. some function of nterms
      static const std::complex<_Tp> zcon1p{ 1.0, 1.732050807568877);
      static const std::complex<_Tp> zcon1m{ 1.0,-1.732050807568877);
      static const std::complex<_Tp> zcon2p{-2.0, 3.464101615137755);
      static const std::complex<_Tp> zcon2m{-2.0,-3.464101615137755);

      static const _Tp deps = 1.0e-06;
      static const _Tp depsai = 1.0e-12;
      int nterms = 4;

      //  Extended to accommodate negative real orders.
      int nusw = 0
      if (std::real(nu) < 0.0)
      {
         nusw = 1;
         nu = -nu;
      }

      // Compute outer factors in the uniform asymptotic expansions
      // for the Hankel functions and their derivatives along with
      // other important functions of nu and z.
      uhanko(znu,z,depsai,zhat,z1dnsq,znm1d3,znm2d3,zt,ztsq,
             zetm3h,zetrat,zaip,zo4dp,zaim,zo4dm,zod2p,
             zod0dp,zod2m,zod0dm,ier);

      // Check for successful completion
      if (ier == 0)
      {

        // Compute further terms in the expansions in their appropriate linear combinations.

        uniform_hankel_sum(zt,ztsq,z1dnsq,zetm3h,zaip,zo4dp,zaim,zo4dm,
                           zod2p,zod0dp,zod2m,zod0dm,deps,nterms,zwksp,
                           nzwksp,zh1,zh1p,zh2,zh2p,ier);

        // Assemble approximations.
        ztmp = zetrat * znm1d3;
        h1 = zcon1m * ztmp * h1;
        h2 = zcon1p * ztmp * h2;
        ztmp = znm2d3 / (zhat * zetrat);
        h1p = zcon2p * ztmp * h1p;
        h2p = zcon2m * ztmp * h2p;

        // Check for successful completion
        if (ier != 0)
        {
          write(6,6000) ier,z,znu
 6000     format(//' *** WARNING ***'
                  /' ier = ',i5,' from summation subroutine uhnksm.'
                  /' z  = (',d23.16,',',d23.16,')'
                  /' nu = (',d23.16,',',d23.16,')')
        }

      }
      else
      {
        write(6,6010) ier, z, nu
 6010   format(//' *** WARNING ***'
                /' ier = ',i5,' from outer factors subroutine uhanko.'
                /' z  = (',d23.16,',',d23.16,')'
                /' nu = (',d23.16,',',d23.16,')')
      }

      if (nusw == 1)
      {
         r_factor = std::exp(1.0li * znu * dpi);
         h1  = h1  * r_factor;
         h1p = h1p * r_factor;
         h2  = h2  / r_factor;
         h2p = h2p / r_factor;
         nu  = -nu;
      }

      return;
}
