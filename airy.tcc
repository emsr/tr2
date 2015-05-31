


/**

    Purpose
      This subroutine computes the Airy function ai(z) and its first
      derivative in the complex z-plane.

      The algorithm used exploits numerous representations of the
      airy function and its derivative.  We record the representations
      here for reference

		    sqrt(z)
      (1) Ai(z)   = ------- (I (xi) - I (xi))
		       3      -1/3     1/3

		    sqrt(z/3)
      (2) Ai(z)   = --------- K (xi)
		       pi      1/3

		      2/3  -5/6
		     2    3
		  =  --------  z exp(-xi) U(5/6; 5/3; 2 xi)
		     sqrt(pi)

		    sqrt(z)
      (3) Ai(-z)  = ------- (J  (xi) + J (xi))
		       3      -1/3	1/3

		    z
      (4) Ai'(z)  = - (I (xi) - I  (xi))
		    3	2/3	 -2/3

			   z
      (5) Ai'(z)  = - ---------- K (xi)
		      pi*sqrt(3)  2/3

			2/3  -7/6
		       4    3	 2
		  =  - -------- Z  exp(-xi) U(7/6; 7/3; 2 xi)
		       sqrt(pi)

		     z
      (6) Ai'(-z) =  - (J (xi) - J  (xi)) ,
		     3   2/3	  -2/3

		 2  3/2
      Where xi = - z	and U( ; ; ) is the confluent hypergeometric
		 3
      Function as defined in

      Stegun, I.A. and Abramowitz, M., Handbook of Mathematical Functions,
	Natl. Bureau of Standards, AMS 55, pp 504-515, 1964.

      The asymptotic expansions derivable from these representations and
      Hankel's asymptotic expansions for the Bessel functions are used for
      large modulus of z.  The implementation has taken advantage of the
      error bounds given in

      Olver, F.W.J., Error Bounds for Asymptotic Expansions, with an
	Application to Cylinder Functions of Large Argument, in
	Asymptotic Solutions of Differential Equations (Wilcox, Ed.),
	Wiley and Sons, pp 163-183, 1964

      and

      Olver, F.W.J., Asymptotics and Special Functions, Academic Press,
	pp 266-268, 1974.

      For small modulus of z, a rational approximation is used.  This
      approximant is derived from

      Luke, Y.L., Mathematical Functions and their Approximations,
	Academic Press, pp 361-363, 1975.

      The identities given below are for Bessel functions of the first
      kind in terms of modified Bessel functions of the first kind are
      also used with the rational approximant.

      For moderate modulus of z, three techniques are used.  Two use
      a backward recursion algorithm with (1), (3), (4), and (6). The
      third uses the confluent hypergeometric representations given by
      (2)and (5).  The backward recursion algorithm generates values of
      the modified Bessel functions of the first kind of orders + or -
      1/3 and + or - 2/3 for z in the right half plane.  Values for
      the corresponding Bessel functions of the first kind are recovered
      via the identities

	    J (z) = exp(nu pi i/2) I (z exp(-pi i/2)) ,
	     nu 		    nu
		  0 <= arg(z) <= pi/2
      AND
	    J (z) = exp(-nu pi i/2) I (z exp(pi i/2)) ,
	     nu 		     nu
		 -pi/2 <= arg(z) < 0 .

      The particular backward recursion algorithm used is discussed in 
    
      Olver, F.W.J, Numerical solution of second-order linear
	difference equations, NBS J. RES., Series B, VOL 71B,
	pp 111-129, 1967.

      Olver, F.W.J. and Sookne, D.J., Note on backward recurrence
	algorithms, Math. Comp. Vol 26, No. 120, pp 941-947,
	OCT. 1972

      Sookne, D.J., Bessel Functions I and J of Complex Argument and
	Integer Order, NBS J. Res., Series B, Vol 77B, Nos 3 & 4,
	pp 111-113, July-December, 1973. 

      The following paper was also useful

      Cody, W.J., Preliminary report on software for the modified
	Bessel functions of the first kind, Applied Mathematics
	Division, Argonne National Laboratory, Tech. Memo. no. 357.

      A backward recursion algorithm is also used to compute the
      confluent hypergeometric function.  The recursion relations
      and a convergence theorem are given in

      Wimp, J., On the computation of Tricomi's psi function, Computing,
	Vol 13, pp 195-203, 1974.

      See individual subprogram comments for additional details.

 
  @param[in]  z       The argument at which the Airy function and its derivative are to be computed.
  @param[in]  deps    Relative error required.  At present, deps is used only in the 
                      backward recursion algorithms.
  @param[out]  zai    The value computed for Ai(z).
  @param[out]  zaipr  The value computed for Ai'(z).

    Date of last revision
      May 5, 1986

    Author
      Rick A. Whitaker

 */
