#ifndef HANKEL_TCC
#define HANKEL_TCC 1

#include <complex>
#include <iostream>
#include <limits>

#include "complex_util.h"
#include "airy.tcc"

template<typename _Tp>
  void
  uniform_hankel_olver(std::complex<_Tp> nu, std::complex<_Tp> z,
                       std::complex<_Tp> & h1, std::complex<_Tp> & h2,
                       std::complex<_Tp> & h1p, std::complex<_Tp> & h2p);

template<typename _Tp>
  void
  uniform_hankel_outer(std::complex<_Tp> nu, std::complex<_Tp> z, _Tp eps,
                       std::complex<_Tp> & zhat, std::complex<_Tp> & _1dnsq,
                       std::complex<_Tp> & nm1d3, std::complex<_Tp> & nm2d3,
                       std::complex<_Tp> & t, std::complex<_Tp> & tsq,
                       std::complex<_Tp> & etm3h, std::complex<_Tp> & etrat,
                       std::complex<_Tp> & aip, std::complex<_Tp> & o4dp,
                       std::complex<_Tp> & aim, std::complex<_Tp> & o4dm,
                       std::complex<_Tp> & od2p, std::complex<_Tp> & od0dp,
                       std::complex<_Tp> & od2m, std::complex<_Tp> & od0dm,
                       int & ier);

template<typename _Tp>
  void
  uniform_hankel_sum(std::complex<_Tp> zt, std::complex<_Tp> ztsq,
        	     std::complex<_Tp> z1dnsq, std::complex<_Tp> zetm3h,
        	     std::complex<_Tp> zaip, std::complex<_Tp> zo4dp,
        	     std::complex<_Tp> zaim, std::complex<_Tp> zo4dm,
		     std::complex<_Tp> zod2p, std::complex<_Tp> zod0dp,
        	     std::complex<_Tp> zod2m, std::complex<_Tp> zod0dm,
        	     _Tp eps,
		     int nterms, std::complex<_Tp> zwksp[50], int nzwksp,
        	     std::complex<_Tp> & h1sum, std::complex<_Tp> & h1psum,
        	     std::complex<_Tp> & h2sum, std::complex<_Tp> & h2psum,
        	     int & ier);

template<typename _Tp>
  bool
  zdiv(std::complex<_Tp> z1, std::complex<_Tp> z2,
        std::complex<_Tp> & z1dz2);

template<typename _Tp>
  void
  aryarg(std::complex<_Tp> znm2d3, std::complex<_Tp> zeta,
         std::complex<_Tp> & zargp, std::complex<_Tp> & zargm, int & ier);

template<typename _Tp>
  void
  dparms(std::complex<_Tp> nu, std::complex<_Tp> zhat,
         std::complex<_Tp> & zt, std::complex<_Tp> & ztsq,
         std::complex<_Tp> & nusq, std::complex<_Tp> & z1dnsq,
         std::complex<_Tp> & znm1d3, std::complex<_Tp> & znm2d3,
         std::complex<_Tp> & znm4d3, std::complex<_Tp> & zeta,
         std::complex<_Tp> & zetphf, std::complex<_Tp> & zetmhf,
         std::complex<_Tp> & zetm3h, std::complex<_Tp> & zetrat,
         int & ier);

