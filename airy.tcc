#ifndef AIRY_TCC
#define AIRY_TCC 1

template<typename _Tp>
  void
  iairy(const std::complex<_Tp> & z, _Tp deps,
        std::complex<_Tp> & zi1d3, std::complex<_Tp> & zim1d3,
        std::complex<_Tp> & zi2d3, std::complex<_Tp> & zim2d3);

template<typename _Tp>
  void
  kairy(const std::complex<_Tp> & z, _Tp deps,
        std::complex<_Tp> & zk1d3, std::complex<_Tp> & zk2d3,
        int & ier);

template<typename _Tp>
  void
  zcrary(const std::complex<_Tp> & z,
         std::complex<_Tp> & zf1d3, std::complex<_Tp> & zfm1d3,
         std::complex<_Tp> & zf2d3, std::complex<_Tp> & zfm2d3);

template<typename _Tp>
  void
  zasary(const std::complex<_Tp> & z,
         std::complex<_Tp> & zai, std::complex<_Tp> & zaipr);

template<typename _Tp>
  void
  zasaly(const std::complex<_Tp> & z,
         std::complex<_Tp> & zai, std::complex<_Tp> & zaipr);

/**
    @brief
    This subroutine computes the Airy function Ai(z) and its first
    derivative in the complex z-plane.

    The algorithm used exploits numerous representations of the
    Airy function and its derivative.  We record the representations
    here for reference

        	  sqrt(z)
    (1) Ai(z)	= ------- (I (xi) - I (xi))
        	     3      -1/3     1/3

        	  sqrt(z/3)
    (2) Ai(z)	= --------- K (xi)
        	     pi      1/3

        	    2/3  -5/6
        	   2	3
        	=  --------  z exp(-xi) U(5/6; 5/3; 2 xi)
        	   sqrt(pi)

        	  sqrt(z)
    (3) Ai(-z)  = ------- (J  (xi) + J (xi))
        	     3      -1/3      1/3

        	  z
    (4) Ai'(z)  = - (I (xi) - I  (xi))
        	  3   2/3      -2/3

        		 z
    (5) Ai'(z)  = - ---------- K (xi)
        	    pi*sqrt(3)  2/3

        	      2/3  -7/6
        	     4    3    2
        	=  - -------- Z  exp(-xi) U(7/6; 7/3; 2 xi)
        	     sqrt(pi)

        	   z
    (6) Ai'(-z) =  - (J (xi) - J  (xi)) ,
        	   3   2/3	-2/3

               2  3/2
    Where xi = - z    and U( ; ; ) is the confluent hypergeometric
               3
    Function as defined in

    @see Stegun, I. A. and Abramowitz, M., Handbook of Mathematical Functions,
      Natl. Bureau of Standards, AMS 55, pp 504-515, 1964.

    The asymptotic expansions derivable from these representations and
    Hankel's asymptotic expansions for the Bessel functions are used for
    large modulus of z.  The implementation has taken advantage of the
    error bounds given in

    @see Olver, F. W. J., Error Bounds for Asymptotic Expansions,
      with an Application to Cylinder Functions of Large Argument,
      in Asymptotic Solutions of Differential Equations (Wilcox, Ed.),
      Wiley and Sons, pp 163-183, 1964

    and

    @see Olver, F. W. J., Asymptotics and Special Functions, Academic Press,
      pp 266-268, 1974.

    For small modulus of z, a rational approximation is used.  This
    approximant is derived from

    Luke, Y. L., Mathematical Functions and their Approximations,
      Academic Press, pp 361-363, 1975.

    The identities given below are for Bessel functions of the first
    kind in terms of modified Bessel functions of the first kind are
    also used with the rational approximant.

    For moderate modulus of z, three techniques are used.  Two use
    a backward recursion algorithm with (1), (3), (4), and (6). The
    third uses the confluent hypergeometric representations given by
    (2) and (5).  The backward recursion algorithm generates values of
    the modified Bessel functions of the first kind of orders + or -
    1/3 and + or - 2/3 for z in the right half plane.  Values for
    the corresponding Bessel functions of the first kind are recovered
    via the identities

          J_nu(z) = exp(nu pi i/2) I_nu(z exp(-pi i/2)),
        	0 <= arg(z) <= pi/2
    and
          J_nu(z) = exp(-nu pi i/2) I_nu(z exp(pi i/2)),
               -pi/2 <= arg(z) < 0 .

    The particular backward recursion algorithm used is discussed in 

    @see Olver, F. W. J, Numerical solution of second-order linear
      difference equations, NBS J. RES., Series B, VOL 71B,
      pp 111-129, 1967.

    @see Olver, F. W. J. and Sookne, D. J., Note on backward recurrence
      algorithms, Math. Comp. Vol 26, No. 120, pp 941-947,
      OCT. 1972

    @see Sookne, D. J., Bessel Functions I and J of Complex Argument and
      Integer Order, NBS J. Res., Series B, Vol 77B, Nos 3 & 4,
      pp 111-113, July-December, 1973. 

    The following paper was also useful

    @see Cody, W. J., Preliminary report on software for the modified
      Bessel functions of the first kind, Applied Mathematics
      Division, Argonne National Laboratory, Tech. Memo. no. 357.

    A backward recursion algorithm is also used to compute the
    confluent hypergeometric function.  The recursion relations
    and a convergence theorem are given in

    @see Wimp, J., On the computation of Tricomi's psi function, Computing,
      Vol 13, pp 195-203, 1974.
 
    @param[in]  z	The argument at which the Airy function and its derivative are to be computed.
    @param[in]  deps	Relative error required.  At present, deps is used only in the 
    			backward recursion algorithms.
    @param[out]  zai	The value computed for Ai(z).
    @param[out]  zaipr  The value computed for Ai'(z).
 */