void
zairy(z, deps, zai, zaipr, ier)
{
      double complex z, zai, zaipr
      double precision     deps
      integer    ier

      double complex zxi, z2xi, zp1d4c, zi1d3, zim1d3, zi2d3,
                 zim2d3, zepd6, zempd6, zepd3, zempd3, z1d3f,
                 zm1d3f, z2d3f, zm2d3f, zpwh
      double precision     dabsz, dzbig, dzxir, dzxii, dzero, dziacc,
                 drsqpi, dzsmal, d1d3, d2d3,
                 dgm2d3, dgm1d3, d2g2d3, dsqrt3

      data zepd6 /( 8.660254037844386d-01, 5.0d-01)/,
           zempd6/( 8.660254037844386d-01,-5.0d-01)/,
           zepd3 /( 5.0d-01, 8.660254037844386d-01)/,
           zempd3/( 5.0d-01,-8.660254037844386d-01)/
      data dzero /0.0d+00/,
           d1d3  /3.333333333333333d-01/,
           d2d3  /6.666666666666667d-01/,
           dsqrt3/1.732050807568877d+00/,
           dgm1d3/2.588194037928068d-01/,
           dgm2d3/3.550280538878172d-01/,
           d2g2d3/1.775140269439086d-01/,
           drsqpi/2.820947917738781d-01/
      data dzsmal/2.5d-01/,dziacc/2.0d+00/,dzbig/15.0d+00/

      ier = 0


      //  compute modulus of z for later use
      dabsz = std::abs(z)
      //  check size of abs(z) and select appropriate methods
      if (dabsz < dzbig)
      then
        //  moderate or small abs(z)
        //  check for right or left half plane argument
        if (std::real(z) >= dzero)
        then
          //  argument in closed right half plane
          //  compute xi as defined in the representations in terms of
          //  bessel functions
          zpwh = zsqrt(z)
          zxi = z*zpwh
          dzxir = d2d3*std::real(zxi)
          dzxii = d2d3*std::imag(zxi)
          zxi = dcmplx(dzxir,dzxii)
          //  check for abs(z) too large for accuracy of representations (1) and (4)
          if (dabsz >= dziacc)
          then
            //  use rational approximation for modified bessel functions
            //  of orders 1/3 and 2/3
            call zkairy(zxi,deps,zai,zaipr,ier)
            //  recover ai(z) and ai'(z)
            zp1d4c = zsqrt(zpwh)
            zxi = std::exp(-zxi)
            zxi = dcmplx(drsqpi*std::real(zxi),drsqpi*std::imag(zxi))
            zai = zxi*(zai/zp1d4c)
            zaipr = -zxi*zp1d4c*zaipr

          else
            //  check for abs(z) small enough for rational approximation
            if (dabsz <= dzsmal)
            then
              //  use rational approximation along with (1) and (4)
              call zcrary(z,zi1d3,zim1d3,zi2d3,zim2d3)
              //  recover ai(z) and ai'(z)
              zim1d3 = dcmplx(std::real(zim1d3)*dgm2d3,
                              std::imag(zim1d3)*dgm2d3)
              zi1d3 = dcmplx(std::real(zi1d3)*dgm1d3,
                             std::imag(zi1d3)*dgm1d3)
              zai = zim1d3 - z*zi1d3
              zim2d3 = dcmplx(std::real(zim2d3)*dgm1d3,
                              std::imag(zim2d3)*dgm1d3)
              zi2d3 = dcmplx(std::real(zi2d3)*d2g2d3,
                             std::imag(zi2d3)*d2g2d3)
              zaipr = z*z*zi2d3 - zim2d3

            else
              //  use backward recurrence along with (1) and (4)
              call ziairy(zxi,deps,zi1d3,zim1d3,zi2d3,zim2d3)
              //  recover ai(z) and ai'(z)
              zai = dcmplx(d1d3*std::real(zpwh),d1d3*std::imag(zpwh))*
                    (zim1d3 - zi1d3)
              zaipr = dcmplx(d1d3*std::real(z),d1d3*std::imag(z))*
                      (zi2d3 - zim2d3)
            end if

          end if

        else
          //  z lies in left half plane
          //  compute xi as defined in the representations in terms of bessel functions
          zpwh = zsqrt(-z)
          zxi = -z*zpwh
          dzxir = d2d3*std::real(zxi)
          dzxii = d2d3*std::imag(zxi)
          zxi = dcmplx(dzxir,dzxii)
          //  set up arguments to recover bessel functions of the first kind in (3) and (6)
          if (dzxii >= dzero)
          then
            //  argument lies in upper half plane, so use appropriate identity
            z2xi = dcmplx(dzxii,-dzxir)
            z1d3f = zepd6
            zm1d3f = zempd6
            z2d3f = zepd3
            zm2d3f = zempd3

          else
            //  argument lies in lower half plane, so use appropriate identity
            z2xi = dcmplx(-dzxii,dzxir)
            z1d3f = zempd6
            zm1d3f = zepd6
            z2d3f = zempd3
            zm2d3f = zepd3
          end if

          //  use approximation depending on size of z
          if (dabsz <= dzsmal)
          then
            //  use rational approximation
            zxi = -z
            call zcrary(z,zi1d3,zim1d3,zi2d3,zim2d3)
            //  recover ai(z) and ai'(z)
              zim1d3 = dcmplx(std::real(zim1d3)*dgm2d3,
                              std::imag(zim1d3)*dgm2d3)
              zi1d3 = dcmplx(std::real(zi1d3)*dgm1d3,
                             std::imag(zi1d3)*dgm1d3)
              zai = zim1d3 - z*zi1d3
                //  zai = zm1d3f*zim1d3 + z*z1d3f*zi1d3
              zim2d3 = dcmplx(std::real(zim2d3)*dgm1d3,
                              std::imag(zim2d3)*dgm1d3)
              zi2d3 = dcmplx(std::real(zi2d3)*d2g2d3,
                             std::imag(zi2d3)*d2g2d3)
              zaipr = z*z*zi2d3 - zim2d3
                //  zaipr = z*z*z2d3f*zi2d3 - zm2d3f*zim2d3

          else
            //  use backward recurrence
            call ziairy(z2xi,deps,zi1d3,zim1d3,zi2d3,zim2d3)
            //  recover ai(z) and ai'(z)
            zai = dcmplx(d1d3*std::real(zpwh),d1d3*std::imag(zpwh))*
                  (zm1d3f*zim1d3 + z1d3f*zi1d3)
            zaipr = dcmplx(d1d3*std::real(z),d1d3*std::imag(z))*
                    (zm2d3f*zim2d3 - z2d3f*zi2d3)
          end if

        end if

      else
        //  abs(z) is large; check arg(z) to see which asymptotic form is appropriate
        if (std::real(z) >= dzero .or.
            std::abs(std::imag(z)) >= -dsqrt3*std::real(z))
        then
          //  abs(arg(z)) <= 2*dpi/3  -  use asymptotic expansion for this region
          zasary(z,zai,zaipr);

        else
          //  abs(arg(-z)) < dpi/3  -  use asymptotic expansion for this region
          zasaly(z,zai,zaipr);
        end if

      end if
      return;
}


