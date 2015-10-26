#ifndef HANKEL_TCC
#define HANKEL_TCC 1

#include <complex>
#include <iostream>
#include <limits>

template<typename _Tp>
  void
  uniform_hankel_olver(std::complex<_Tp> & h1, std::complex<_Tp> & h2,
                       std::complex<_Tp> & h1p, std::complex<_Tp> & h2p,
                       std::complex<_Tp> z, std::complex<_Tp> nu);

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
                       int & ier);

template<typename _Tp>
  void
  uniform_hankel_sum(std::complex<_Tp> zt, std::complex<_Tp> ztsq,
        	     std::complex<_Tp> z1dnsq, std::complex<_Tp> zetm3h,
        	     std::complex<_Tp> zaip, std::complex<_Tp> zo4dp,
        	     std::complex<_Tp> zaim, std::complex<_Tp> zo4dm,
		     std::complex<_Tp> zod2p, std::complex<_Tp> zod0dp,
        	     std::complex<_Tp> zod2m, std::complex<_Tp> zod0dm,
        	     _Tp deps,
		     int nterms, std::complex<_Tp> zwksp[50], int nzwksp,
        	     std::complex<_Tp> & zh1sm, std::complex<_Tp> & zh1dsm,
        	     std::complex<_Tp> & zh2sm, std::complex<_Tp> & zh2dsm,
        	     int & ier);

template<typename _Tp>
  bool
  lzdiv(std::complex<_Tp> z1, std::complex<_Tp> z2,
        std::complex<_Tp> & z1dz2);

template<typename _Tp>
  void
  aryarg(std::complex<_Tp> znm2d3, std::complex<_Tp> zeta,
         std::complex<_Tp> & zargp, std::complex<_Tp> & zargm, int & ier);

template<typename _Tp>
  void
  dparms(std::complex<_Tp> zhat, std::complex<_Tp> znu,
         std::complex<_Tp> & zt, std::complex<_Tp> & ztsq,
         std::complex<_Tp> & znusq, std::complex<_Tp> & z1dnsq,
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
  uniform_hankel(std::complex<_Tp> & h1, std::complex<_Tp> & h2,
                 std::complex<_Tp> & h1p, std::complex<_Tp> & h2p,
                 std::complex<_Tp> z, std::complex<_Tp> nu)
  {
    _Tp test = std::pow(std::abs(nu), 1.0L/3.0L) / 5.0L;

    if (std::abs(z - nu) > test)
      uniform_hankel_olver(h1, h2, h1p, h2p, z, nu);
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

    constexpr int nwksp = 50; // > some function of nterms
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
            std::cerr << " *** WARNING ***\n"
                      << " ier = " << ier << " from summation subroutine uniform_hankel_sum.\n"
                      << " z  = " << z << "\n"
                      << " nu = " << nu << "\n";
	  }
      }
    else
      std::cerr << " *** WARNING ***\n"
                << " ier = " << ier << " from outer factors subroutine uniform_hankel_outer.\n"
                << " z  = " << z << "\n"
                << " nu = " << nu << "\n";

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
 *  Carefully compute @c z1/z2 avoiding overflow and destructive underflow.
 *  If the quotient is successfully computed, then the logical value @c true
 *  is returned and the quotient is returned in @c z1dz2.
 *  Otherwise, @c false is returned and the quotient is not.
 */
template<typename _Tp>
  bool
  lzdiv(std::complex<_Tp> z1, std::complex<_Tp> z2,
        std::complex<_Tp> & z1dz2)
  {
    //  Note that dxhinf is a machine floating-point dependent constant
    //  set equal to half the largest available floating-point number.
    static constexpr auto xhinf = 0.5L * std::numeric_limits<_Tp>::max();

    //  Separate real and imaginary parts of arguments
    auto z1r = std::real(z1);
    auto z1i = std::imag(z1);
    auto z2r = std::real(z2);
    auto z2i = std::imag(z2);

    //  Set up largest and smallest magnitudes needed
    auto z1b = std::max(std::abs(z1r), std::abs(z1i));
    auto z2b = std::abs(z2r);
    auto z2ub = std::abs(z2i);

    if (z2b < z2ub)
      std::swap(z2b, z2ub);

    //  If overflow will occur, then abort
    if (z2b < _Tp(1) && z1b > z2b * xhinf)
      return false;

    //  Compute the quotient
    z1r /= z1b;
    z1i /= z1b;
    z2r /= z2b;
    z2i /= z2b;
    auto term = z2ub / z2b;
    auto denom = _Tp(1) + term * term;
    auto scale = z1b / z2b / denom;
    auto qr = (z1r * z2r + z1i * z2i) * scale;
    auto qi = (z2r * z1i - z1r * z2i) * scale;
    z1dz2 = std::complex<_Tp>{qr, qi};

    return true;
  }