template<typename _Tp>
  void
  airy(const std::complex<_Tp> & z, _Tp deps,
       std::complex<_Tp> & zai, std::complex<_Tp> & zaipr, int & ier)
  {
    using dcmplx = std::complex<_Tp>;

    constexpr std::complex<_Tp>
         zepd6{8.660254037844386e-01, 5.0e-01},
         zempd6{8.660254037844386e-01, -5.0e-01},
         zepd3 {5.0e-01,  8.660254037844386e-01},
         zempd3{5.0e-01, -8.660254037844386e-01};
    constexpr _Tp
         dzero{0},
         d1d3  {3.33333333333333333e-01},
         d2d3  {6.66666666666666667e-01},
         dsqrt3{1.732050807568877e+00},
         dgm1d3{2.588194037928068e-01},
         dgm2d3{3.550280538878172e-01},
         d2g2d3{1.775140269439086e-01},
         drsqpi{2.820947917738781e-01};
    constexpr _Tp small{0.25}, dziacc{2}, big{15};

    ier = 0;

    //  Compute modulus of z for later use
    auto absz = std::abs(z);
    //  Check size of abs(z) and select appropriate methods
    if (absz < big)
      {
        dcmplx zi1d3, zim1d3, zi2d3, zim2d3,
               z1d3f, zm1d3f, z2d3f, zm2d3f;

        //  Moderate or small abs(z)
        //  Check for right or left half plane argument
        if (std::real(z) >= dzero)
          {
            //  Argument in closed right half plane
            //  Compute xi as defined in the representations in terms of Bessel functions
            auto zpwh = std::sqrt(z);
            auto zxi = z * zpwh;
            auto dzxir = d2d3 * std::real(zxi);
            auto dzxii = d2d3 * std::imag(zxi);
            zxi = dcmplx(dzxir, dzxii);

            //  Check for abs(z) too large for accuracy of representations (1) and (4)
            if (absz >= dziacc)
              {
        	//  Use rational approximation for modified Bessel functions of orders 1/3 and 2/3
        	kairy(zxi, deps, zai, zaipr, ier);
        	//  Recover Ai(z) and Ai'(z)
        	auto zp1d4c = std::sqrt(zpwh);
        	zxi = std::exp(-zxi);
        	zxi = drsqpi * zxi;
        	zai = zxi * (zai / zp1d4c);
        	zaipr = -zxi * zp1d4c * zaipr;

              }
            else
              {
        	//  Check for abs(z) small enough for rational approximation
        	if (absz <= small)
        	  {
        	    //  Use rational approximation along with (1) and (4)
        	    zcrary(z, zi1d3, zim1d3, zi2d3, zim2d3);
        	    //  Recover Ai(z) and Ai'(z)
        	    zim1d3 = dgm2d3 * zim1d3;
        	    zi1d3 = dgm1d3 * zi1d3;
        	    zai = zim1d3 - z * zi1d3;
        	    zim2d3 = dgm1d3 * zim2d3;
        	    zi2d3 = d2g2d3 * zi2d3;
        	    zaipr = z * z * zi2d3 - zim2d3;
        	  }
        	else
        	  {
                    //  Use backward recurrence along with (1) and (4)
                    iairy(zxi, deps, zi1d3, zim1d3, zi2d3, zim2d3);
                    //  Recover Ai(z) and Ai'(z)
                    zai = d1d3 * zpwh * (zim1d3 - zi1d3);
                    zaipr = d1d3 * z * (zi2d3 - zim2d3);
        	  }
              }
          }
        else
          {
            //  z lies in left half plane
            //  Compute xi as defined in the representations in terms of bessel functions
            auto zpwh = std::sqrt(-z);
            auto zxi = -z * zpwh;
            auto dzxir = d2d3 * std::real(zxi);
            auto dzxii = d2d3 * std::imag(zxi);
            zxi = dcmplx(dzxir, dzxii);
            dcmplx z2xi;
            //  Set up arguments to recover bessel functions of the first kind in (3) and (6)
            if (dzxii >= dzero)
              {
        	//  Argument lies in upper half plane, so use appropriate identity
        	z2xi = std::complex<_Tp>(dzxii, -dzxir);
        	z1d3f = zepd6;
        	zm1d3f = zempd6;
        	z2d3f = zepd3;
        	zm2d3f = zempd3;
              }
            else
              {
        	//  Argument lies in lower half plane, so use appropriate identity
        	z2xi = std::complex<_Tp>(-dzxii, dzxir);
        	z1d3f = zempd6;
        	zm1d3f = zepd6;
        	z2d3f = zempd3;
        	zm2d3f = zepd3;
              }

            //  Use approximation depending on size of z
            if (absz <= small)
              {
        	//  Use rational approximation
        	zxi = -z;
        	zcrary(z, zi1d3, zim1d3, zi2d3, zim2d3);
        	//  Recover Ai(z) and Ai'(z)
        	zim1d3 = dgm2d3 * zim1d3;
        	zi1d3 = dgm1d3 * zi1d3;
        	zai = zim1d3 - z * zi1d3;
        	//zai = zm1d3f * zim1d3 + z * z1d3f * zi1d3

        	zim2d3 = dgm1d3 * zim2d3;
        	zi2d3 = d2g2d3 * zi2d3;
        	zaipr = z * z * zi2d3 - zim2d3;
        	//zaipr = z * z * z2d3f * zi2d3 - zm2d3f * zim2d3
              }
            else
              {
        	//  Use backward recurrence
        	iairy(z2xi, deps, zi1d3, zim1d3, zi2d3, zim2d3);
        	//  Recover Ai(z) and Ai'(z)
        	zai = d1d3 * zpwh * (zm1d3f * zim1d3 + z1d3f * zi1d3);
        	zaipr = d1d3 * z * (zm2d3f * zim2d3 - z2d3f * zi2d3);
              }
          }
      }
    else
      {
        //  abs(z) is large; check arg(z) to see which asymptotic form is appropriate
        if (std::real(z) >= dzero
            || std::abs(std::imag(z)) >= -dsqrt3 * std::real(z))
        {
          //  abs(arg(z)) <= 2*dpi/3  -  use asymptotic expansion for this region
          zasary(z, zai, zaipr);
        }
        else
        {
          //  abs(arg(-z)) < dpi/3  -  use asymptotic expansion for this region
          zasaly(z, zai, zaipr);
        }
      }
    return;
}