/**

    Purpose
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

	I-sub-(nu-1) = (2*nu/z)*I-sub-nu + I-sub-(nu+1)

      SATISFIED BY THE MODIFIED BESSEL FUNCTIONS OF THE FIRST KIND.
      THE NORMALIZATION RELATIONSHIP USED IS
	     NU Z 
	(Z/2)  E		 INF  (K+NU)*GAMMA(2 NU+K)
	-----------  = I (Z) + 2 SUM  -------------------- I (Z) .
	GAMMA(NU+1)	NU	 K=1   K! GAMMA(1 + 2 NU)   NU+K

      THIS MODIFICATION OF THE ALGORITHM IS GIVEN IN PART IN

      OLVER, F.W.J. AND SOOKNE, D.J., NOTE ON BACKWARD RECURRENCE
	ALGORITHMS, MATH. OF COMP., VOL. 26, NO. 120, OCT. 1972.

      AND FURTHER ELABORATED FOR THE BESSEL FUNCTIONS IN
  
      SOOKNE, D.J., BESSEL FUNCTIONS I AN J OF COMPLEX ARGUMENT 
	AND INTEGER ORDER, J. RES. NBS - SERIES B, VOL 77B, NOS.
	3 & 4, JULY-DECEMBER, 1973.

      INSIGHT WAS ALSO GAINED FROM

      CODY, W.J., PRELIMINARY REPORT ON SOFTWARE FOR THE MODIFIED
	BESSEL FUNCTIONS OF THE FIRST KIND, ARGONNE NATIONAL
	LABORATORY, APPLIED MATHEMATICS DIVISION, TECH. MEMO. 
	NO. 357, AUGUST, 1980.

      CODY IMPLEMENTS THE ALGORITHM OF SOOKNE FOR FRACTIONAL ORDER
      AND NONNEGATIVE REAL ARGUMENT.  LIKE CODY, WE DO NOT CHANGE
      THE CONVERGENCE TESTING MECHANISM IN ANY SUBSTANTIAL WAY.
      HOWEVER, WE DO TRIM THE OVERHEAD BY MAKING THE ADDITIONAL
      ASSUMPTION THAT PERFORMING THE CONVERGENCE TEST FOR THE
      FUNCTIONS OF ORDER 2/3 WILL SUFFICE FOR ORDER 1/3 AS WELL.
      THIS ASSUMPTION HAS NOT BEEN ESTABLISHED BY RIGOUROUS
      ANALYSIS AT THIS TIME.  TWO ADDITIONAL CHANGES HAVE BEEN
      MADE FOR THE SAKE OF SPEED.  FIRST, THE STRONG CONVERGENCE
      CRITERIA IS COMPUTED IN SINGLE PRECISION SINCE MAGNITUDE IS
      THE MOST IMPORTANT THING HERE.  SECOND, THE TESTS ARE
      PERFORMED IN THE 1-NORM INSTEAD OF THE USUAL EUCLIDEAN
      NORM USED IN THE COMPLEX PLANE.  TO INSURE THE VALIDITY
      OF THE RESULTS, THE INEQUALITY
					   2	2 
	ABS(X) + ABS(Y) <= SQRT(2) SQRT(X  + Y ) 

      WAS USED TO MODIFY THE CONVERGENCE TESTS.

      NOTE ALSO THAT FOR THE SAKE OF SPEED AND THE FACT THAT THIS
      SUBROUTINE WILL BE DRIVEN BY ANOTHER, CHECKS THAT ARE NOT
      ABSOLUTELY NECESSARY ARE NOT MADE.  UNDER THESE ASSUMPTIONS
      AN ERROR RETURN IS NOT NEEDED.

      ARGUMENTS
	Z      DOUBLE PRECISION COMPLEX INPUT VARIABLE SET EQUAL
	       TO THE ARGUMENT AT WHICH THE MODIFIED BESSEL
	       FUNCTIONS COMPUTED BY THIS PROGRAM ARE TO BE
	       EVALUATED.
	DEPS   DOUBLE PRECISION INPUT VARIABLE SET EQUAL TO THE
	       MAXIMUM RELATIVE ERROR REQUIRED IN THE RESULTS.
	ZI1D3  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING
	       THE VALUE OF I-SUB-1/3 (Z) COMPUTED.
	ZIM1D3 DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING
	       THE VALUE OF I-SUB-(-1/3) COMPUTED.
	ZI2D3  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING
	       THE VALUE OF I-SUB-2/3 (Z) COMPUTED.
	ZIM2D3 DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING
	       THE VALUE OF I-SUB-(-2/3) (Z) COMPUTED.

      DATE OF LAST REVISION
	APRIL 28, 1986

      AUTHOR
	RICK A. WHITAKER

 */