/**
 *    Purpose
 *      This routine uses the old uniform_hankel subroutine of
 *      Whitaker (now called uniform_hankel_olver) to compute the
 *      uniform asymptotic approximations of the Hankel functions
 *      and their derivatives including a patch for the case when
 *      the order equals or nearly equals the argument.  At such 
 *      points, Olver's expressions have zero denominators (and
 *      numerators) resulting in numerical problems.  This routine
 *      averages four surrounding points in the complex order plane
 *      to obtain the result in such cases.
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
  uniform_hankel(std::complex<_Tp> nu, std::complex<_Tp> z,
                 std::complex<_Tp> & h1, std::complex<_Tp> & h2,
                 std::complex<_Tp> & h1p, std::complex<_Tp> & h2p)
  {
    _Tp test = std::pow(std::abs(nu), 1.0L/3.0L) / 5.0L;

    if (std::abs(z - nu) > test)
      uniform_hankel_olver(nu, z, h1, h2, h1p, h2p);
    else
      {
	_Tp r = 2 * test;
	std::complex<_Tp> anu[4]{nu + std::complex<_Tp>{r, 0.0L},
                        	 nu + std::complex<_Tp>{0.0L, r},
                        	 nu - std::complex<_Tp>{r, 0.0L},
                        	 nu - std::complex<_Tp>{0.0L, r}};

	for (auto tnu : anu)
	  {
            std::complex<_Tp> th1, th2, th1p, th2p;
            uniform_hankel_olver(tnu, z, th1, th2, th1p, th2p);
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
  uniform_hankel_olver(std::complex<_Tp> nu, std::complex<_Tp> z,
                       std::complex<_Tp> & h1, std::complex<_Tp> & h2,
                       std::complex<_Tp> & h1p, std::complex<_Tp> & h2p)
  {
    using namespace std::literals::complex_literals;

    constexpr int nwksp = 100; // > some function of nterms
    std::complex<_Tp> t, tsq,
                      _1dnsq, etm3h, aip, o4dp, aim, o4dm,
                      od2p, od0dp, od0dm, tmp, zhat, nm1d3,
                      nm2d3, etrat, od2m, wksp[nwksp], r_factor;
    int ier;

    static constexpr _Tp pi   = 3.1415'92653'58979'32384'62643'38327'95028'84195e+0L;
    static constexpr _Tp pi_3 = 1.0471'97551'19659'77461'54214'46109'31676'28063e+0L;
    static constexpr std::complex<_Tp> con1p{ 1.0, 1.732050807568877}; // 2*exp( dpi*i/3)
    static constexpr std::complex<_Tp> con1m{ 1.0,-1.732050807568877}; // 2*exp(-dpi*i/3)
    static constexpr std::complex<_Tp> con2p{-2.0, 3.464101615137755}; // 4*exp( 2*dpi*i/3)
    static constexpr std::complex<_Tp> con2m{-2.0,-3.464101615137755}; // 4*exp(-2*dpi*i/3)
    static constexpr std::complex<_Tp> j{1.0il};
    static constexpr _Tp eps   = 1.0e-06;
    static constexpr _Tp epsai = 1.0e-12;
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
            std::cerr << "  uniform_hankel_sum: "
                      << "  ier = " << ier << '\n'
                      << "  nu  = " << nu << '\n'
                      << "  z   = " << z << '\n';
	  }
      }
    else
      std::cerr << "  uniform_hankel_outer: "
                << "  ier = " << ier << '\n'
                << "  nu  = " << nu << '\n'
                << "  z   = " << z << '\n';

    if (nuswitch)
      {
	r_factor = std::exp(j * nu * pi);
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
  uniform_hankel_outer(std::complex<_Tp> nu, std::complex<_Tp> z,
                       _Tp eps,
                       std::complex<_Tp> & zhat, std::complex<_Tp> & _1dnsq,
                       std::complex<_Tp> & nm1d3, std::complex<_Tp> & nm2d3,
                       std::complex<_Tp> & t, std::complex<_Tp> & tsq,
                       std::complex<_Tp> & etm3h, std::complex<_Tp> & etrat,
                       std::complex<_Tp> & aip, std::complex<_Tp> & o4dp,
                       std::complex<_Tp> & aim, std::complex<_Tp> & o4dm,
                       std::complex<_Tp> & od2p, std::complex<_Tp> & od0dp,
                       std::complex<_Tp> & od2m, std::complex<_Tp> & od0dm,
                       int & ier)
  {
    std::complex<_Tp> nusq, eta,
                      etphf, etmhf,
                      argp, argm,
                      aidp, aidm;
    ier = 0;
    int ier1 = 0, ier2 = 0;

    static const std::complex<_Tp> e2pd3{-0.5L,  0.8660254037844386L};
    static const std::complex<_Tp> d2pd3{-0.5L, -0.8660254037844386L};

    if (zdiv(z, nu, zhat))
      {
	//  Try to compute other nu and z dependent parameters except args to airy functions
	std::complex<_Tp> nm4d3;
	dparms(nu, zhat, t, tsq, nusq, _1dnsq, nm1d3, nm2d3, nm4d3,
               eta, etphf, etmhf, etm3h, etrat, ier);

	if (ier == 0)
	{
          //  Try to compute airy function arguments
          aryarg(nm2d3, eta, argp, argm, ier);

          if (ier == 0)
            {
              //  Compute Airy functions and derivatives             
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
              else  //  error in evaluation of Airy functions
        	ier = 134;
            }
          else  //  Airy function args not computable
            ier = 133;
	}
	else  //  factors not successfully computed
          ier = 135;
      }
    else //  z/nu not successfully computed
      ier = 130;

    return;
  }


/**
    PURPOSE
      Compute the sums in appropriate linear combinations appearing in
      Olver's uniform asymptotic expansions for the Hankel functions
      of the first and second kinds and their derivatives, using up to
      nterms (less than 5) to achieve relative error eps.
 */