/**
    Compute the modified Bessel functions of the first kind orders 
    + or - 1/3 and + or - 2/3 needed to compute the Airy functions
    and their derivatives from their representation in terms of the
    modified Bessel functions.  This program is only used for z
    less than two in modulus and in the closed right half plane.
    This stems from the fact that the values of the modified 
    Bessel functions occuring in the representations of the Airy
    functions and their derivatives are almost equal for z large
    in the right half plane.  This means that loss of significance
    occurs if these representations are used for z to large in
    magnitude.  This algorithm is also not used for z too small,
    since a low order rational approximation can be used instead.

    This subroutine is an implementation of a modified version of
    Miller's algorithm for computation by backward recurrence
    from the recurrence relation

      I_(nu-1) = (2*nu/z)*I_nu + I_(nu+1)

    satisfied by the modified Bessel functions of the first kind.
    the normalization relationship used is
           nu z 
      (z/2)  e  	         INF  (k+nu)*Gamma(2 nu+k)
      -----------  = I_nu(z) + 2 SUM  -------------------- I_(nu+k)(z) .
      Gamma(nu+1)                k=1   k! Gamma(1 + 2 nu)   

    This modification of the algorithm is given in part in

    Olver, F. W. J. and Sookne, D. J., Note on Backward Recurrence
      Algorithms, Math. of Comp., Vol. 26, no. 120, Oct. 1972.

    And further elaborated for the Bessel functions in
  
    Sookne, D. J., Bessel Functions I and J of Complex Argument 
      and Integer Order, J. Res. NBS - Series B, Vol 77B, Nos.
      3 & 4, July-December, 1973.

    Insight was also gained from

    Cody, W. J., Preliminary Report on Software for the Modified
      Bessel Functions of the First Kind, Argonne National
      Laboratory, Applied Mathematics Division, Tech. Memo. 
      No. 357, August, 1980.

    Cody implements the algorithm of Sookne for fractional order
    and nonnegative real argument.  Like Cody, we do not change
    the convergence testing mechanism in any substantial way.
    However, we do trim the overhead by making the additional
    assumption that performing the convergence test for the
    functions of order 2/3 will suffice for order 1/3 as well.
    This assumption has not been established by rigourous
    analysis at this time.  Two additional changes have been
    made for the sake of speed.  First, the strong convergence
    criteria is computed in single precision since magnitude is
    the most important thing here.  Second, the tests are
    performed in the 1-norm instead of the usual Euclidean
    norm used in the complex plane.
    To insure the validity of the results, the inequality

      abs(x) + abs(y) <= sqrt(2) sqrt(x^2 + y^2) 

    was used to modify the convergence tests.

    Note also that for the sake of speed and the fact that this
    subroutine will be driven by another, checks that are not
    absolutely necessary are not made.  Under these assumptions
    an error return is not needed.

    Arguments
    @param[in]   z      The argument at which the modified Bessel
                        functions computed by this program are to be
                        evaluated.
    @param[in]   deps   The maximum relative error required in the results.
    @param[out]  zi1d3   The value of I_1/3 (z).
    @param[out]  zim1d3  The value of I_(-1/3) (z).
    @param[out]  zi2d3   The value of I_2/3 (z).
    @param[out]  zim2d3  The value of I_(-2/3) (z).
 */