void
ziairy(z,deps,zi1d3,zim1d3,zi2d3,zim2d3)
{
      double complex z, zi1d3, zim1d3, zi2d3, zim2d3;
      double deps;

      double complex z1dz, zplst2, zp2, zpold2, zplst1, zp1, zpold1, 
                 zsum1, zsum2, zd2pow, zero, zone;
      double dzr, dzi, dzrabs, dziabs, du, dv, d1dzi,
                 d1dzr, dmodz, dtest, dn, d2n, dzp2r, d2d3,
                 dzp2i, dnpn1, dnpn2, dnp2n1, dnp2n2, dfac1, dfac2,
                 done, dtwo, d2sqr2, dhalf, dgm4d3, dgm5d3, d1d3,
                 d4d3, d5d3, d8d3, d10d3, d14d3, d16d3;
      double spr, spi, splstr, splsti, slamn, skn, sone, stwo;
      bool lstcnv;

      int n, l, nend

      data sone/1.0/, stwo/2.0/
      data zero/(0.0d+00,0.0d+00)/, zone/(1.0d+00,0.0d+00)/
      data done/1.0d+00/, dtwo/2.0d+00/, dhalf/5.0d-01/,
           d1d3  /3.333333333333333d-01/, d2d3  /6.666666666666667d-01/,
           d4d3  /1.333333333333333d+00/, d5d3  /1.666666666666667d+00/,
           d8d3  /2.666666666666667d+00/, d10d3 /3.333333333333333d+00/,
           d14d3 /4.666666666666667d+00/, d16d3 /5.333333333333333d+00/,
           dgm4d3/8.929795115692492d-01/, dgm5d3/9.027452929509336d-01/,
           d2sqr2/2.828427124746190d+01/

      //  compute 1/z for use in recurrence for speed and abs(z)
      dzr = std::real(z);
      dzi = std::imag(z);
      dzrabs = std::abs(dzr);
      dziabs = std::abs(dzi);
      du = std::max(dzrabs, dziabs);
      dv = std::min(dzrabs, dziabs);
      d1dzi = (done + (dv/du)**2);
      dmodz = du * std::sqrt(d1dzi);
      d1dzi = du * d1dzi;
      d1dzr = (dzr / du) / d1dzi;
      d1dzi = -(dzi / du) / d1dzi;
      z1dz = dcmplx(d1dzr, d1dzi);

      //  initialize for forward recursion based on order 2/3
      n = 0;
      d2n = double(n + n) + d4d3;
      zplst2 = zone;
      zp2 = d2n * dcmplx(d1dzr, d1dzi);

      //  calculate weak convergence test and set flag for weak convergence loop
      dtest = d2sqr2 / deps;
      lstcnv = false;


      //  loop until weak and strong convergence tests satisfied when recurring forward
   10 continue

        //  loop until current convergence test satisfied
   20   continue
          //  update n dependent quantities
          ++n;
          d2n += dtwo;
          //  interchange values
          zpold2 = zplst2;
          zplst2 = zp2;
          //  recur forward one step
          zp2 = z1dz * d2n * zplst2 + zpold2;

          //  check if convergence test (in 1-norm) satisfied

          if (std::abs(std::real(zp2)) + std::abs(std::imag(zp2)) >= dtest)
            go to 30

        go to 20

   30   continue

        //  if strong convergence, then weak and strong convergence
        if (lstcnv) go to 40

        //  calculate strong convergence test in single precision.  see
        //  the olver and sookne papers cited for details.
        spr = std::real(zp2);
        spi = std::imag(zp2);
        splstr = std::real(zplst2);
        splsti = std::imag(zplst2);
        //  compute scale factor to avoid possible overflow
        slamn = std::max(std::abs(spr), std::abs(spi));
        //  compute the k-sub-n of strong convergence lemma
        skn = std::sqrt(((spr/slamn)**2 + (spi/slamn)**2)/
                        ((splstr/slamn)**2 + (splsti/slamn)**2));
        //  compute quantity needed for lambda-sub-n of strong convergence lemma
        slamn = double(n + 1) / dmodz;
        //  determine appropriate value for rho-sub-n of lemma
        if (skn + sone / skn > stwo * slamn)
          skn = slamn + std::sqrt(slamn * slamn - sone);
        //  compute test value - sqrt(2) multiple already included
        dtest *= std::sqrt(skn - sone / skn);
        //  set strong convergence test flag
        lstcnv = true;
        go to 10
   40 continue
      //  repeat

      //  prepare for backward recurrence for both orders 1/3 and 2/3
      dn = double(n);
      ++n;
      d2n = dfloat(n+n);
      zplst1 = zero;
      zplst2 = zero;
      //  carefully compute 1/zp2 to avoid overflow in complex divide
      dzp2r = std::real(zp2);
      dzp2i = std::imag(zp2);
      dzrabs = std::abs(dzp2r);
      dziabs = std::abs(dzp2i);
      du = std::max(dzrabs, dziabs);
      dv = std::min(dzrabs, dziabs);
      d1dzi = du * (done + (dv/du)**2);
      d1dzr = (dzp2r / du) / d1dzi;
      d1dzi = -(dzp2i / du) / d1dzi;
      zp1 = dcmplx(d1dzr, d1dzi);
      zp2 = zp1;
      //  set up n dependent parameters used in normalization sum
      dnpn1 = dn + d1d3;
      dnpn2 = dn + d2d3;
      dnp2n1 = (dn - done) + d2d3;
      dnp2n2 = (dn - done) + d4d3;
      //  initialize normalization sum
      dfac1 = dnpn1 * dnp2n1 / dn;
      zsum1 = dfac1 * zp1;
      dfac2 = dnpn2 * dnp2n2 / dn;
      zsum2 = dfac2 * zp2;
      //  set ending loop index to correspond to k=1 term of the
      //  normalization relationship
      nend = n - 3

      //  if backward recurrence loop will be nontrivial
      if (nend > 0)
      {
        //  loop until backward recursion to k=1 term of normalization
        do 50 l = 1, nend
          //  update n dependent quantities
          --n;
          d2n = d2n - dtwo;
          dfac1 = d2n + d2d3;
          dfac2 = d2n + d4d3;
          //  interchanges for order 1/3 recurrence
          zpold1 = zplst1;
          zplst1 = zp1;
          //  recur back one step for order 1/3
          zp1 = dcmplx(dfac1*std::real(zplst1),dfac1*std::imag(zplst1))*
                z1dz + zpold1
          //  interchanges for order 2/3 recurrence
          zpold2 = zplst2;
          zplst2 = zp2;
          //  recur back one step for order 2/3
          zp2 = dfac2 * zplst2* z1dz + zpold2;
          //  update quantities for computing normalization sums
          dn = dn - done;
          dnpn1 = dn + d1d3;
          dnp2n1 = dn - d1d3;
          dnpn2 = dn + d2d3;
          dnp2n2 = dnpn1;
          dfac1 = dnp2n1 / dn;
          dfac2 = dnp2n2 / dn;
          //  update normalization sums
          zsum1 += dnpn1 * zp1;
          zsum1 *= dfac1;
          zsum2 += dnpn2 * zp2;
          zsum2 *= dfac2;
   50   continue
      }

      //  perform last two recurrence steps for order 1/3
      zpold1 = zplst1;
      zplst1 = zp1;
      zp1 = z1dz * d14d3 * zplst1 + zpold1;
      zsum1 += d4d3 * zp1;
      zpold1 = zplst1;
      zplst1 = zp1;
      zp1 = d8d3 * zplst1 * z1dz + zpold1;
      zsum1 = (zsum1 + zsum1) + zp1;

      //  compute scale factor and scale results for order 1/3 case
      zd2pow = dcmplx(dhalf*dzr, dhalf*dzi)**(-d1d3);
      zpold1 = zd2pow * std::exp(-z);
      zsum1 *= zpold1 * dgm4d3;
      zplst1 /= zsum1;
      zi1d3 = zp1 / zsum1;

      //  perform last two recurrence steps for order 2/3
      zpold2 = zplst2
      zplst2 = zp2
      zp2 = d16d3 * zplst2 * z1dz + zpold2
      zsum2 += d5d3 * zp2;
      zpold2 = zplst2;
      zplst2 = zp2;
      zp2 = d10d3 * zplst2 * z1dz + zpold2
      zsum2 = (zsum2 + zsum2) + zp2

      //  compute scale factor and scale results for order 2/3 case
      zsum2 *= zd2pow * zpold1 * dgm5d3;
      zplst2 /= zsum2;
      zi2d3 = zp2 / zsum2;

      //  recur back one step from order 1/3 to get order -2/3
      zim2d3 = dcmplx(d2d3*std::real(zi1d3),d2d3*std::imag(zi1d3))*z1dz +
               zplst1

      //  recur back one step from order 2/3 to get order -1/3
      zim1d3 = dcmplx(d4d3*std::real(zi2d3),d4d3*std::imag(zi2d3))*z1dz +
               zplst2

      return
}