template<typename _Tp>
  void
  uniform_hankel_sum(std::complex<_Tp> zt, std::complex<_Tp> ztsq,
        	     std::complex<_Tp> z1dnsq, std::complex<_Tp> zetm3h,
        	     std::complex<_Tp> zaip, std::complex<_Tp> zo4dp,
        	     std::complex<_Tp> zaim, std::complex<_Tp> zo4dm,
		     std::complex<_Tp> zod2p, std::complex<_Tp> zod0dp,
        	     std::complex<_Tp> zod2m, std::complex<_Tp> zod0dm,
        	     _Tp eps,
		     int nterms, std::complex<_Tp> zwksp[50], int nzwksp,
        	     std::complex<_Tp> & h1sum, std::complex<_Tp> & h1psum,
        	     std::complex<_Tp> & h2sum, std::complex<_Tp> & h2psum,
        	     int & ier)
  {
    using cmplx = std::complex<_Tp>;

    cmplx za1, zb0, zb1, zc0, zc1, zd1, ztpowk,
     	   zatrm, zbtrm, zctrm, zdtrm, zsuma, zsumb, zsumc,
     	   zsumd, ztmpa, ztmpb, ztmpc, ztmpd, z1dif,
     	   z1pdif, z2dif, z2pdif, h1save, h1psave, h2save, h2psave,
     	   z1dn2k;

    _Tp dxzt3h,
     	dyzt3h, dytsq2, dxzu1, dyzu1, dxzv1, 
     	dyzv1, dukta, dvkta, duktb, dvktb, dukpta,
     	dvkpta, dukptb, dvkptb, dsdata;

    int index, indexp, nduv, indexend, k, l, i2k,
        i2kp1, i2km1, indexv, indexvpl, i2kl;

    bool coverged;

    constexpr auto zone = cmplx(1, 0);
    constexpr auto dtwo = _Tp(2);
    constexpr auto dthree = _Tp(3);

    //  Coefficients for u and v polynomials appearing in Olver's
    //  uniform asymptotic expansions for the Hankel functions
    //  and their derivatives

    static constexpr _Tp
    a[66]
    {
       0.1000000000000000e+01,
      -0.2083333333333333e+00,
       0.1250000000000000e+00,
       0.3342013888888889e+00,
      -0.4010416666666667e+00,
       0.7031250000000000e-01,
      -0.1025812596450617e+01,
       0.1846462673611111e+01,
      -0.8912109136581421e+00,
       0.7324218750000000e-01,
       0.4669584423426247e+01,
      -0.1120700261622299e+02,
       0.8789123535156250e+01,
      -0.2364086866378784e+01,
       0.1121520996093750e+00,
      -0.2821207255820024e+02,
       0.8463621767460073e+02,
      -0.9181824154324002e+02,
       0.4253499984741211e+02,
      -0.7368794441223145e+01,
       0.2271080017089844e+00,
       0.2125701300392171e+03,
      -0.7652524681411816e+03,
       0.1059990452528000e+04,
      -0.6995796273761325e+03,
       0.2181905059814453e+03,
      -0.2649143028259277e+02,
       0.5725014209747314e+00,
      -0.1919457662318407e+04,
       0.8061722181737309e+04,
      -0.1358655000643414e+05,
       0.1165539333686453e+05,
      -0.5305646972656250e+04,
       0.1200902954101563e+04,
      -0.1080909194946289e+03,
       0.1727727532386780e+01,
       0.2020429133096615e+05,
      -0.9698059838863751e+05,
       0.1925470012325315e+06,
      -0.2034001772804155e+06,
       0.1222004649830175e+06,
      -0.4119265625000000e+05,
       0.7109514160156250e+04,
      -0.4939153137207031e+03,
       0.6074041843414307e+01,
      -0.2429191879005513e+06,
       0.1311763614662977e+07,
      -0.2998015918538107e+07,
       0.3763271297656404e+07,
      -0.2813563226586534e+07,
       0.1268365250000000e+07,
      -0.3316451875000000e+06,
       0.4521876953125000e+05,
      -0.2499830566406250e+04,
       0.2438052940368652e+02,
       0.3284469853072038e+07,
      -0.1970681911843223e+08,
       0.5095260249266464e+08,
      -0.7410514821153266e+08,
       0.6634451227472903e+08,
      -0.3756717666076335e+08,
       0.1328876700000000e+08,
      -0.2785618250000000e+07,
       0.3081864062500000e+06,
      -0.1388608984375000e+05,
       0.1100171432495117e+03
    };

    static constexpr _Tp
    b[66]
    {  0.1000000000000000e+01,
       0.2916666666666667e+00,
      -0.3750000000000000e+00,
      -0.3949652777777778e+00,
       0.5156250000000000e+00,
      -0.1171875000000000e+00,
       0.1146496431327160e+01,
      -0.2130533854166667e+01,
       0.1089257836341858e+01,
      -0.1025390625000000e+00,
      -0.5075635242854617e+01,
       0.1238668710214120e+02,
      -0.9961006673177083e+01,
       0.2793920993804932e+01,
      -0.1441955566406250e+00,
       0.3015773273462785e+02,
      -0.9140711508856879e+02,
       0.1005628359759295e+03,
      -0.4753911590576172e+02,
       0.8502454757690430e+01,
      -0.2775764465332031e+00,
      -0.2247169946128867e+03,
       0.8146235951180321e+03,
      -0.1138508263826370e+04,
       0.7604126384523180e+03,
      -0.2411579284667969e+03,
       0.3002362060546875e+02,
      -0.6765925884246826e+00,
       0.2013089743407110e+04,
      -0.8497490948317704e+04,
       0.1440997727955136e+05,
      -0.1245921356699312e+05,
       0.5730098632812500e+04,
      -0.1315274658203125e+04,
       0.1208074951171875e+03,
      -0.1993531703948975e+01,
      -0.2106404840887960e+05,
       0.1014913238950858e+06,
      -0.2024212064239434e+06,
       0.2150230445535821e+06,
      -0.1300843659496637e+06,
       0.4424396093750000e+05,
      -0.7727732910156250e+04,
       0.5459063720703125e+03,
      -0.6883914470672607e+01,
       0.2520859497081193e+06,
      -0.1365304986690037e+07,
       0.3131261070473134e+07,
      -0.3946845507298180e+07,
       0.2965647725320941e+07,
      -0.1345235875000000e+07,
       0.3545172500000000e+06,
      -0.4883626953125000e+05,
       0.2737909667968750e+04,
      -0.2724882698059082e+02,
      -0.3395807814193124e+07,
       0.2042343072273885e+08,
      -0.5295074376688679e+08,
       0.7725855877372554e+08,
      -0.6943030354332107e+08,
       0.3949369854080250e+08,
      -0.1404812500000000e+08,
       0.2965335500000000e+07,
      -0.3310150312500000e+06,
       0.1509357617187500e+05,
      -0.1215978927612305e+03
    };

    //  lambda and mu coefficients appearing in the expansions
    static constexpr _Tp
    lambda[21]
    {
       0.1041666666666667e+00,
       0.8355034722222222e-01,
       0.1282265745563272e+00,
       0.2918490264641405e+00,
       0.8816272674437577e+00,
       0.3321408281862768e+01,
       0.1499576298686255e+02,
       0.7892301301158652e+02,
       0.4744515388682643e+03,
       0.3207490090890662e+04,
       0.2408654964087401e+05,
       0.1989231191695098e+06,
       0.1791902007775344e+07,
       0.1748437718003412e+08,
       0.1837073796763307e+09,
       0.2067904032945155e+10,
       0.2482751937593589e+11,
       0.3166945498173489e+12,
       0.4277112686513472e+13,
       0.6097113241139256e+14,
       0.9148694223435640e+15
    };

    static constexpr _Tp
    mu[21]
    {
      -0.1458333333333333e+00,
      -0.9874131944444445e-01,
      -0.1433120539158951e+00,
      -0.3172272026784136e+00,
      -0.9424291479571203e+00,
      -0.3511203040826354e+01,
      -0.1572726362036805e+02,
      -0.8228143909718595e+02,
      -0.4923553705236705e+03,
      -0.3316218568547973e+04
      -0.2482767424520859e+05,
      -0.2045265873151298e+06,
      -0.1838444917068210e+07,
      -0.1790568747352892e+08,
      -0.1878356353993943e+09,
      -0.2111438854691369e+10,
      -0.2531915342298413e+11,
      -0.3226140741130003e+12,
      -0.4352813796009286e+13,
      -0.6199585732586975e+14,
      -0.9295073331010611e+15
    };

    ier = 0;
    //  Initialize for modified Horner's rule evaluation of u_k and v_k polynomials
    auto dxtsq = std::real(ztsq);
    auto dytsq = std::imag(ztsq);
    auto dr = dtwo * dxtsq;
    //  Compute square of magnituds
    auto ds = std::norm(ztsq);

    //  Compute u_0,1,2 and v_0,1,2 and store for later use
    indexv = 2 * nterms + 1;
    ztpowk = zt;
    zwksp[0] = ztpowk * a[1] * (ztsq + a[2]);
    zwksp[indexv] = ztpowk * b[1] * (ztsq + b[2]);
    dytsq2 = dytsq * dytsq;
    ztpowk *= zt;
    zwksp[1] = ztpowk * cmplx((a[3] * dxtsq + a[4]) * dxtsq + a[5] - a[3] * dytsq2,
                               (dtwo * a[3] * dxtsq + a[4]) * dytsq);
    zwksp[indexv + 1] = ztpowk
                    * cmplx((b[3] * dxtsq + b[4]) * dxtsq + b[5] - b[3] * dytsq2,
                             (dtwo * b[3] * dxtsq + b[4]) * dytsq);
    ztpowk *= zt;
    zwksp[2] = ztpowk
             * cmplx(((a[6] * dxtsq + a[7]) * dxtsq + a[8]) * dxtsq
     		      + a[9] - (dthree * a[6] * dxtsq + a[7]) * dytsq2,
     		      ((dthree * a[6] * dxtsq + dtwo * a[7]) * dxtsq + a[8]
     		      - a[6] * dytsq2) * dytsq);
    zwksp[indexv + 2] = ztpowk
                    * cmplx(((b[6] * dxtsq + b[7]) * dxtsq + b[8])
     		      * dxtsq + b[9] - (dthree * b[6] * dxtsq + b[7]) * dytsq2,
     		      ((dthree * b[6] * dxtsq + dtwo * b[7]) * dxtsq + b[8]
     		      - b[6] * dytsq2) * dytsq);

    //  Compute a_1, b_0,1, c_0,1, d_1 ... note that
    //  uhnksm exploits that fact that a_0 = d_0 = 1
    //  also, b_k and c_k are computed up to -zeta**(-1/2)
    //  -zeta**(1/2) factors, respectively.  These recurring factors
    //  are included as appropriate in the outer factors, thus saving
    //  repeated multiplications by them.
    dxzu1 = std::real(zwksp[0]);
    dyzu1 = std::imag(zwksp[0]);
    dxzv1 = std::real(zwksp[indexv]);
    dyzv1 = std::imag(zwksp[indexv]);
    dxzt3h = std::real(zetm3h);
    dyzt3h = std::imag(zetm3h);
    za1 = zwksp[1]
        + zetm3h * (mu[1] * cmplx(dxzt3h, dyzt3h)
		 +  mu[0] * cmplx(dxzu1, dyzu1));
    zb0 = zwksp[0] + lambda[0] * cmplx(dxzt3h, dyzt3h);
    zb1 = zwksp[2]
        + zetm3h * (lambda[2] * cmplx(dxzt3h, dyzt3h)
		  + lambda[1] * cmplx(dxzu1, dyzu1)
		  + lambda[0] * zwksp[1]);
    zc0 = zwksp[indexv]
        + mu[0] * cmplx(dxzt3h, dyzt3h);
    zc1 = zwksp[indexv + 2]
        + zetm3h * (mu[2] * cmplx(dxzt3h, dyzt3h)
		  + mu[1] * cmplx(dxzv1, dyzv1)
                  + mu[0] * zwksp[indexv + 2]);
    zd1 = zwksp[indexv + 1]
        + zetm3h * (lambda[1] * cmplx(dxzt3h, dyzt3h)
		  + lambda[0] * cmplx(dxzv1, dyzv1));

    //  Compute terms
    zatrm = za1 * z1dnsq;
    zbtrm = zb1 * z1dnsq;
    zctrm = zc1 * z1dnsq;
    zdtrm = zd1 * z1dnsq;
    //  Compute sum of first two terms, thus initializing Kahan summing scheme
    zsuma = zone + zatrm;
    ztmpa = zatrm - (zsuma - zone);
    zsumb = zb0 + zbtrm;
    ztmpb = zbtrm - (zsumb - zb0);
    zsumc = zc0 + zctrm;
    ztmpc = zctrm - (zsumc - zc0);
    zsumd = zone + zdtrm;
    ztmpd = zdtrm - (zsumd - zone);

    //  Set convergence flag to no convergence indication
    coverged = false;

    //  Combine sums in form appearing in expansions
    h1sum = zaip * zsuma + zo4dp * zsumb;
    h2sum = zaim * zsuma + zo4dm * zsumb;
    h1psum = zod2p * zsumc + zod0dp * zsumd;
    h2psum = zod2m * zsumc + zod0dm * zsumd;
    h1save = zaip + zo4dp * zb0;
    h2save = zaim + zo4dm * zb0;
    h1psave = zod2p * zc0 + zod0dp;
    h2psave = zod2m * zc0 + zod0dm;

    //  Prepare to check convergence criteria for terms included thus far
    z1dif = h1sum - h1save;
    z2dif = h2sum - h2save;
    z1pdif = h1psum - h1psave;
    z2pdif = h2psum - h2psave;

    //  If convergence criteria now satisfied
    if (norm1(z1dif) < eps * norm1(h1sum)
     && norm1(z2dif) < eps * norm1(h2sum)
     && norm1(z1pdif) < eps * norm1(h1psum)
     && norm1(z2pdif) < eps * norm1(h2psum))
      coverged = true;

    //  Save current sums combined as in expansion for next convergence test
    h1save = h1sum;
    h2save = h2sum;
    h1psave = h1psum;
    h2psave = h2psum;

    //  Update index into u_k and v_k coefficients
    index = 10;
    indexp = 15;
    //  Update index into storage for u and v polynomials
    nduv = 4;
    //  Update power of nu**(-2)
    z1dn2k = z1dnsq;

    //  Loop until convergence criteria satisfied or maximum number of terms reached
    for (auto k = 2; k <= nterms; ++k)
    {
      //  Initialize for evaluation of two new u and v polynomials
      //  via Horner's rule modified for complex arguments and real coefficients
      indexend = indexp;
      ++index;
      dukta = a[index];
      dvkta = b[index];
      ++index;
      duktb = a[index];
      dvktb = b[index];
      ++indexp;
      dukpta = a[indexp];
      dvkpta = b[indexp];
      ++indexp;
      dukptb = a[indexp];
      dvkptb = b[indexp];
      //  update indices into coefficients to reflect initialization
      ++index;
      ++indexp;

      //  Loop until quantities to evaluate lowest order u and v 
      //  polynomials and partial quantities to evaluate
      //  next highest order polynomials computed
      for (auto l = index; l < indexend; ++l)
      {
        dsdata = ds * dukta;
        dukta = duktb + dr * dukta;
        duktb = a[l] - dsdata;
        dsdata = ds * dvkta;
        dvkta = dvktb + dr * dvkta;
        dvktb = b[l] - dsdata;
        dsdata = ds * dukpta;
        dukpta = dukptb + dr * dukpta;
        dukptb = a[indexp] - dsdata;
        dsdata = ds * dvkpta;
        dvkpta = dvkptb + dr * dvkpta;
        dvkptb = b[indexp] - dsdata;
        ++indexp;
      }

      //  One more iteration for highest order polynomials
      dsdata = ds * dukpta;
      dukpta = dukptb + dr * dukpta;
      dukptb = a[indexp] - dsdata;
      dsdata = ds * dvkpta;
      dvkpta = dvkptb + dr * dvkpta;
      dvkptb = b[indexp] - dsdata;

      //  Update power appearing outside polynomials
      ztpowk *= zt;

      //  Post multiply and form new polynomials
      zwksp[nduv] = ztpowk * (dukta * cmplx(dxtsq, dytsq) + duktb);
      zwksp[indexv + nduv] = ztpowk * (dvkta * cmplx(dxtsq, dytsq) + dvktb);
      ztpowk *= zt;
      ++nduv = nduv;
      zwksp[nduv] = ztpowk * (dukpta * cmplx(dxtsq, dytsq) + dukptb);
      zwksp[indexv + nduv] = ztpowk * (dvkpta * cmplx(dxtsq, dytsq) + dvkptb);

      //  Update indices in preparation for next iteration
      ++nduv = nduv;
      index = indexp;
      i2k = 2 * k;
      i2km1 = i2k - 1;
      i2kp1 = i2k + 1;
      indexp = indexp + i2kp1 + 2;

      //  Initialize for evaluation of a, b, c, and d polynomials via Horner's rule.
      za1 = mu[i2k] * cmplx(dxzt3h, dyzt3h)
          + mu[i2km1] * cmplx(dxzu1, dyzu1);
      zb1 = lambda[i2kp1] * cmplx(dxzt3h, dyzt3h)
          + lambda[i2k] * cmplx(dxzu1, dyzu1);
      zc1 = mu[i2kp1] * cmplx(dxzt3h, dyzt3h)
          + mu[i2k] * cmplx(dxzv1, dyzv1);
      zd1 = lambda[i2k] * cmplx(dxzt3h, dyzt3h)
          + lambda[i2km1] * cmplx(dxzv1, dyzv1);

      //  loop until partial a, b, c, and d evaluations done via Horner's rule
      for(auto l = 2; l <= i2km1; ++l)
      {
        indexvpl = indexv + l;
        i2kl = i2k - l;
        za1 = za1 * zetm3h + mu[i2kl] * zwksp[l];
        zd1 = zd1 * zetm3h + lambda[i2kl] * zwksp[indexvpl];
        i2kl = i2kp1 - l;
        zb1 = zb1 * zetm3h + lambda[i2kl] * zwksp[l];
        zc1 = zc1 * zetm3h + mu[i2kl] * zwksp[indexvpl];
      }

      //  complete the evaluations
      za1 = za1 * zetm3h + zwksp[i2k];
      zd1 = zd1 * zetm3h + zwksp[indexv + i2k];
      zb1 = zetm3h
     	  * (zb1 * zetm3h + lambda[0] * zwksp[i2k])
          + zwksp[i2kp1];
      zc1 = zetm3h
     	  * (zc1 * zetm3h + mu[0] * zwksp[indexv + i2k])
     	  + zwksp[indexv + i2kp1];

      //  Evaluate new terms for sums
      z1dn2k *= z1dnsq;
      zatrm = za1 * z1dn2k + ztmpa;
      zbtrm = zb1 * z1dn2k + ztmpb;
      zctrm = zc1 * z1dn2k + ztmpc;
      zdtrm = zd1 * z1dn2k + ztmpd;

      //  Update sums via Kahan summing scheme
      ztmpa = zsuma;
      zsuma += zatrm;
      ztmpa = zatrm - (zsuma - ztmpa);
      ztmpb = zsumb;
      zsumb += zbtrm;
      ztmpb = zbtrm - (zsumb - ztmpb);
      ztmpc = zsumc;
      zsumc += zctrm;
      ztmpc = zctrm - (zsumc - ztmpc);
      ztmpd = zsumd;
      zsumd += zdtrm;
      ztmpd = zdtrm - (zsumd - ztmpd);

      //  Combine sume in form appearing in expansions
      h1sum  = zaip  * zsuma  + zo4dp * zsumb;
      h2sum  = zaim  * zsuma  + zo4dm * zsumb;
      h1psum = zod2p * zsumc + zod0dp * zsumd;
      h2psum = zod2m * zsumc + zod0dm * zsumd;

      //  Prepare for convergence tests
      z1dif = h1sum - h1save;
      z2dif = h2sum - h2save;
      z1pdif = h1psum - h1psave;
      z2pdif = h2psum - h2psave;

      //  If convergence criteria met this term, see if it was before
      if (norm1(z1dif) < eps * norm1(h1sum)
       && norm1(z2dif) < eps * norm1(h2sum)
       && norm1(z1pdif) < eps * norm1(h1psum)
       && norm1(z2pdif) < eps * norm1(h2psum)) 
        {
          if (coverged) // Convergence - relative error criteria met twice in a row
            return;
          else  //  Converged
            coverged = true;
        }
      else  //  Reset relative error criteria flag
        coverged = false;

      //  Save combined sums for comparison next iteration
      h1save = h1sum;
      h2save = h2sum;
      h1psave = h1psum;
      h2psave = h2psum;
    }

    //  All allowable terms used - set completion code
    ier = 177;

    return;
  }