template<typename _Tp>
  void
  iairy(const std::complex<_Tp> & z, _Tp deps,
        std::complex<_Tp> & zi1d3, std::complex<_Tp> & zim1d3,
        std::complex<_Tp> & zi2d3, std::complex<_Tp> & zim2d3)
  {
    using dcmplx = std::complex<_Tp>;

    constexpr _Tp sone{1}, stwo{2};
    constexpr dcmplx zero{0}, zone{1};
    constexpr _Tp done{1.0e+00}, dtwo{2.0e+00}, dhalf{5.0e-01},
    	 d1d3  {3.333333333333333e-01}, d2d3  {6.666666666666667e-01},
    	 d4d3  {1.333333333333333e+00}, d5d3  {1.666666666666667e+00},
    	 d8d3  {2.666666666666667e+00}, d10d3 {3.333333333333333e+00},
    	 d14d3 {4.666666666666667e+00}, d16d3 {5.333333333333333e+00},
    	 dgm4d3{8.929795115692492e-01}, dgm5d3{9.027452929509336e-01},
    	 d2sqr2{2.828427124746190e+01};

    //  compute 1/z for use in recurrence for speed and abs(z)
    auto dzr = std::real(z);
    auto dzi = std::imag(z);
    auto dzrabs = std::abs(dzr);
    auto dziabs = std::abs(dzi);
    auto du = std::max(dzrabs, dziabs);
    auto dv = std::min(dzrabs, dziabs);
    auto d1dzi = (done + (dv / du) * (dv / du));
    auto dmodz = du * std::sqrt(d1dzi);
    d1dzi = du * d1dzi;
    auto d1dzr = (dzr / du) / d1dzi;
    d1dzi = -(dzi / du) / d1dzi;
    dcmplx z1dz(d1dzr, d1dzi);

    //  initialize for forward recursion based on order 2/3
    int n = 0;
    auto d2n = _Tp(n + n) + d4d3;
    auto zplst2 = zone;
    auto zp2 = d2n * std::complex<_Tp>(d1dzr, d1dzi);

    //  calculate weak convergence test and set flag for weak convergence loop
    auto dtest = d2sqr2 / deps;
    bool lstcnv = false;


    //  loop until weak and strong convergence tests satisfied when recurring forward
    while (true)
      {
	do
	  {
    	    //  Update n dependent quantities
    	    ++n;
    	    d2n += dtwo;
    	    //  Interchange values
    	    auto zpold2 = zplst2;
    	    zplst2 = zp2;
    	    //  Recur forward one step
    	    zp2 = z1dz * d2n * zplst2 + zpold2;

    	    //  Check if convergence test (in 1-norm) satisfied
	  }
	while (std::abs(std::real(zp2)) + std::abs(std::imag(zp2)) < dtest);

	//  if strong convergence, then weak and strong convergence
	if (lstcnv)
    	  break;

	//  Calculate strong convergence test in single precision.  see
	//  the Olver and Sookne papers cited for details.
	auto spr = std::real(zp2);
	auto spi = std::imag(zp2);
	auto splstr = std::real(zplst2);
	auto splsti = std::imag(zplst2);
	//  Compute scale factor to avoid possible overflow
	auto slamn = std::max(std::abs(spr), std::abs(spi));
	//  Compute the k-sub-n of strong convergence lemma
	auto skn = std::sqrt(((spr / slamn) * (spr / slamn)
    			    + (spi / slamn) * (spi / slamn))
    			   / ((splstr / slamn) * (splstr / slamn)
    			    + (splsti / slamn) * (splsti / slamn)));
	//  Compute quantity needed for lambda-sub-n of strong convergence lemma
	slamn = _Tp(n + 1) / dmodz;
	//  Determine appropriate value for rho-sub-n of lemma
	if (skn + sone / skn > stwo * slamn)
    	  skn = slamn + std::sqrt(slamn * slamn - sone);
	//  Compute test value - sqrt(2) multiple already included
	dtest *= std::sqrt(skn - sone / skn);
	//  Set strong convergence test flag
	lstcnv = true;
    }

    //  Prepare for backward recurrence for both orders 1/3 and 2/3
    auto dn = _Tp(n);
    ++n;
    d2n = _Tp(n + n);
    auto zplst1 = zero;
    zplst2 = zero;
    //  Carefully compute 1/zp2 to avoid overflow in complex divide
    auto dzp2r = std::real(zp2);
    auto dzp2i = std::imag(zp2);
    dzrabs = std::abs(dzp2r);
    dziabs = std::abs(dzp2i);
    du = std::max(dzrabs, dziabs);
    dv = std::min(dzrabs, dziabs);
    d1dzi = du * (done + (dv / du) * (dv / du));
    d1dzr = (dzp2r / du) / d1dzi;
    d1dzi = -(dzp2i / du) / d1dzi;
    auto zp1 = dcmplx(d1dzr, d1dzi);
    zp2 = zp1;
    //  Set up n dependent parameters used in normalization sum
    auto dnpn1 = dn + d1d3;
    auto dnpn2 = dn + d2d3;
    auto dnp2n1 = (dn - done) + d2d3;
    auto dnp2n2 = (dn - done) + d4d3;
    //  Initialize normalization sum
    auto dfac1 = dnpn1 * dnp2n1 / dn;
    auto zsum1 = dfac1 * zp1;
    auto dfac2 = dnpn2 * dnp2n2 / dn;
    auto zsum2 = dfac2 * zp2;
    //  Set ending loop index to correspond to k=1 term of the
    //  normalization relationship
    auto nend = n - 3;

    //  if backward recurrence loop will be nontrivial
    if (nend > 0)
      {
	//  Loop until backward recursion to k=1 term of normalization
	for (int l = 1; l <= nend; ++l)
	  {
    	    //  Update n dependent quantities
    	    --n;
    	    d2n = d2n - dtwo;
    	    dfac1 = d2n + d2d3;
    	    dfac2 = d2n + d4d3;
    	    //  Interchanges for order 1/3 recurrence
    	    auto zpold1 = zplst1;
    	    zplst1 = zp1;
    	    //  Recur back one step for order 1/3
    	    zp1 = z1dz * dfac1 * zplst1 + zpold1;
    	    //  Interchanges for order 2/3 recurrence
    	    auto zpold2 = zplst2;
    	    zplst2 = zp2;
    	    //  Recur back one step for order 2/3
    	    zp2 = z1dz * dfac2 * zplst2 + zpold2;
    	    //  Update quantities for computing normalization sums
    	    dn = dn - done;
    	    dnpn1 = dn + d1d3;
    	    dnp2n1 = dn - d1d3;
    	    dnpn2 = dn + d2d3;
    	    dnp2n2 = dnpn1;
    	    dfac1 = dnp2n1 / dn;
    	    dfac2 = dnp2n2 / dn;
    	    //  Update normalization sums
    	    zsum1 += dnpn1 * zp1;
    	    zsum1 *= dfac1;
    	    zsum2 += dnpn2 * zp2;
    	    zsum2 *= dfac2;
	  }
      }

    //  Perform last two recurrence steps for order 1/3
    auto zpold1 = zplst1;
    zplst1 = zp1;
    zp1 = z1dz * d14d3 * zplst1 + zpold1;
    zsum1 += d4d3 * zp1;
    zpold1 = zplst1;
    zplst1 = zp1;
    zp1 = d8d3 * zplst1 * z1dz + zpold1;
    zsum1 = (zsum1 + zsum1) + zp1;

    //  Compute scale factor and scale results for order 1/3 case
    auto zd2pow = std::pow(dhalf * dcmplx(dzr, dzi), -d1d3);
    zpold1 = zd2pow * std::exp(-z);
    zsum1 *= zpold1 * dgm4d3;
    zplst1 /= zsum1;
    zi1d3 = zp1 / zsum1;

    //  Perform last two recurrence steps for order 2/3
    auto zpold2 = zplst2;
    zplst2 = zp2;
    zp2 = d16d3 * zplst2 * z1dz + zpold2;
    zsum2 += d5d3 * zp2;
    zpold2 = zplst2;
    zplst2 = zp2;
    zp2 = d10d3 * zplst2 * z1dz + zpold2;
    zsum2 = (zsum2 + zsum2) + zp2;

    //  Compute scale factor and scale results for order 2/3 case
    zsum2 *= zd2pow * zpold1 * dgm5d3;
    zplst2 /= zsum2;
    zi2d3 = zp2 / zsum2;

    //  Recur back one step from order 1/3 to get order -2/3
    zim2d3 = d2d3 * zi1d3 * z1dz + zplst1;

    //  Recur back one step from order 2/3 to get order -1/3
    zim1d3 = d4d3 * zi2d3 * z1dz + zplst2;

    return;
  }