/**

    PURPOSE
      COMPUTE APPROXIMATIONS TO THE MODIFIED BESSEL FUNCTIONS OF THE
      SECOND KIND OF ORDERS 1/3 AND 2/3 FOR MODERATE ARGUMENTS.  MORE
      SPECIFICALLY, THE PROGRAM COMPUTES

	E (Z) = EXP(Z) SQRT(2 Z/PI) K (Z), FOR NU = 1/3 AND NU = 2/3.
	 NU			     NU

      THIS SUBPROGRAM USES A RATIONAL APPROXIMATION GIVEN IN

      LUKE, Y.L., MATHEMATICAL FUNCTIONS AND THEIR APPROXIMATIONS,
	ACADEMIC PRESS, PP 366-367, 1975.

      THOUGH THE APPROXIMATION CONVERGES IN ABS(ARG(Z)) <= PI,
      THE CONVERGENCE WEAKENS AS ABS(ARG(Z)) INCREASES.  ALSO, IN
      THE CASE OF REAL ORDER BETWEEN 0 AND 1, CONVERGENCE WEAKENS
      AS THE ORDER APPROACHES 1.  FOR THESE REASONS, WE ONLY USE
      THIS CODE FOR ABS(ARG(Z)) <= 3*PI/4 AND THE CONVERGENCE TEST
      IS PERFORMED ONLY FOR ORDER 2/3.

      THE CODING OF THIS SUBPROGRAM IS ALSO INFLUENCED BY THE FACT
      THAT IT WILL ONLY BE USED FOR ABOUT 2 <= ABS(Z) <= 15.
      HENCE, CERTAIN CONSIDERATIONS OF OVERFLOW, UNDERFLOW, AND
      LOSS OF SIGNIFICANCE ARE UNIMPORTANT FOR OUR PURPOSE.

      ARGUMENTS
	Z      DOUBLE PRECISION COMPLEX INPUT VARIABLE SET EQUAL 
	       TO THE VALUE FOR WHICH THE QUANTITY E-SUB-NU IS TO
	       BE COMPUTED.  IT IS RECOMMENDED THAT ABS(Z) NOT BE
	       TOO SMALL AND THAT ABS(ARG(Z)) <= 3*PI/4.
	DEPS   DOUBLE PRECISION INPUT VARIABLE SET EQUAL TO THE
	       MAXIMUM RELATIVE ERROR ALLOWABLE IN THE COMPUTED
	       RESULTS.  THE RELATIVE ERROR TEST IS BASED ON THE
	       COMPARISON OF SUCCESSIVE ITERATES.
	ZK1D3  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAIN-
	       ING THE VALUE COMPUTED FOR E-SUB-(1/3) OF Z.
	ZK2D3  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAIN-
	       ING THE VALUE COMPUTED FOR E-SUB-(2/3) OF Z.
	IER    INTEGER OUTPUT VARIABLE SET EQUAL TO A COMPLETION
	       CODE.  IER = 0 INDICATES NORMAL COMPLETION.  IF
	       IER = 129, THEN CONVERGENCE FAILED IN 100 
	       ITERATIONS.

	       REMARK:  ACCORDING TO PUBLISHED INFORMATION ABOUT
			THE BEHAVIOUR OF THE ERROR FOR ORDERS
			1/3 AND 2/3, IER = 129 SHOULD NEVER OCCUR
			FOR THE DOMAIN OF Z THAT WE RECOMMEND.
			INDEED, IN THE WORST CASE, SAY, Z=2 AND
			ARG(Z) = 3*PI/4, WE EXPECT 20 ITERATIONS
			TO GIVE 7 OR 8 DECIMALS OF ACCURACY.

    DATE OF LAST REVISION
      MAY 7, 1986

    AUTHOR
      RICK A. WHITAKER

 */