/**
 *    Compute parameters depending on z and nu that appear in the
 *    uniform asymptotic expansions of the Hankel functions and
 *    their derivatives, except the arguments to the Airy functions.
 */
template<typename _Tp>
  void
  dparms(std::complex<_Tp> nu, std::complex<_Tp> zhat,
         std::complex<_Tp> & zt, std::complex<_Tp> & ztsq,
         std::complex<_Tp> & nusq, std::complex<_Tp> & z1dnsq,
         std::complex<_Tp> & znm1d3, std::complex<_Tp> & znm2d3,
         std::complex<_Tp> & znm4d3, std::complex<_Tp> & zeta,
         std::complex<_Tp> & zetphf, std::complex<_Tp> & zetmhf,
         std::complex<_Tp> & zetm3h, std::complex<_Tp> & zetrat,
         int & ier)
  {
    std::complex<_Tp> ztemp, zlnzet;
    _Tp du, dv,
        dxir, dxii;

    //  data statements defining constants used in this subroutine
    //  note that dinf and dinfsr are machine floating-point dependent
    //  constants equal to the largest available floating-point number and
    //  its square root, respectively.

    constexpr auto dinf   = std::numeric_limits<_Tp>::max();
    constexpr auto dinfsr = std::sqrt(dinf);

    constexpr auto dzero  = _Tp(0.0L);
    constexpr auto d1d4   = _Tp(0.25L);
    constexpr auto d1d3   = _Tp(0.33333333333333333333L);
    constexpr auto dhalf  = _Tp(0.5L);
    constexpr auto d2d3   = _Tp(0.66666666666666633337L);
    constexpr auto done   = _Tp(1.0L);
    constexpr auto dtwo   = _Tp(2.0L);
    constexpr auto d2pi   = _Tp(6.283185307179586L);
    constexpr auto dlncon = _Tp(0.2703100720721096L);
    constexpr auto dsqr2  = _Tp(1.4142135623730950L);
    constexpr auto d4d3   = _Tp(1.33333333333333333333L);

    constexpr std::complex<_Tp> zone{1.0L, 0.0L};

    //  Separate real and imaginary parts of zhat
    auto dx = std::real(zhat);
    auto dy = std::imag(zhat);
    auto dxabs = std::abs(dx);
    auto dyabs = std::abs(dy);

    //  If 1 - zhat**2 can be computed without overflow
    if (dxabs <= dinfsr && dyabs <= (dinfsr - done))
      {
	//  find max and min of abs(dx) and abs(dy)
	du = dxabs;
	dv = dyabs;
	if (du < dv)
          std::swap(du, dv);
        if (du >= dhalf && dv > dinf / (dtwo * du))
	  {
	    //  set completion code - unable to compute 1-zhat**2 and exit
	    ier = 131;
	    return;
          }
      }
    else
      {
	//  set completion code - unable to compute 1-zhat**2 and exit
	ier = 131;
	return;
      }

    //  compute 1 - zhat**2 and related constants
    ztemp = std::complex<_Tp>{done - (dx - dy) * (dx + dy), -dtwo * dx * dy};
    ztemp = std::sqrt(ztemp);
    zt = done / ztemp;
    ztsq = zt * zt;

    //  if nu**2 can be computed without overflow
    if (std::abs(nu) <= dinfsr)
      {
	nusq = nu * nu;
	z1dnsq = done / nusq;
	//  compute nu**(-2/3), nu**(-4/3), nu**(-1/3)
	znm4d3 = -std::log(nu);
	znm1d3 = std::exp(d1d3 * znm4d3);
	znm2d3 = std::exp(d2d3 * znm4d3);
	znm4d3 = std::exp(d4d3 * znm4d3);
      }
    else
      {
	//  set completion code - unable to compute nu**2
	ier = 132;
	return;
      }

    //  compute xi = ln(1+(1-zhat**2)**(1/2)) - ln(zhat) - (1-zhat**2)**(1/2)
    //  using default branch of logarithm and square root
    auto xi = std::log(zone + ztemp) - std::log(zhat) - ztemp;
    zetm3h = d2d3 / xi;

    //  Compute principal value of ln(xi) and then adjust imaginary part
    auto lnxi = std::log(xi);

    //  Prepare to adjust logarithm of xi to appropriate Riemann sheet
    auto dtemp = dzero;

    //  Find adjustment necessary to get on proper Riemann sheet

    if (dy == dzero)  //  zhat is real
      {
	if (dx > done)
	  dtemp = d2pi;
      }
    else  //  zhat is not real
      {
	//  if zhat is in upper half-plane
	if (dy > dzero)
	  {
	    //  if xi lies in upper half-plane
	    if (std::imag(xi) > dzero)
	      dtemp = -d2pi;
	    else
	      dtemp = d2pi;
	  }
      }

    //  Adjust logarithm of xi.
    lnxi += dtemp;

    //  compute ln(zeta), zeta, zeta^(1/2), zeta^(-1/2)
    zlnzet = lnxi + dlncon;
    zeta = std::exp(zlnzet);
    zetphf = std::sqrt(zeta);
    zetmhf = done / zetphf;

    //  compute (4 * zeta / (1 - zhat**2))^(1/4)
    ztemp = std::log(ztemp);
    zetrat = dsqr2 * std::exp(d1d4 * zlnzet - dhalf * ztemp);

    return;
  }