/**
    @brief
    Compute approximations to the modified Bessel functions of the
    second kind of orders 1/3 and 2/3 for moderate arguments.
    More specifically, the program computes

      E_nu(Z) = exp(z) sqrt(2 z/pi) K_nu(z), for nu = 1/3 and nu = 2/3.
       			   

    This subprogram uses a rational approximation given in

    Luke, Y.L., Mathematical functions and their approximations,
      Academic Press, pp 366-367, 1975.

    Though the approximation converges in abs(arg(z)) <= pi,
    The convergence weakens as abs(arg(z)) increases.  Also, in
    the case of real order between 0 and 1, convergence weakens
    as the order approaches 1.  For these reasons, we only use
    this code for abs(arg(z)) <= 3*pi/4 and the convergence test
    is performed only for order 2/3.

    The coding of this subprogram is also influenced by the fact
    that it will only be used for about 2 <= abs(z) <= 15.
    Hence, certain considerations of overflow, underflow, and
    loss of significance are unimportant for our purpose.

    Arguments
      z      The value for which the quantity e-sub-nu is to
             be computed.  it is recommended that abs(z) not be
             too small and that abs(arg(z)) <= 3*pi/4.
      deps   The maximum relative error allowable in the computed
             results.  the relative error test is based on the
             comparison of successive iterates.
      zk1d3  The value computed for E-sub-(1/3) of z.
      zk2d3  The value computed for E-sub-(2/3) of z.
      ier    A completion code.
             ier = 0 indicates normal completion
             ier = 129, convergence failed in 100 iterations
            

    @note According to published information about the behaviour of the error for orders
          1/3 and 2/3, ier = 129 should never occur for the domain of z that we recommend.
          indeed, in the worst case, say, z=2 and arg(z) = 3*pi/4, we expect 20 iterations
          to give 7 or 8 decimals of accuracy.
 */
template<typename _Tp>
  void
  kairy(const std::complex<_Tp> & z, _Tp deps,
        std::complex<_Tp> & zk1d3, std::complex<_Tp> & zk2d3,
        int & ier)
  {
    using dcmplx = std::complex<_Tp>;

    _Tp an1,
        danm1, danm2, dqzr, dqzi;

    constexpr _Tp dtwo{2},
                  deight{8},
		  d16{16},
		  d24{24},
                  dthree{3},
		  dfive{5},
		  delti{32},
        	  dan1i{ 4.8555555555555555555e+01L},
		  dan2i{ 4.7222222222222222222e+01L},
        	  dp12i{ 3.1444444444444444444e+01L},
		  dp22i{ 3.2777777777777777777e+01L},
        	  dp13i{-9.2592592592592592592e-01L},
		  dp23i{ 1.2962962962962962962e+00L},
        	  dp11i{-7.9074074074074074074e+01L},
		  dp21i{-8.1296296296296296296e+01L};

    constexpr std::complex<_Tp> zone{1};

    constexpr _Tp
    dfco[8]
    {144, 77, 62208, 95472, 17017, 65, 90288, 13585};

    constexpr _Tp
    dphico[6]
    {67, 91152, 12697, 79, 96336, 19633};

    ier = 0;

    //  Initialize polynomials for recurrence
    auto zf10 = zone;
    auto zf20 = zone;
    auto zf11 = zone + dfco[0] * z / dfco[1];
    auto zf12 = z * (dfco[2] * z + dfco[3]);
    zf12 = zone + zf12 / dfco[4];
    auto zf21 = zone + dfco[0] * z / dfco[5];
    auto zf22 = z * (dfco[2] * z + dfco[6]);
    zf22 = zone + zf22 / dfco[7];

    auto zphi10 = zone;
    auto zphi20 = zone;
    auto zphi11 = dcmplx((dfco[0] * std::real(z) + dphico[0]) / dfco[1],
                     std::imag(zf11));
    auto zphi12 = z * (dfco[2] * z + dphico[1]);
    zphi12 = (zphi12 + dphico[2]) / dfco[4];
    auto zphi21 = dcmplx((dfco[0] * std::real(z) + dphico[3]) / dfco[5],
                     std::imag(zf21));
    auto zphi22 = z * (dfco[2] * z + dphico[4]);
    zphi22 = zone + (zphi22 + dphico[5]) / dfco[7];

    //  Initialize for recursion
    auto zratol = zphi22 / zf22;
    auto zrat1 = zphi12 / zf12;
    auto delt = delti;
    auto dan1 = dan1i;
    auto dan2 = dan2i;
    auto dp11 = dp11i;
    auto dp12 = dp12i;
    auto dp13 = dp13i;
    auto dp21 = dp21i;
    auto dp22 = dp22i;
    auto dp23 = dp23i;
    auto deta = d24;
    auto dgamm = dthree;
    auto dgam = dfive;
    auto dq = d16 * dgam;

    //  Loop until maximum iterations used or convergence
    for (int i = 1; i < 100; ++i)
      {
	//  Evaluate next term in recurrence for order 1/3 polynomials
	auto dqz = dq * z;
	auto zfact1 = dqz - dp11;
	auto zfact2 = dqz - dp12;
	auto zf13 = zfact1 * zf12 + zfact2 * zf11 - dp13 * zf10;
	zf13 /= dan1;
	auto zphi13 = zfact1 * zphi12 + zfact2 * zphi11 - dp13 * zphi10;
	zphi13 /= dan1;

	//  Evaluate next term in recurrence for order 2/3 polynomials
	zfact1 = dqz - dp21;
	zfact2 = dqz - dp22;
	auto zf23 = zfact1 * zf22 + zfact2 * zf21 - dp23 * zf20;
	zf23 = zf23 / dan2;
	auto zphi23 = zfact1 * zphi22 + zfact2 * zphi21 - dp23 * zphi20;
	zphi23 = zphi23 / dan2;

	//  check for convergence
	auto zratnw = zphi23 / zf23;
	zrat1 = zphi13 / zf13;

	if (std::abs(zratnw - zratol) < deps * std::abs(zratnw))
	  {
	   //  Convergence.
	    zk2d3 = zratnw;
	    zk1d3 = zphi13 / zf13;
	    return;
	  }

	//  Prepare for next iteration
	zratol = zratnw;
	zf20 = zf21;
	zf21 = zf22;
	zf22 = zf23;
	zphi20 = zphi21;
	zphi21 = zphi22;
	zphi22 = zphi23;
	zf10 = zf11;
	zf11 = zf12;
	zf12 = zf13;
	zphi10 = zphi11;
	zphi11 = zphi12;
	zphi12 = zphi13;
	delt = delt + d24;
	dp12 = dp12 + delt;
	dp22 = dp22 + delt;
	deta = deta + deight;
	dan1 = dan1 + deta;
	dan2 = dan2 + deta;
	danm1 = dan1 - delt - d16;
	danm2 = dan2 - delt - d16;
	dgamm = dgam;
	dgam = dgam + dtwo;
	dp23 = -dgam / dgamm;
	dp13 = dp23 * danm1;
	dp23 = dp23 * danm2;
	dp11 = -dan1 - dp12 - dp13;
	dp21 = -dan2 - dp22 - dp23;
	dq = d16 * dgam;
      }

    //  Maximum iterations exceeded
    ier = 129;
    return;
  }