void
zkairy(z,deps,zk1d3,zk2d3,ier)
      double complex z, zk1d3, zk2d3
      double precision     deps
      integer    ier

      double complex zone, zf10, zf11, zf12, zf13, zphi10,
                 zphi11, zphi12, zphi13, zf20, zf21, zf22,
                 zf23, zphi20, zphi21, zphi22, zphi23, zfact1,
                 zfact2, zratnw, zratol
      double precision     dtwo, deight, d16, d24, dthree, dfive, 
                 delti, dan1i, dan2i, dp12i, dp22i, dfco(8),
                 dp13i, dp23i, dp11i, dp21i, dan1, dphico(6),
                 dan2, dp11, dp12, dp13, dp21, dp22,
                 dp23, delt, deta, dq, dgamm, dgam,
                 danm1, danm2, dqzr, dqzi
      integer    i

      double complex zrat1

      double complex dcmplx

      data dtwo/2.0d+00/,deight/8.0d+00/,d16/16.0d+00/d24/24.0d+00/,
           dthree/3.0d+00/,dfive/5.0d+00/delti/32.0d+00/,
           dan1i/4.855555555555555d+01/dan2i/4.722222222222222d+01/,
           dp12i/3.144444444444444d+01/,dp22i/3.277777777777777d+01/,
           dp13i/-9.259259259259259d-01/,dp23i/1.296296296296296d+00/,
           dp11i/-7.907407407407407d+01/,dp21i/-8.129629629629629d+01/
      data zone/(1.0d+00,0.0d+00)/

      data dfco/144.0d+00,77.0d+00,62208.0d+00,95472.0d+00,
                17017.0d+00,65.0d+00,90288.0d+00,13585.0d+00/
      data dphico/67.0d+00,91152.0d+00,12697.0d+00,79.0d+00,
                  96336.0d+00,19633.0d+00/

      ier = 0

      //  initialize polynomials for recurrence
      zf10 = zone
      zf20 = zone
      zphi10 = zone
      zphi20 = zone
      zf11 = dcmplx((dfco(1)*std::real(z)+dfco(2))/dfco(2),
                     dfco(1)*std::imag(z)/dfco(2))
      zf12 = dcmplx(dfco(3)*std::real(z)+dfco(4),dfco(3)*std::imag(z))*z
      zf12 = dcmplx((std::real(zf12)+dfco(5))/dfco(5),
                     std::imag(zf12)/dfco(5))
      zf21 = dcmplx((dfco(1)*std::real(z)+dfco(6))/dfco(6),
                     dfco(1)*std::imag(z)/dfco(6))
      zf22 = dcmplx(dfco(3)*std::real(z)+dfco(7),dfco(3)*std::imag(z))*z
      zf22 = dcmplx((std::real(zf22)+dfco(8))/dfco(8),
                     std::imag(zf22)/dfco(8))
      zphi11 = dcmplx((dfco(1)*std::real(z)+dphico(1))/dfco(2),
                       std::imag(zf11))
      zphi12 = dcmplx(dfco(3)*std::real(z)+dphico(2),
                      dfco(3)*std::imag(z))*z
      zphi12 = dcmplx((std::real(zphi12)+dphico(3))/dfco(5),
                       std::imag(zphi12)/dfco(5))
      zphi21 = dcmplx((dfco(1)*std::real(z)+dphico(4))/dfco(6),
                       std::imag(zf21))
      zphi22 = dcmplx(dfco(3)*std::real(z)+dphico(5),
                       dfco(3)*std::imag(z))*z
      zphi22 = dcmplx((std::real(zphi22)+dphico(6))/dfco(8),
                       std::imag(zphi22)/dfco(8))

      //  initialize for recursion
      zratol = zphi22/zf22
      zrat1 = zphi12/zf12
      delt = delti
      dan1 = dan1i
      dan2 = dan2i
      dp11 = dp11i
      dp12 = dp12i
      dp13 = dp13i
      dp21 = dp21i
      dp22 = dp22i
      dp23 = dp23i
      deta = d24
      dgamm = dthree
      dgam = dfive
      dq = d16*dgam

      //  loop until maximum iterations used or convergence
      do 10 i=1,100
        //  evaluate next term in recurrence for order 1/3 polynomials
        dqzr = dq*std::real(z)
        dqzi = dq*std::imag(z)
        zfact1 = dcmplx(dqzr-dp11,dqzi)
        zfact2 = dcmplx(dqzr-dp12,dqzi)
        zf13 = zfact1*zf12 + zfact2*zf11 - dcmplx(dp13*std::real(zf10),
                                                  dp13*std::imag(zf10))
        zf13 = dcmplx(std::real(zf13)/dan1,std::imag(zf13)/dan1)
        zphi13 = zfact1*zphi12 + zfact2*zphi11 -
                 dcmplx(dp13*std::real(zphi10),dp13*std::imag(zphi10))
        zphi13 = dcmplx(std::real(zphi13)/dan1,std::imag(zphi13)/dan1)
        //  evaluate next term in recurrence for order 2/3 polynomials
        zfact1 = dcmplx(dqzr-dp21,dqzi)
        zfact2 = dcmplx(dqzr-dp22,dqzi)
        zf23 = zfact1*zf22 + zfact2*zf21 - dcmplx(dp23*std::real(zf20),
                                                  dp23*std::imag(zf20))
        zf23 = dcmplx(std::real(zf23)/dan2,std::imag(zf23)/dan2)
        zphi23 = zfact1*zphi22 + zfact2*zphi21 -
                 dcmplx(dp23*std::real(zphi20),dp23*std::imag(zphi20))
        zphi23 = dcmplx(std::real(zphi23)/dan2,std::imag(zphi23)/dan2)

        //  check for convergence
        zratnw = zphi23/zf23
        zrat1 = zphi13/zf13


        if (std::abs(zratnw-zratol)
            < deps*std::abs(zratnw)) go to 20

        //  prepare for next iteration
        zratol = zratnw
        zf20 = zf21
        zf21 = zf22
        zf22 = zf23
        zphi20 = zphi21
        zphi21 = zphi22
        zphi22 = zphi23
        zf10 = zf11
        zf11 = zf12
        zf12 = zf13
        zphi10 = zphi11
        zphi11 = zphi12
        zphi12 = zphi13
        delt = delt + d24
        dp12 = dp12 + delt
        dp22 = dp22 + delt
        deta = deta + deight
        dan1 = dan1 + deta
        dan2 = dan2 + deta
        danm1 = dan1 - delt - d16
        danm2 = dan2 - delt - d16
        dgamm = dgam
        dgam = dgam + dtwo
        dp23 = -dgam/dgamm
        dp13 = dp23*danm1
        dp23 = dp23*danm2
        dp11 = -dan1 - dp12 - dp13
        dp21 = -dan2 - dp22 - dp23
        dq = d16*dgam
   10 continue

      //  maximum iterations exceeded
      ier = 129
      return
      //  convergence
   20 continue
      zk2d3 = zratnw
      zk1d3 = zphi13/zf13

      return
}