/**
    Purpose
      Compute the arguments for the Airy function evaluations
      carefully to prevent premature overflow.  Note that the
      major work here is in zdiv.  A faster, but less safe
      implementation can be obtained without use of zdiv.

    Arguments
    @param[in]  znm2d3  nu**(-2/3).  in our implementation, zmn2d3 is
	                output from dparms.
    @param[in]  zeta    zeta in the uniform asymptotic expansions.
	                In our implementation, zeta is output from dparms.
    @param[out]  zargp  exp(2*pi*i/3) * nu(2/3) * zeta.
    @param[out]  zargm  exp(-2*pi*i/3) * nu(2/3) * zeta.
    @param[out]  ier    A completion code.
	                ier = 0 indicates normal completion.
                        ier = 133 indicates failure in computing nu(2/3)*zeta
                        from the input zmn2d3 and zeta.
 */
template<typename _Tp>
  void
  aryarg(std::complex<_Tp> znm2d3, std::complex<_Tp> zeta,
         std::complex<_Tp> & zargp, std::complex<_Tp> & zargm, int & ier)
  {
    //  zexpp and zexpm are exp(2*pi*i/3) and its reciprocal, respectively.
    constexpr auto zexpp = std::complex<_Tp>{-0.5L,  0.8660254037844386L};
    constexpr auto zexpm = std::complex<_Tp>{-0.5L, -0.8660254037844386L};

    ier = 0;

    if (zdiv(zeta, znm2d3, zargm))
      {
        zargp *= zexpp;
        zargm *= zexpm;
      }
    else
      ier = 133;
  }

#endif // HANKEL_TCC