/**
    @brief
      This subroutine computes rational approximations to the
      hypergeometric functions related to the modified Bessel
      functions of orders nu = + or - 1/3 and + or - 2/3.  That is,
      A(z)/B(z), Where A(z) and B(z) are cubic polynomials with
      real coefficients, approximates

	Gamma(nu+1)		       2 
	----------- I (Z) =  F (;nu+1;z /4) ,
	 (z/2)**nu   nu     0 1
     
      Where the function on the right is a generalized Gaussian
      hypergeometric function.  For abs(z) <= 1/4  and
      abs(arg(z)) <= pi/2, the approximations are accurate to
      about 16 decimals.

      For further details including the four term recurrence
      relation satisfied by the numerator and denominator poly-
      nomials in the higher order approximants, see

      Luke, Y.L., Mathematical Functions and their Approximations,
      Academic Press, pp 361-363, 1975.

      An asymptotic expression for the error is given as well as
      other useful expressions in the event one wants to extend
      this subroutine to incorporate higher order approximants.

      Note also that for the sake of speed and the fact that this
      subroutine will be driven by another, checks that are not
      absolutely necessary are not made.  Under these assumptions
      an error return is not needed.

      ARGUMENTS
	z      Double precision complex input variable set equal
	       to the argument at which the hypergeometric given
	       above is to be evaluated.  Since the approximation
	       is of fixed order, abs(z) must be small to insure
	       sufficient accuracy of the computed results.
	zf1d3  Double precision complex output variable containing
	       the approximate value of the hypergeometric
	       function related to the modified Bessel function
	       of order 1/3.
	zfm1d3 Double precision complex output variable containing
	       the approximate value of the hypergeometric
	       function related to the modified Bessel function
	       of order -1/3.
	zf2d3  Double precision complex output variable containing
	       the approximate value of the hypergeometric
	       function related to the modified Bessel function
	       of order 2/3.
	zfm2d3 Double precision complex output variable containing
	       the approximate value of the hypergeometric
	       function related to the modified Bessel function
	       of order -2/3.

    Date of last revision
      May 2, 1986

    Author
      Rick A. Whitaker
 */
template<typename _Tp>
  void
  zcrary(const std::complex<_Tp> & z,
         std::complex<_Tp> & zf1d3, std::complex<_Tp> & zfm1d3,
         std::complex<_Tp> & zf2d3, std::complex<_Tp> & zfm2d3)
  {
    using dcmplx = std::complex<_Tp>;

    constexpr _Tp dtwo{2};
    constexpr dcmplx zone{1};

    //  dsmall is the 1/3 root of the smallest magnitude
    //  floating-point number representable on the machine
    constexpr _Tp dsmall = 1.0e-12;

    constexpr _Tp da1d3[4]{  81, 32400, 2585520, 37920960 };
    constexpr _Tp db1d3[4]{ -35, 5040, -574560, 37920960 };
    constexpr _Tp dam1d3[4]{ 81, 22680, 1156680, 7711200 };
    constexpr _Tp dbm1d3[4]{ -10, 1260, -128520, 7711200 };
    constexpr _Tp da2d3[4]{ 162, 75735, 7270560, 139352400 };
    constexpr _Tp db2d3[4]{ -110, 16830, -2019600, 139352400 };
    constexpr _Tp dam2d3[4]{ 162, 36855, 1415232, 4481568 };
    constexpr _Tp dbm2d3[4]{ -7, 819, -78624, 4481568 };

    //  Check to see if z**3 will underflow and act accordingly

    if (std::abs(z) < dsmall)
      {
	//  All ratios are 1 
	zf1d3  = zone;
	zfm1d3 = zone;
	zf2d3  = zone;
	zfm2d3 = zone;
      }
    else
      {
	//  Initialize argument dependent quantities used throughout
	dcmplx zzz = z * z * z;
	_Tp dx = std::real(zzz);
	_Tp dy = std::imag(zzz);
	_Tp dr = dtwo * dx;
	_Tp ds = dx * dx + dy * dy;

	//  All of the following polynomial evaluations are done using
	//  a modified of Horner's rule which exploits the fact that
	//  the polynomial coefficients are all real.  the algorithm is
	//  discussed in detail in

	//  Knuth, D. E., The Art of Computer Programming: Seminumerical
	//  Algorithms (Vol. 2), Addison-Wesley, pp 423-424, 1969.

	//  If n is the degree of the polynomial, n - 3 multiplies are
	//  saved and 4 * n - 6 additions are saved.

	_Tp dtwo;

	//  Evaluate numerator polynomial for nu=1/3 approximant
	auto dal = da1d3[0];
	auto dt  = ds * dal;
	dal = da1d3[1] + dr * dal;
	auto dbe = da1d3[2] - dt;
	dt  = ds * dal;
	dal = dbe + dr * dal;
	dbe = da1d3[3] - dt;
	zf1d3 = dal * dcmplx(dx, dy) + dbe;

	//  Evaluate denominator polynomial for nu=1/3 approximant
	//  and compute ratio of numerator and denominator
	dal = db1d3[0];
	dt  = ds * dal;
	dal = db1d3[1] + dr * dal;
	dbe = db1d3[2] - dt;
	dt  = ds * dal;
	dal = dbe + dr * dal;
	dbe = db1d3[3] - dt;
	zf1d3 /= dal * dcmplx(dx, dy) + dbe;

	//  Evaluate numerator polynomial for nu=-1/3 approximant
	dal = dam1d3[0];
	dt  = ds * dal;
	dal = dam1d3[1] + dr * dal;
	dbe = dam1d3[2] - dt;
	dt  = ds * dal;
	dal = dbe + dr * dal;
	dbe = dam1d3[3] - dt;
	zfm1d3 = dal * dcmplx(dx, dy) + dbe;
	//  Evaluate denominator polynomial for nu=-1/3 approximant
	//  and compute ratio of numerator and denominator
	dal = dbm1d3[0];
	dt  = ds * dal;
	dal = dbm1d3[1] + dr * dal;
	dbe = dbm1d3[2] - dt;
	dt  = ds * dal;
	dal = dbe + dr * dal;
	dbe = dbm1d3[3] - dt;
	zfm1d3 /= dal * dcmplx(dx, dy) + dbe;

	//  Evaluate numerator polynomial for nu=2/3 approximant
	dal = da2d3[0];
	dt  = ds * dal;
	dal = da2d3[1] + dr * dal;
	dbe = da2d3[2] - dt;
	dt  = ds * dal;
	dal = dbe + dr * dal;
	dbe = da2d3[3] - dt;
	zf2d3 = dal * dcmplx(dx, dy) + dbe;

	//  Evaluate denominator polynomial for nu=2/3 approximant
	//  and compute ratio of numerator and denominator
	dal = db2d3[0];
	dt  = ds * dal;
	dal = db2d3[1] + dr * dal;
	dbe = db2d3[2] - dt;
	dt  = ds * dal;
	dal = dbe + dr * dal;
	dbe = db2d3[3] - dt;
	zf2d3 /= dal * dcmplx(dx, dy) + dbe;

	//  Evaluate numerator polynomial for nu=-2/3 approximant
	dal = dam2d3[0];
	dt  = ds * dal;
	dal = dam2d3[1] + dr * dal;
	dbe = dam2d3[2] - dt;
	dt  = ds * dal;
	dal = dbe + dr * dal;
	dbe = dam2d3[3] - dt;
	zfm2d3 = dal * dcmplx(dx, dy) + dbe;

	//  Evaluate denominator polynomial for nu=-2/3 approximant
	//  and compute ratio of numerator and denominator
	dal = dbm2d3[0];
	dt  = ds * dal;
	dal = dbm2d3[1] + dr * dal;
	dbe = dbm2d3[2] - dt;
	dt  = ds * dal;
	dal = dbe + dr * dal;
	dbe = dbm2d3[3] - dt;
	zfm2d3 /= dal * dcmplx(dx, dy) + dbe;
      }

    return;
  }