/**
    PURPOSE
      Compute the sums in appropriate linear combinations appearing in
      Olver's uniform asymptotic expansions for the Hankel functions
      of the first and second kinds and their derivatives, using up to
      nterms (less than 5) to achieve relative error deps.
 */
template<typename _Tp>
  void
  uniform_hankel_sum(std::complex<_Tp> zt, std::complex<_Tp> ztsq,
        	     std::complex<_Tp> z1dnsq, std::complex<_Tp> zetm3h,
        	     std::complex<_Tp> zaip, std::complex<_Tp> zo4dp,
        	     std::complex<_Tp> zaim, std::complex<_Tp> zo4dm,
		     std::complex<_Tp> zod2p, std::complex<_Tp> zod0dp,
        	     std::complex<_Tp> zod2m, std::complex<_Tp> zod0dm,
        	     _Tp deps,
		     int nterms, std::complex<_Tp> zwksp[50], int nzwksp,
        	     std::complex<_Tp> & zh1sm, std::complex<_Tp> & zh1dsm,
        	     std::complex<_Tp> & zh2sm, std::complex<_Tp> & zh2dsm,
        	     int & ier)
  {
    using dcmplx = std::complex<_Tp>;

    dcmplx za1, zb0, zb1, zc0, zc1, zd1, ztpowk,
     	   zatrm, zbtrm, zctrm, zdtrm, zsuma, zsumb, zsumc,
     	   zsumd, ztmpa, ztmpb, ztmpc, ztmpd, z1dif,
     	   z1ddif, z2dif, z2ddif, zh1s, zh1ds, zh2s, zh2ds,
     	   z1dn2k;

    _Tp dxzt3h,
     	dyzt3h, dytsq2, dxzu1, dyzu1, dxzv1, 
     	dyzv1, dukta, dvkta, duktb, dvktb, dukpta,
     	dvkpta, dukptb, dvkptb, dsdata;

    int ndx, ndxp, nduv, ndxend, k, l, i2k,
        i2kp1, i2km1, ndxv, ndxvpl, i2kl;

    bool lcvgnc;

    auto zone = dcmplx(1.0L, 0.0L);
    auto dtwo = _Tp(2.0L);
    auto dthree = _Tp(3.0L);

    //  Coefficients for u and v polynomials appearing in Olver's
    //  uniform asymptotic expansions for the hankel functions
    //  and their derivatives

    static constexpr _Tp
    a[66]{  0.1000000000000000e+01,
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
            0.1100171432495117e+03 };

    static constexpr _Tp
    b[66]{  0.1000000000000000e+01,
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
           -0.1215978927612305e+03 };

    //  lambda and mu coefficients appearing in the expansions
    static constexpr _Tp
    dlamda[21]{ 0.1041666666666667e+00,
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
		0.9148694223435640e+15 };

    static constexpr _Tp
    dmu[21]{ -0.1458333333333333e+00,
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
	     -0.9295073331010611e+15 };

    ier = 0;
    //  Initialize for modified Horner's rule evaluation of u_k and v_k polynomials
    auto dxtsq = std::real(ztsq);
    auto dytsq = std::imag(ztsq);
    auto dr = dtwo * dxtsq;

    //  Compute square of magnituds
    auto ds = dxtsq * dxtsq + dytsq * dytsq;
    //  Compute u-sub-1,2,3 and v-sub-1,2,3 and store for later use
    ndxv = 2 * nterms + 1;
    ztpowk = zt;
    zwksp[0] = ztpowk * dcmplx(a[1] * dxtsq + a[2], a[1] * dytsq);
    zwksp[ndxv] = ztpowk * dcmplx(b[1] * dxtsq + b[2], b[1] * dytsq);
    dytsq2 = dytsq * dytsq;
    ztpowk = zt * ztpowk;
    zwksp[1] = ztpowk * dcmplx((a[3] * dxtsq + a[4]) * dxtsq + a[5] - a[3] * dytsq2,
                               (dtwo * a[3] * dxtsq + a[4]) * dytsq);
    zwksp[ndxv + 1] = ztpowk
                    * dcmplx((b[3] * dxtsq + b[4]) * dxtsq + b[5] - b[3] * dytsq2,
                             (dtwo * b[3] * dxtsq + b[4]) * dytsq);
    ztpowk = zt * ztpowk;
    zwksp[2] = ztpowk
             * dcmplx(((a[6] * dxtsq + a[7]) * dxtsq + a[8]) * dxtsq
     		      + a[9] - (dthree * a[6] * dxtsq + a[7]) * dytsq2,
     		      ((dthree * a[6] * dxtsq + dtwo * a[7]) * dxtsq + a[8]
     		      - a[6] * dytsq2) * dytsq);
    zwksp[ndxv + 2] = ztpowk
                    * dcmplx(((b[6] * dxtsq + b[7]) * dxtsq + b[8])
     		      * dxtsq + b[9] - (dthree * b[6] * dxtsq + b[7]) * dytsq2,
     		      ((dthree * b[6] * dxtsq + dtwo * b[7]) * dxtsq + b[8]
     		      - b[6] * dytsq2) * dytsq);

    //  Compute a-sub-1, b-sub-0,1, c-sub-0,1, d-sub-1 ... note that
    //  uhnksm exploits that fact that a-sub-0 = d-sub-0 = 1
    //  also, b-sub-k and c-sub-k are computed up to -zeta**(-1/2)
    //  -zeta**(1/2) factors, respectively.  These recurring factors
    //  are included as appropriate in the outer factors, thus saving
    //  repeated multiplications by them.
    dxzu1 = std::real(zwksp[0]);
    dyzu1 = std::imag(zwksp[0]);
    dxzv1 = std::real(zwksp[ndxv]);
    dyzv1 = std::imag(zwksp[ndxv]);
    dxzt3h = std::real(zetm3h);
    dyzt3h = std::imag(zetm3h);
    za1 = zwksp[1]
        + (dcmplx(dmu[1] * dxzt3h, dmu[1] * dyzt3h)
        +  dcmplx(dmu[0] * dxzu1, dmu[0] * dyzu1)) * zetm3h;
    zb0 = zwksp[0]
        + dcmplx(dlamda[0] * dxzt3h, dlamda[0] * dyzt3h);
    zb1 = zwksp[2]
        + zetm3h * (dcmplx(dlamda[2] * dxzt3h + dlamda[1] * dxzu1,
                           dlamda[2] * dyzt3h + dlamda[1] * dyzu1)
                  + zetm3h * dcmplx(dlamda[0] * std::real(zwksp[1]),
                                    dlamda[0] * std::imag(zwksp[1])));
    zc0 = zwksp[ndxv]
        + dcmplx(dmu[0] * dxzt3h, dmu[0] * dyzt3h);
    zc1 = zwksp[ndxv + 2]
        + zetm3h * (dcmplx(dmu[2] * dxzt3h + dmu[1] * dxzv1,
                           dmu[2] * dyzt3h + dmu[1] * dyzv1)
                  + zetm3h * dcmplx(dmu[0] * std::real(zwksp[ndxv + 2]),
                                    dmu[0] * std::imag(zwksp[ndxv + 2])));
    zd1 = zwksp[ndxv + 1]
        + zetm3h * dcmplx(dlamda[1] * dxzt3h + dlamda[0] * dxzv1,
                          dlamda[1] * dyzt3h + dlamda[0] * dyzv1);

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
    lcvgnc = false;

    //  Combine sums in form appearing in expansions
    zh1sm = zaip * zsuma + zo4dp * zsumb;
    zh2sm = zaim * zsuma + zo4dm * zsumb;
    zh1dsm = zod2p * zsumc + zod0dp * zsumd;
    zh2dsm = zod2m * zsumc + zod0dm * zsumd;
    zh1s = zaip + zo4dp * zb0;
    zh2s = zaim + zo4dm * zb0;
    zh1ds = zod2p * zc0 + zod0dp;
    zh2ds = zod2m * zc0 + zod0dm;

    //  Prepare to check convergence criteria for terms included thus far
    z1dif = zh1sm - zh1s;
    z2dif = zh2sm - zh2s;
    z1ddif = zh1dsm - zh1ds;
    z2ddif = zh2dsm - zh2ds;

    //  If convergence criteria now satisfied
    if (std::abs(std::real(z1dif)) + std::abs(std::imag(z1dif))
     	 < deps * (std::abs(std::real(zh1sm)) + std::abs(std::imag(zh1sm)))
     && std::abs(std::real(z2dif)) + std::abs(std::imag(z2dif))
     	 < deps * (std::abs(std::real(zh2sm)) + std::abs(std::imag(zh2sm)))
     && std::abs(std::real(z1ddif)) + std::abs(std::imag(z1ddif))
     	 < deps * (std::abs(std::real(zh1dsm)) + std::abs(std::imag(zh1dsm)))
     && std::abs(std::real(z2ddif)) + std::abs(std::imag(z2ddif))
     	 < deps * (std::abs(std::real(zh2dsm)) + std::abs(std::imag(zh2dsm)))) 
      lcvgnc = true;

    //  Save current sums combined as in expansion for next convergence test
    zh1s = zh1sm;
    zh2s = zh2sm;
    zh1ds = zh1dsm;
    zh2ds = zh2dsm;

    //  Update index into u_k and v_k coefficients
    ndx = 10;
    ndxp = 15;
    //  Update index into storage for u and v polynomials
    nduv = 4;
    //  Update power of nu**(-2)
    z1dn2k = z1dnsq;

    //  Loop until convergence criteria satisfied or maximum number of terms reached
    for (auto k = 2; k <= nterms; ++k)
    {
      //  Initialize for evaluation of two new u and v polynomials
      //  via Horner's rule modified for complex arguments and real coefficients
      ndxend = ndxp;
      ++ndx;
      dukta = a[ndx];
      dvkta = b[ndx];
      ++ndx;
      duktb = a[ndx];
      dvktb = b[ndx];
      ++ndxp;
      dukpta = a[ndxp];
      dvkpta = b[ndxp];
      ++ndxp;
      dukptb = a[ndxp];
      dvkptb = b[ndxp];
      //  update indices into coefficients to reflect initialization
      ++ndx;
      ++ndxp;

      //  Loop until quantities to evaluate lowest order u and v 
      //  polynomials and partial quantities to evaluate
      //  next highest order polynomials computed
      for (auto l = ndx; l < ndxend; ++l)
      {
        dsdata = ds * dukta;
        dukta = duktb + dr * dukta;
        duktb = a[l] - dsdata;
        dsdata = ds * dvkta;
        dvkta = dvktb + dr * dvkta;
        dvktb = b[l] - dsdata;
        dsdata = ds * dukpta;
        dukpta = dukptb + dr * dukpta;
        dukptb = a[ndxp] - dsdata;
        dsdata = ds * dvkpta;
        dvkpta = dvkptb + dr * dvkpta;
        dvkptb = b[ndxp] - dsdata;
        ++ndxp;
      }

      //  One more iteration for highest order polynomials
      dsdata = ds * dukpta;
      dukpta = dukptb + dr * dukpta;
      dukptb = a[ndxp] - dsdata;
      dsdata = ds * dvkpta;
      dvkpta = dvkptb + dr * dvkpta;
      dvkptb = b[ndxp] - dsdata;

      //  Update power appearing outside polynomials
      ztpowk *= zt;

      //  Post multiply and form new polynomials
      zwksp[nduv] = ztpowk
                  * dcmplx(dukta * dxtsq + duktb, dukta * dytsq);
      zwksp[ndxv + nduv] = ztpowk
                         * dcmplx(dvkta * dxtsq + dvktb, dvkta * dytsq);
      ztpowk *= zt;
      ++nduv = nduv;
      zwksp[nduv] = ztpowk
                  * dcmplx(dukpta * dxtsq + dukptb, dukpta * dytsq);
      zwksp[ndxv + nduv] = ztpowk
                         * dcmplx(dvkpta * dxtsq + dvkptb, dvkpta * dytsq);

      //  Update indices in preparation for next iteration
      ++nduv = nduv;
      ndx = ndxp;
      i2k = 2 * k;
      i2km1 = i2k - 1;
      i2kp1 = i2k + 1;
      ndxp = ndxp + i2kp1 + 2;

      //  Initialize for evaluation of a, b, c, and d polynomials via Horner's rule.
      za1 = dcmplx(dmu[i2k] * dxzt3h + dmu[i2km1] * dxzu1,
                   dmu[i2k] * dyzt3h + dmu[i2km1] * dyzu1);
      zb1 = dcmplx(dlamda[i2kp1] * dxzt3h + dlamda[i2k] * dxzu1,
                   dlamda[i2kp1] * dyzt3h + dlamda[i2k] * dyzu1);
      zc1 = dcmplx(dmu[i2kp1] * dxzt3h + dmu[i2k]*dxzv1,
                   dmu[i2kp1] * dyzt3h + dmu[i2k]*dyzv1);
      zd1 = dcmplx(dlamda[i2k] * dxzt3h + dlamda[i2km1] * dxzv1,
                   dlamda[i2k] * dyzt3h + dlamda[i2km1] * dyzv1);

      //  loop until partial a, b, c, and d evaluations done via horner's rule
      for(auto l = 2; l <= i2km1; ++l)
      {
        ndxvpl = ndxv + l;
        i2kl = i2k - l;
        za1 = za1 * zetm3h
            + dcmplx(dmu[i2kl] * std::real(zwksp[l]),
                     dmu[i2kl] * std::imag(zwksp[l]));
        zd1 = zd1 * zetm3h
            + dcmplx(dlamda[i2kl] * std::real(zwksp[ndxvpl]),
                     dlamda[i2kl] * std::imag(zwksp[ndxvpl]));
        i2kl = i2kp1 - l;
        zb1 = zb1 * zetm3h
            + dcmplx(dlamda[i2kl] * std::real(zwksp[l]),
                     dlamda[i2kl] * std::imag(zwksp[l]));
        zc1 = zc1 * zetm3h
            + dcmplx(dmu[i2kl] * std::real(zwksp[ndxvpl]),
                     dmu[i2kl] * std::imag(zwksp[ndxvpl]));
      }

      //  complete the evaluations
      za1 = za1 * zetm3h + zwksp[i2k];
      zd1 = zd1 * zetm3h + zwksp[ndxv + i2k];
      zb1 = zetm3h * (zb1 * zetm3h
     	  + dcmplx(dlamda[0] * std::real(zwksp[i2k]),
     		   dlamda[0] * std::imag(zwksp[i2k])))
          + zwksp[i2kp1];
      zc1 = zetm3h * (zc1 * zetm3h
     	  + dcmplx(dmu[0] * std::real(zwksp[ndxv + i2k]),
     		   dmu[0] * std::imag(zwksp[ndxv + i2k])))
     	  + zwksp[ndxv + i2kp1];

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
      zh1sm  = zaip  * zsuma  + zo4dp * zsumb;
      zh2sm  = zaim  * zsuma  + zo4dm * zsumb;
      zh1dsm = zod2p * zsumc + zod0dp * zsumd;
      zh2dsm = zod2m * zsumc + zod0dm * zsumd;

      //  Prepare for convergence tests
      z1dif = zh1sm - zh1s;
      z2dif = zh2sm - zh2s;
      z1ddif = zh1dsm - zh1ds;
      z2ddif = zh2dsm - zh2ds;

      //  If convergence criteria met this term, see if it was before
      if (std::abs(std::real(z1dif))  + std::abs(std::imag(z1dif))
     	   < deps*(std::abs(std::real(zh1sm)) + std::abs(std::imag(zh1sm)))
       && std::abs(std::real(z2dif))  + std::abs(std::imag(z2dif))
     	   < deps*(std::abs(std::real(zh2sm)) + std::abs(std::imag(zh2sm)))
       && std::abs(std::real(z1ddif)) + std::abs(std::imag(z1ddif))
     	   < deps*(std::abs(std::real(zh1dsm)) + std::abs(std::imag(zh1dsm)))
       && std::abs(std::real(z2ddif)) + std::abs(std::imag(z2ddif))
     	   < deps*(std::abs(std::real(zh2dsm))+std::abs(std::imag(zh2dsm)))) 
        {
          if (lcvgnc) // Convergence - rel. error criteria met twice in a row
            return;
          else  //  Converged
            lcvgnc = true;
        }
      else // Reset rel. error criteria flag
        lcvgnc = false;

      //  Save combined sums for comparison next iteration
      zh1s = zh1sm;
      zh2s = zh2sm;
      zh1ds = zh1dsm;
      zh2ds = zh2dsm;
    } // 30

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
  dparms(std::complex<_Tp> zhat, std::complex<_Tp> znu,
         std::complex<_Tp> & zt, std::complex<_Tp> & ztsq,
         std::complex<_Tp> & znusq,std::complex<_Tp> & z1dnsq,
         std::complex<_Tp> & znm1d3, std::complex<_Tp> & znm2d3,
         std::complex<_Tp> & znm4d3, std::complex<_Tp> & zeta,
         std::complex<_Tp> & zetphf, std::complex<_Tp> & zetmhf,
         std::complex<_Tp> & zetm3h, std::complex<_Tp> & zetrat,
         int & ier)
  {
    std::complex<_Tp> ztemp, zxi, zlnxi, zlnzet;
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
    constexpr auto d1d3   = _Tp(0.3333333333333333L);
    constexpr auto dhalf  = _Tp(0.5L);
    constexpr auto d2d3   = _Tp(0.6666666666666667L);
    constexpr auto done   = _Tp(1.0L);
    constexpr auto dtwo   = _Tp(2.0L);
    constexpr auto d2pi   = _Tp(6.283185307179586L);
    constexpr auto dlncon = _Tp(0.2703100720721096L);
    constexpr auto dsqr2  = _Tp(1.4142135623730950L);
    constexpr auto d4d3   = _Tp(1.333333333333333L);

    constexpr std::complex<_Tp> zone{1.0L, 0.0L};

    //  separate real and imaginary parts of zhat
    auto dx = std::real(zhat);
    auto dy = std::imag(zhat);
    auto dxabs = std::abs(dx);
    auto dyabs = std::abs(dy);

    //  if 1 - zhat**2 can be computed without overflow
    if (dxabs <= dinfsr && dyabs <= (dinfsr - done))
      {
	//  find max and min of abs(dx) and abs(dy)
	du = dxabs;
	dv = dyabs;
	if (du < dv)
          std::swap(du, dv);
        if (du >= dhalf)
          {
            if (dv > dinf / (dtwo * du))
	      {
		//  set completion code - unable to compute 1-zhat**2 and exit
		ier = 131;
		return;
	      }
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
    if (std::abs(znu) <= dinfsr)
      {
	znusq = znu * znu;
	z1dnsq = done / znusq;
	//  compute nu**(-2/3), nu**(-4/3), nu**(-1/3)
	znm4d3 = -std::log(znu);
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
    zxi = std::log(zone + ztemp) - std::log(zhat) - ztemp;
    zetm3h = d2d3 / zxi;

    //  compute principal value of ln(xi) and then adjust imaginary part
    zlnxi = std::log(zxi);

    //  prepare to adjust logarithm of xi to appropriate Riemann sheet
    auto dtemp = dzero;

    //  find adjustment necessary to get on proper Riemann sheet

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
	    if (std::imag(zxi) > dzero)
	      dtemp = -d2pi;
	    else
	      dtemp = d2pi;
	  }
      }

    //  Adjust logarithm of xi.
    zlnxi += dtemp;

    //  compute ln(zeta), zeta, zeta^(1/2), zeta^(-1/2)
    zlnzet = zlnxi + dlncon;
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
      major work here is in lzdiv.  A faster, but less safe
      implementation can be obtained without use of lzdiv.

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
    //  zexpp and zexpm are exp(2*dpi*i/3) and its reciprocal, respectively.
    constexpr auto zexpp = std::complex<_Tp>{-0.5L,  0.8660254037844386L};
    constexpr auto zexpm = std::complex<_Tp>{-0.5L, -0.8660254037844386L};

    ier = 0;

    if (lzdiv(zeta, znm2d3, zargm))
      {
        zargp = zexpp * zargm; // Correct?
        zargm = zexpm * zargm;
      }
    else
      ier = 133;

  }
  
#endif // HANKEL_TCC