/**

    PURPOSE
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
void
zcrary(z, zf1d3, zfm1d3, zf2d3, zfm2d3)
{
      double complex z, zf1d3, zfm1d3, zf2d3, zfm2d3

      double complex zone, zzz
      double precision     dx, dy, dr, ds, dt, dtwo,
                 dal, dbe, da1d3(4), dam1d3(4),
                 da2d3(4), dam2d3(4), db1d3(4),
                 dbm1d3(4), db2d3(4), dbm2d3(4),
                 dsmall

      data dtwo/2.0d+00/,zone/(1.0d+00,1.0d+00)/

      //  dsmall is the 1/3 root of the smallest magnitude
      //  floating-point number representable on the machine
      data dsmall/1.0d-12/

      data da1d3 /81d+00,32400d+00,2585520d+00,37920960d+00/,
           db1d3 /-35d+00,5040d+00,-574560d+00,37920960d+00/,
           dam1d3/81d+00,22680d+00,1156680d+00,7711200d+00/,
           dbm1d3/-10d+00,1260d+00,-128520d+00,7711200d+00/,
           da2d3 /162d+00,75735d+00,7270560d+00,139352400d+00/,
           db2d3 /-110d+00,16830d+00,-2019600d+00,139352400d+00/,
           dam2d3/162d+00,36855d+00,1415232d+00,4481568d+00/,
           dbm2d3/-7d+00,819d+00,-78624d+00,4481568d+00/

      //  check to see if z**3 will underflow and act accordingly

      if (std::abs(z) < dsmall)
      then
        //  all ratios are 1 
        zf1d3  = zone
        zfm1d3 = zone
        zf2d3  = zone
        zfm2d3 = zone
      else
        //  initialize argument dependent quantities used throughout
        zzz = z**3
        dx = std::real(zzz)
        dy = std::imag(zzz)
        dr = dtwo*dx
        ds = dx*dx + dy*dy

         //  all of the following polynomial evaluations are done using
         //  a modified of horner's rule which exploits the fact that
         //  the polynomial coefficients are all real.  the algorithm is
         //  discussed in detail in

         //  knuth, d. e., the art of computer programming: seminumerical
         //  algorithms (vol. 2), addison-wesley, pp 423-424, 1969.

         //  if n is the degree of the polynomial, n-3 multiplies are
         //  saved and 4*n-6 additions are saved.

        //  evaluate numerator polynomial for nu=1/3 approximant
        dal = da1d3(1)
        dt  = ds * dal
        dal = da1d3(2) + dr * dal
        dbe = da1d3(3) - dt
        dt  = ds * dal
        dal = dbe + dr * dal
        dbe = da1d3(4) - dt
        zf1d3 = dcmplx(dal * dx + dbe, dal * dy)
        //  evaluate denominator polynomial for nu=1/3 approximant and
        //  compute ratio of numerator and denominator
        dal = db1d3(1)
        dt  = ds * dal
        dal = db1d3(2) + dr * dal
        dbe = db1d3(3) - dt
        dt  = ds * dal
        dal = dbe + dr * dal
        dbe = db1d3(4) - dt  
        zf1d3 /= dcmplx(dal * dx + dbe, dal * dy)

        //  evaluate numerator polynomial for nu=-1/3 approximant
        dal = dam1d3(1)
        dt  = ds * dal
        dal = dam1d3(2) + dr * dal
        dbe = dam1d3(3) - dt
        dt  = ds * dal
        dal = dbe + dr * dal  
        dbe = dam1d3(4) - dt
        zfm1d3 = dcmplx(dal * dx + dbe, dal * dy)
        //  evaluate denominator polynomial for nu=-1/3 approximant and
        //  compute ratio of numerator and denominator
        dal = dbm1d3(1)
        dt  = ds * dal
        dal = dbm1d3(2) + dr * dal
        dbe = dbm1d3(3) - dt
        dt  = ds * dal
        dal = dbe + dr * dal
        dbe = dbm1d3(4) - dt
        zfm1d3 /= dcmplx(dal * dx + dbe,dal * dy)

        //  evaluate numerator polynomial for nu=2/3 approximant
        dal = da2d3(1)
        dt  = ds * dal
        dal = da2d3(2) + dr * dal
        dbe = da2d3(3) - dt
        dt  = ds * dal
        dal = dbe + dr * dal  
        dbe = da2d3(4) - dt
        zf2d3 = dcmplx(dal * dx + dbe, dal * dy)
        //  evaluate denominator polynomial for nu=2/3 approximant and
        //  compute ratio of numerator and denominator
        dal = db2d3(1)
        dt  = ds * dal
        dal = db2d3(2) + dr * dal
        dbe = db2d3(3) - dt
        dt  = ds * dal
        dal = dbe + dr * dal  
        dbe = db2d3(4) - dt
        zf2d3 /= dcmplx(dal * dx + dbe, dal * dy)

        //  evaluate numerator polynomial for nu=-2/3 approximant
        dal = dam2d3(1)
        dt  = ds * dal
        dal = dam2d3(2) + dr * dal
        dbe = dam2d3(3) - dt  
        dt  = ds * dal
        dal = dbe + dr * dal
        dbe = dam2d3(4) - dt
        zfm2d3 = dcmplx(dal * dx + dbe, dal * dy)
        //  evaluate denominator polynomial for nu=-2/3 approximant and
        //  compute ratio of numerator and denominator
        dal = dbm2d3(1)
        dt  = ds * dal
        dal = dbm2d3(2) + dr * dal
        dbe = dbm2d3(3) - dt
        dt  = ds * dal
        dal = dbe + dr * dal
        dbe = dbm2d3(4) - dt
        zfm2d3 /= dcmplx(dal * dx + dbe, dal * dy)

      end if

      return
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
void
zasary(z,zai,zaipr)
{
      double complex z, zai, zaipr

      double complex zout, zoutpr, zpw1d4, zxim, zmone
      double precision     dpmhd2, d2d3, dx, dy, dr, ds,
                 dal, dbe, dalpr, dbepr, dsdata,
                 dck(15), ddk(15)
      integer    nterm, ntermx, ndx, k, nterms(5)

      integer    int, min0

      data d2d3  /6.666666666666667d-01/,
           dpmhd2/2.820947917738781d-01/,
           zmone /(-1.0d+00,0.0d+00)/
      data ntermx/15/,nterms/15,12,11,11,9/

      //  coefficients for the expansion
      data dck( 1)/ 0.5989251356587907d+05/,
           dck( 2)/ 0.9207206599726415d+04/,
           dck( 3)/ 0.1533169432012796d+04/,
           dck( 4)/ 0.2784650807776026d+03/,
           dck( 5)/ 0.5562278536591708d+02/,
           dck( 6)/ 0.1234157333234524d+02/,
           dck( 7)/ 0.3079453030173167d+01/,
           dck( 8)/ 0.8776669695100169d+00/,
           dck( 9)/ 0.2915913992307505d+00/,
           dck(10)/ 0.1160990640255154d+00/,
           dck(11)/ 0.5764919041266972d-01/,
           dck(12)/ 0.3799305912780064d-01/,
           dck(13)/ 0.3713348765432099d-01/,
           dck(14)/ 0.6944444444444444d-01/,
           dck(15)/ 0.1000000000000000d+01/

      data ddk( 1)/-0.6133570666385206d+05/,
           ddk( 2)/-0.9446354823095932d+04/,
           ddk( 3)/-0.1576357303337100d+04/,
           ddk( 4)/-0.2870332371092211d+03/,
           ddk( 5)/-0.5750830351391427d+02/,
           ddk( 6)/-0.1280729308073563d+02/,
           ddk( 7)/-0.3210493584648621d+01/,
           ddk( 8)/-0.9204799924129446d+00/,
           ddk( 9)/-0.3082537649010791d+00/,
           ddk(10)/-0.1241058960272751d+00/,
           ddk(11)/-0.6266216349203231d-01/,
           ddk(12)/-0.4246283078989483d-01/,
           ddk(13)/-0.4388503086419753d-01/,
           ddk(14)/-0.9722222222222222d-01/,
           ddk(15)/ 0.1000000000000000d+01/

      //  compute -xi and z**(1/4)
      zpw1d4 = zsqrt(z);
      zxim = z * zpw1d4;
      zxim *= d2d3;
      zpw1d4 = zsqrt(zpw1d4);

      //  compute outer factors in the expansions
      zoutpr = std::exp(-zxim);
      zoutpr *= dpmhd2;
      zout = zoutpr / zpw1d4;
      zoutpr *= -zpw1d4;

      //  determine number of terms to use
      nterm = nterms(min0(5, (int(std::abs(z)) - 10) / 5));
      //  initialize for modified horner's rule evaluation of sums
      //  it is assumed that at least three terms are used
      zxim = zmone / zxim;
      dx = std::real(zxim);
      dy = std::imag(zxim);
      dr = dx + dx;
      ds = dx * dx + dy * dy;
      ndx = ntermx - nterm + 1;
      dal = dck(ndx)
      dalpr = ddk(ndx)
      ndx = ndx + 1
      dbe = dck(ndx)
      dbepr = ddk(ndx)
      ndx = ndx + 1

      //  loop until components contributing to sums are computed
      do 10 k=ndx,ntermx
        dsdata = ds * dal
        dal = dbe + dr * dal
        dbe = dck(k) - dsdata
        dsdata = ds * dalpr
        dalpr = dbepr + dr * dalpr
        dbepr = ddk(k) - dsdata
   10 continue

      //  complete evaluation of the airy functions
      zai = zout * dcmplx(dal * dx + dbe, dal * dy);
      zaipr = zoutpr * dcmplx(dalpr * dx + dbepr, dalpr * dy);

      return
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
void
zasaly(z, zai, zaipr)
{
      double complex z, zai, zaipr

      double complex zwk, zsinxi, zcosxi, zpw1d4, zxi, zone
      double precision     dpimh, d2d3, d9d4, dx, dy, dr,
                 ds, dals, dalc, dalprs, dalprc, dbes,
                 dbec, dbeprs, dbeprc, dsdata, dpid4,
                 dcks(9), dckc(9), ddks(9),
                 ddkc(9)
      integer    nterm, ntermx, ndx, k, nterms(5)

      integer    int, min0

      data d2d3  /6.666666666666667d-01/,d9d4/2.25d+00/,
           dpimh/5.641895835477563d-01/,
           dpid4/7.853981633974483d-01/,
           zone /(1.0d+00,0.0d+00)/
      data ntermx/9/,nterms/9,7,6,6,5/

      //  coefficients for the expansion
      data dckc( 1)/ 0.2519891987160237d+08/,
           dckc( 2)/ 0.4195248751165511d+06/,
           dckc( 3)/ 0.9207206599726415d+04/,
           dckc( 4)/ 0.2784650807776026d+03/,
           dckc( 5)/ 0.1234157333234524d+02/,
           dckc( 6)/ 0.8776669695100169d+00/,
           dckc( 7)/ 0.1160990640255154d+00/,
           dckc( 8)/ 0.3799305912780064d-01/,
           dckc( 9)/ 0.6944444444444444d-01/
      data dcks( 1)/ 0.3148257417866826d+07/,
           dcks( 2)/ 0.5989251356587907d+05/,
           dcks( 3)/ 0.1533169432012796d+04/,
           dcks( 4)/ 0.5562278536591708d+02/,
           dcks( 5)/ 0.3079453030173167d+01/,
           dcks( 6)/ 0.2915913992307505d+00/,
           dcks( 7)/ 0.5764919041266972d-01/,
           dcks( 8)/ 0.3713348765432099d-01/,
           dcks( 9)/ 0.1000000000000000d+01/

      data ddks( 1)/-0.2569790838391133d+08/,
           ddks( 2)/-0.4289524004000691d+06/,
           ddks( 3)/-0.9446354823095932d+04/,
           ddks( 4)/-0.2870332371092211d+03/,
           ddks( 5)/-0.1280729308073563d+02/,
           ddks( 6)/-0.9204799924129446d+00/,
           ddks( 7)/-0.1241058960272751d+00/,
           ddks( 8)/-0.4246283078989483d-01/,
           ddks( 9)/-0.9722222222222222d-01/
      data ddkc( 1)/-0.3214536521400865d+07/,
           ddkc( 2)/-0.6133570666385206d+05/,
           ddkc( 3)/-0.1576357303337100d+04/,
           ddkc( 4)/-0.5750830351391427d+02/,
           ddkc( 5)/-0.3210493584648621d+01/,
           ddkc( 6)/-0.3082537649010791d+00/,
           ddkc( 7)/-0.6266216349203231d-01/,
           ddkc( 8)/-0.4388503086419753d-01/,
           ddkc( 9)/ 0.1000000000000000d+01/

      //  set up working value of z
      zwk = -z
      //  compute xi and z**(1/4)
      zpw1d4 = zsqrt(zwk)
      zxi = zwk*zpw1d4
      zxi = dcmplx(d2d3*std::real(zxi),d2d3*std::imag(zxi))
      zpw1d4 = zsqrt(zpw1d4)

      //  compute sine and cosine factors in the expansions
      zcosxi = dcmplx(std::real(zxi)+dpid4,std::imag(zxi)) 
      zsinxi = std::sin(zcosxi)
      zcosxi = std::cos(zcosxi)

      //  determine number of terms to use
      nterm = nterms(min0(5,(int(std::abs(z))-10)/5))
      //  initialize for modified horner's rule evaluation of sums
      //  it is assumed that at least three terms are used
      zwk = (zone/zwk)**3
      zwk = dcmplx(d9d4*std::real(zwk),d9d4*std::imag(zwk))  
      dx = -std::real(zwk)
      dy = -std::imag(zwk)
      dr = dx + dx
      ds = dx*dx + dy*dy
      ndx = ntermx - nterm + 1
      dals = dcks(ndx)
      dalc = dckc(ndx)
      dalprs = ddks(ndx)
      dalprc = ddkc(ndx)
      ndx = ndx + 1
      dbes = dcks(ndx)
      dbec = dckc(ndx)
      dbeprs = ddks(ndx)
      dbeprc = ddkc(ndx)
      ndx = ndx + 1

      //  loop until components contributing to sums are computed
      do 10 k=ndx,ntermx
        dsdata = ds*dals
        dals = dbes + dr*dals
        dbes = dcks(k) - dsdata
        dsdata = ds*dalc
        dalc = dbec + dr*dalc
        dbec = dckc(k) - dsdata
        dsdata = ds*dalprs
        dalprs = dbeprs + dr*dalprs
        dbeprs = ddks(k) - dsdata
        dsdata = ds*dalprc
        dalprc = dbeprc + dr*dalprc
        dbeprc = ddkc(k) - dsdata
   10 continue

      //  complete evaluation of the airy functions
      zxi = zone/zxi
      zai = zsinxi*dcmplx(dals*dx+dbes,dals*dy) -
            zxi*zcosxi*dcmplx(dalc*dx+dbec,dalc*dy)
      zai = dcmplx(dpimh*std::real(zai),dpimh*std::imag(zai))/zpw1d4
      zaipr = zcosxi*dcmplx(dalprc*dx+dbeprc,dalprc*dy) +
              zxi*zsinxi*dcmplx(dalprs*dx+dbeprs,dalprs*dy)
      zaipr = dcmplx(-dpimh*std::real(zaipr),-dpimh*std::imag(zaipr))*zpw1d4

      return
}