/**
    Purpose
      This subroutine evaluates Ai(z) and Ai'(z) from their asymptotic
      expansions for abs(arg(z)) < 2*pi/3.  For speed, the number
      of terms needed to achieve about 16 decimals accuracy is tabled
      and determined from abs(z).

      Note that for the sake of speed and the fact that this subroutine
      is to be called by another, checks for valid arguments are not
      made.  Hence, an error return is not needed.

    Arguments
      z      Double precision complex input variable set equal to the
	     value at which Ai(z) and its derivative are to be eval-
	     uated.  this subroutine assumes abs(z) > 15 and
	     abs(arg(z)) < 2*pi/3.
      zai    Double precision complex output variable containing the
	     value computed for Ai(z).
      zaipr  Double precision complex output variable containing the
	     value computed for Ai'(z).

    Date of last revision
      May 3, 1986

    Author
      Rick A. Whitaker
 */
template<typename _Tp>
  void
  zasary(const std::complex<_Tp> & z,
         std::complex<_Tp> & zai, std::complex<_Tp> & zaipr)
  {
    using dcmplx = std::complex<_Tp>;

    _Tp dsdata;

    constexpr _Tp d2d3 = 6.666666666666667e-01;
    constexpr _Tp dpmhd2 = 2.820947917738781e-01;
    constexpr std::complex<_Tp> zmone{-1};
    constexpr int ntermx = 15;
    constexpr int nterms[5]{ 15, 12, 11, 11, 9 };

    //  coefficients for the expansion
    constexpr _Tp
    dck[15]
    {
      0.5989251356587907e+05,
      0.9207206599726415e+04,
      0.1533169432012796e+04,
      0.2784650807776026e+03,
      0.5562278536591708e+02,
      0.1234157333234524e+02,
      0.3079453030173167e+01,
      0.8776669695100169e+00,
      0.2915913992307505e+00,
      0.1160990640255154e+00,
      0.5764919041266972e-01,
      0.3799305912780064e-01,
      0.3713348765432099e-01,
      0.6944444444444444e-01,
      0.1000000000000000e+01
    };

    constexpr _Tp
    ddk[15]
    {
      -0.6133570666385206e+05,
      -0.9446354823095932e+04,
      -0.1576357303337100e+04,
      -0.2870332371092211e+03,
      -0.5750830351391427e+02,
      -0.1280729308073563e+02,
      -0.3210493584648621e+01,
      -0.9204799924129446e+00,
      -0.3082537649010791e+00,
      -0.1241058960272751e+00,
      -0.6266216349203231e-01,
      -0.4246283078989483e-01,
      -0.4388503086419753e-01,
      -0.9722222222222222e-01,
       0.1000000000000000e+01
    };

    //  compute -xi and z**(1/4)
    auto zpw1d4 = std::sqrt(z);
    auto zxim = z * zpw1d4;
    zxim *= d2d3;
    zpw1d4 = std::sqrt(zpw1d4);

    //  Compute outer factors in the expansions
    auto zoutpr = std::exp(-zxim);
    zoutpr *= dpmhd2;
    auto zout = zoutpr / zpw1d4;
    zoutpr *= -zpw1d4;

    //  Determine number of terms to use
    auto nterm = nterms[std::min(5, (int(std::abs(z)) - 10) / 5)];
    //  Initialize for modified horner's rule evaluation of sums
    //  it is assumed that at least three terms are used
    zxim = zmone / zxim;
    auto dx = std::real(zxim);
    auto dy = std::imag(zxim);
    auto dr = dx + dx;
    auto ds = dx * dx + dy * dy;
    auto ndx = ntermx - nterm + 1;
    auto dal = dck[ndx];
    auto dalpr = ddk[ndx];
    ++ndx;
    auto dbe = dck[ndx];
    auto dbepr = ddk[ndx];
    ++ndx;

    //  Loop until components contributing to sums are computed
    for (int k = ndx; k <= ntermx; ++k)
    {
      auto dsdata = ds * dal;
      dal = dbe + dr * dal;
      dbe = dck[k] - dsdata;
      dsdata = ds * dalpr;
      dalpr = dbepr + dr * dalpr;
      dbepr = ddk[k] - dsdata;
    }

    //  Complete evaluation of the airy functions
    zai = zout * dal * dcmplx(dx, dy) + dbe;
    zaipr = zoutpr * dalpr * dcmplx(dx, dy) + dbepr;

    return;
  }



/**
    Purpose
      This subroutine evaluates Ai(z) and Ai'(z) from their asymptotic
      expansions for abs(arg(-z)) < pi/3.  For speed, the number
      of terms needed to achieve about 16 decimals accuracy is tabled
      and determined from abs(z).

      Note that for the sake of speed and the fact that this subroutine
      is to be called by another, checks for valid arguments are not
      made.  Hence, an error return is not needed.

    Arguments
      z      Double precision complex input variable set equal to the
	     value at which Ai(z) and its derivative are to be eval-
	     uated.  This subroutine assumes abs(z) > 15 and
	     abs(arg(-z)) < pi/3.
      zai    Double precision complex output variable containing the
	     value computed for Ai(z).
      zaipr  Double precision complex output variable containing the
	     value computed for Ai'(z).

    Date of last revision
      May 4, 1986

    Author
      Rick A. Whitaker
 */
template<typename _Tp>
  void
  zasaly(const std::complex<_Tp> & z,
         std::complex<_Tp> & zai, std::complex<_Tp> & zaipr)
  {
    using dcmplx = std::complex<_Tp>;

    constexpr _Tp d2d3{6.666666666666667e-01};
    constexpr _Tp d9d4{2.25e+00};
    constexpr _Tp dpimh{5.641895835477563e-01};
    constexpr _Tp dpid4{7.853981633974483e-01};
    constexpr std::complex<_Tp> zone{1};
    constexpr int ntermx = 9;
    constexpr int nterms[5]{ 9, 7, 6, 6, 5 };

    //  coefficients for the expansion
    constexpr _Tp
    dckc[9]
    {
      0.2519891987160237e+08,
      0.4195248751165511e+06,
      0.9207206599726415e+04,
      0.2784650807776026e+03,
      0.1234157333234524e+02,
      0.8776669695100169e+00,
      0.1160990640255154e+00,
      0.3799305912780064e-01,
      0.6944444444444444e-01
    };
    constexpr _Tp
    dcks[9]
    {
      0.3148257417866826e+07,
      0.5989251356587907e+05,
      0.1533169432012796e+04,
      0.5562278536591708e+02,
      0.3079453030173167e+01,
      0.2915913992307505e+00,
      0.5764919041266972e-01,
      0.3713348765432099e-01,
      0.1000000000000000e+01
    };

    constexpr _Tp
    ddks[9]
    {
      -0.2569790838391133e+08,
      -0.4289524004000691e+06,
      -0.9446354823095932e+04,
      -0.2870332371092211e+03,
      -0.1280729308073563e+02,
      -0.9204799924129446e+00,
      -0.1241058960272751e+00,
      -0.4246283078989483e-01,
      -0.9722222222222222e-01
    };
    constexpr _Tp
    ddkc[9]
    {
      -0.3214536521400865e+07,
      -0.6133570666385206e+05,
      -0.1576357303337100e+04,
      -0.5750830351391427e+02,
      -0.3210493584648621e+01,
      -0.3082537649010791e+00,
      -0.6266216349203231e-01,
      -0.4388503086419753e-01,
       0.1000000000000000e+01
    };

    //  Set up working value of z
    auto zwk = -z;
    //  Compute xi and z**(1/4)
    auto zpw1d4 = std::sqrt(zwk);
    auto zxi = zwk * zpw1d4;
    zxi = d2d3 * zxi;
    zpw1d4 = std::sqrt(zpw1d4);

    //  Compute sine and cosine factors in the expansions.
    auto zxiarg = zxi + dpid4;
    auto zsinxi = std::sin(zxiarg);
    auto zcosxi = std::cos(zxiarg);

    //  Determine number of terms to use.
    auto nterm = nterms[std::min(5, (int(std::abs(z)) - 10) / 5)];
    //  Initialize for modified Horner's rule evaluation of sums
    //  it is assumed that at least three terms are used
    zwk = std::pow(zone / zwk, 3);
    zwk = d9d4 * zwk;
    auto dx = -std::real(zwk);
    auto dy = -std::imag(zwk);
    auto dr = dx + dx;
    auto ds = dx * dx + dy * dy;
    auto ndx = ntermx - nterm + 1;
    auto dals = dcks[ndx];
    auto dalc = dckc[ndx];
    auto dalprs = ddks[ndx];
    auto dalprc = ddkc[ndx];
    ++ndx;
    auto dbes = dcks[ndx];
    auto dbec = dckc[ndx];
    auto dbeprs = ddks[ndx];
    auto dbeprc = ddkc[ndx];
    ++ndx;

    //  Loop until components contributing to sums are computed
    for (int k = ndx; k <= ntermx; ++k) // TODO Loop term!!
    {
      auto dsdata = ds * dals;
      dals = dbes + dr*dals;
      dbes = dcks[k] - dsdata;
      dsdata = ds * dalc;
      dalc = dbec + dr * dalc;
      dbec = dckc[k] - dsdata;
      dsdata = ds * dalprs;
      dalprs = dbeprs + dr * dalprs;
      dbeprs = ddks[k] - dsdata;
      dsdata = ds * dalprc;
      dalprc = dbeprc + dr * dalprc;
      dbeprc = ddkc[k] - dsdata;
    }

    //  Complete evaluation of the Airy functions
    zxi = zone / zxi;
    zai =       zsinxi * dals * dcmplx(dx, dy) + dbes
        - zxi * zcosxi * dalc * dcmplx(dx, dy) + dbec;
    zai = dpimh * zai / zpw1d4;
    zaipr =       zcosxi * dalprc * dcmplx(dx, dy) + dbeprc
          + zxi * zsinxi * dalprs * dcmplx(dx, dy) + dbeprs;
    zaipr = -dpimh * zaipr * zpw1d4;

    return;
  }
  
#endif // AIRY_TCC
