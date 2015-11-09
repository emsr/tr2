      double complex function spec_sqrt(zz)
*
*
*      **      Subroutine to compute the complex square root of a quantity
*      **      and place it on the so-called spectral sheet, defined by 
*      **      imag(sqrt(z)) <= 0.  This sheet choice is "spectral" in the
*      **      sense that outgoing waves decay under exp(jwt) time convention.
*
*
*      **      by L. W. Pearson 7/85
*
*
*      **      LANGUAGE: VAX Fortran  (G_FLOATING)
*
*
      include 'utd.i'

      double complex      zz,
     1            root,
     1            zsqrt


      double precision            dimag,
     1            imag_part


      root=zsqrt(zz)

      imag_part=dimag(root)

      if(imag_part.gt.0.) root=-root !      choose other branch to
*                               !      place on spectral sheet
      spec_sqrt=root

      return

      end



      subroutine zairy(z,deps,zai,zaipr,ier)
ci-----------------------------------------------------------------------
ci
ci    PURPOSE
ci      This subroutine computes the airy function ai(z) and its first
ci      derivative in the complex z-plane.
ci
ci      The algorithm used exploits numerous representations of the
ci      airy function and its derivative.  We record the representations
ci      here for reference
ci
ci                    sqrt(z)
ci      (1) Ai(z)   = ------- (I  (xi) - I (xi))
ci                       3      -1/3      1/3
ci
ci                    sqrt(z/3)
ci      (2) Ai(z)   = --------- K (xi)
ci                       pi      1/3
ci
ci                      2/3  -5/6
ci                     2    3
ci                  =  --------  z exp(-xi) U(5/6; 5/3; 2 xi)
ci                     sqrt(pi)
ci
ci                    sqrt(z)
ci      (3) Ai(-z)  = ------- (J  (xi) + J (xi))
ci                       3      -1/3      1/3
ci
ci                    z
ci      (4) Ai'(z)  = - (I (xi) - I  (xi))
ci                    3   2/3      -2/3
ci
ci                           z
ci      (5) Ai'(z)  = - ---------- K (xi)
ci                      pi*sqrt(3)  2/3
ci
ci                        2/3  -7/6
ci                       4    3    2
ci                  =  - -------- Z  exp(-xi) U(7/6; 7/3; 2 xi)
ci                       sqrt(pi)
ci
ci                     z
ci      (6) Ai'(-z) =  - (J (xi) - J  (xi)) ,
ci                     3   2/3      -2/3
ci
ci                 2  3/2
ci      Where xi = - z    and U( ; ; ) is the confluent hypergeometric
ci                 3
ci      Function as defined in
ci
ci      Stegun, I.A. and Abramowitz, M., Handbook of Mathematical Functions,
ci        Natl. Bureau of Standards, AMS 55, pp 504-515, 1964.
ci
ci      The asymptotic expansions derivable from these representations and
ci      Hankel's asymptotic expansions for the Bessel functions are used for
ci      large modulus of z.  The implementation has taken advantage of the
ci      error bounds given in
ci
ci      Olver, F.W.J., Error Bounds for Asymptotic Expansions, with an
ci        Application to Cylinder Functions of Large Argument, in
ci        Asymptotic Solutions of Differential Equations (Wilcox, Ed.),
ci        Wiley and Sons, pp 163-183, 1964
ci
ci      and
ci
ci      Olver, F.W.J., Asymptotics and Special Functions, Academic Press,
ci        pp 266-268, 1974.
ci
ci      For small modulus of z, a rational approximation is used.  This
ci      approximant is derived from
ci
ci      Luke, Y.L., Mathematical Functions and their Approximations,
ci        Academic Press, pp 361-363, 1975.
ci
ci      The identities given below are for Bessel functions of the first
ci      kind in terms of modified Bessel functions of the first kind are
ci      also used with the rational approximant.
ci
ci      For moderate modulus of z, three techniques are used.  Two use
ci      a backward recursion algorithm with (1), (3), (4), and (6). The
ci      third uses the confluent hypergeometric representations given by
ci      (2)and (5).  The backward recursion algorithm generates values of
ci      the modified Bessel functions of the first kind of orders + or -
ci      1/3 and + or - 2/3 for z in the right half plane.  Values for
ci      the corresponding Bessel functions of the first kind are recovered
ci      via the identities
ci
ci            J (Z) = EXP(NU PI I/2) I (Z EXP(-PI I/2)) ,
ci             nu                     nu
ci                  0 .LE. ARG(Z) .LE. PI/2
ci      AND
ci            J (Z) = EXP(-NU PI I/2) I (Z EXP(PI I/2)) ,
ci             NU                      NU
ci                 -PI/2 .LE. ARG(Z) .LT. 0 .
ci
ci      THE PARTICULAR BACKWARD RECURSION ALGORITHM USED IS DISCUSSED
ci      IN 
ci    
ci      OLVER, F.W.J, NUMERICAL SOLUTION OF SECOND-ORDER LINEAR
ci        DIFFERENCE EQUATIONS, NBS J. RES., SERIES B, VOL 71B,
ci        PP 111-129, 1967.
ci
ci      OLVER, F.W.J. AND SOOKNE, D.J., NOTE ON BACKWARD RECURRENCE
ci        ALGORITHMS, MATH. COMP. VOL 26, NO. 120, PP 941-947,
ci        OCT. 1972
ci
ci      SOOKNE, D.J., BESSEL FUNCTIONS I AND J OF COMPLEX ARGUMENT AND
ci        INTEGER ORDER, NBS J. RES., SERIES B, VOL 77B, NOS 3 & 4,
ci        PP 111-113, JULY-DECEMBER, 1973. 
ci
ci      THE FOLLOWING PAPER WAS ALSO USEFUL
ci
ci      CODY, W.J., PRELIMINARY REPORT ON SOFTWARE FOR THE MODIFIED
ci        BESSEL FUNCTIONS OF THE FIRST KIND, APPLIED MATHEMATICS
ci        DIVISION, ARGONNE NATIONAL LABORATORY, TECH. MEMO. NO. 357.
ci
ci      A BACKWARD RECURSION ALGORITHM IS ALSO USED TO COMPUTE THE
ci      CONFLUENT HYPERGEOMETRIC FUNCTION.  THE RECURSION RELATIONS
ci      AND A CONVERGENCE THEOREM ARE GIVEN IN
ci
ci      WIMP, J., ON THE COMPUTATION OF TRICOMI'S PSI FUNCTION, COMPUTING,
ci        VOL 13, PP 195-203, 1974.
ci
ci      SEE INDIVIDUAL SUBPROGRAM COMMENTS FOR ADDITIONAL DETAILS.
ci
ci 
ci    ARGUMENTS
ci      Z      DOUBLE PRECISION COMPLEX INPUT VARIABLE SET EQUAL TO
ci             THE ARGUMENT AT WHICH THE AIRY FUNCTION AND ITS DERIVATIVE
ci             ARE TO BE COMPUTED.
ci      DEPS   DOUBLE PRECISION INPUT VARIABLE SET EQUAL TO THE RELATIVE
ci             ERROR REQUIRED.  AT PRESENT, DEPS IS USED ONLY IN THE 
ci             BACKWARD RECURSION ALGORITHMS.
ci      ZAI    DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING THE
ci             VALUE COMPUTED FOR AI(Z).
ci      ZAIPR  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING THE
ci             VALUE COMPUTED FOR AI'(Z).
ci
ci    DATE OF LAST REVISION
ci      MAY 5, 1986
ci
ci    AUTHOR
ci      RICK A. WHITAKER
ci
ci-----------------------------------------------------------------------

c     type declarations for global variables

      include 'utd.i'

      double complex z, zai, zaipr
      double precision     deps
      integer    ier

c     type declarations for local variables
      double complex zxi, z2xi, zp1d4c, zi1d3, zim1d3, zi2d3,
     1           zim2d3, zepd6, zempd6, zepd3, zempd3, z1d3f,
     2           zm1d3f, z2d3f, zm2d3f, zpwh
      double precision     dabsz, dzbig, dzxir, dzxii, dzero, dziacc,
     1           drsqpi, dzsmal, d1d3, d2d3,
     2           dgm2d3, dgm1d3, d2g2d3, dsqrt3

c     type declarations for functions
      double complex zsqrt, zexp, dcmplx
      double precision     dreal, dimag, dabs

c     data statements defining constants used herein
      data zepd6 /( 8.660254037844386d-01, 5.0d-01)/,
     1     zempd6/( 8.660254037844386d-01,-5.0d-01)/,
     2     zepd3 /( 5.0d-01, 8.660254037844386d-01)/,
     3     zempd3/( 5.0d-01,-8.660254037844386d-01)/
      data dzero /0.0d+00/,
     1     d1d3  /3.333333333333333d-01/,
     2     d2d3  /6.666666666666667d-01/,
     3     dsqrt3/1.732050807568877d+00/,
     4     dgm1d3/2.588194037928068d-01/,
     5     dgm2d3/3.550280538878172d-01/,
     6     d2g2d3/1.775140269439086d-01/,
     7     drsqpi/2.820947917738781d-01/
      data dzsmal/2.5d-01/,dziacc/2.0d+00/,dzbig/15.0d+00/

c     initialize completion code
      ier = 0

      write(6,*) '> airy:'
      write(6,*) '> > z = ', z
      write(6,*) '> > small = ', dzsmal
      write(6,*) '> > big   = ', dzbig

cp    compute modulus of z for later use
      dabsz = zabs(z)
      write(6,*) '> > absz = ', dabsz
cp    check size of abs(z) and select appropriate methods
      if (dabsz .lt. dzbig)
     1then
cp      moderate or small abs(z)
cp      check for right or left half plane argument
        if (dreal(z) .ge. dzero)
     1  then
cp        argument in closed right half plane
cp        compute xi as defined in the representations in terms of
cp          bessel functions
          zpwh = zsqrt(z)
          zxi = z*zpwh
          dzxir = d2d3*dreal(zxi)
          dzxii = d2d3*dimag(zxi)
          zxi = dcmplx(dzxir,dzxii)
cp        check for abs(z) too large for accuracy of representations (1)
cp          and (4)
          if (dabsz .ge. dziacc)
     1    then
cp          use rational approximation for modified bessel functions
cp            of orders 1/3 and 2/3
            call zkairy(zxi,deps,zai,zaipr,ier)
cp          recover ai(z) and ai'(z)
            zp1d4c = zsqrt(zpwh)
            zxi = zexp(-zxi)
            zxi = dcmplx(drsqpi*dreal(zxi),drsqpi*dimag(zxi))
            zai = zxi*(zai/zp1d4c)
            zaipr = -zxi*zp1d4c*zaipr

          else
cp          check for abs(z) small enough for rational approximation
            if (dabsz .le. dzsmal)
     1      then
cp            use rational approximation along with (1) and (4)
              call zcrary(z,zi1d3,zim1d3,zi2d3,zim2d3)
cp            recover ai(z) and ai'(z)
              zim1d3 = dcmplx(dreal(zim1d3)*dgm2d3,
     1                        dimag(zim1d3)*dgm2d3)
              zi1d3 = dcmplx(dreal(zi1d3)*dgm1d3,
     1                       dimag(zi1d3)*dgm1d3)
              zai = zim1d3 - z*zi1d3
              zim2d3 = dcmplx(dreal(zim2d3)*dgm1d3,
     1                        dimag(zim2d3)*dgm1d3)
              zi2d3 = dcmplx(dreal(zi2d3)*d2g2d3,
     1                       dimag(zi2d3)*d2g2d3)
              zaipr = z*z*zi2d3 - zim2d3
              write(6,*) '> > > > > > ai = ', zai
              write(6,*) '> > > > > > aip = ', zaipr

            else
cp            use backward recurrence along with (1) and (4)
              call ziairy(zxi,deps,zi1d3,zim1d3,zi2d3,zim2d3)
cp            recover ai(z) and ai'(z)
              zai = dcmplx(d1d3*dreal(zpwh),d1d3*dimag(zpwh))*
     1              (zim1d3 - zi1d3)
              zaipr = dcmplx(d1d3*dreal(z),d1d3*dimag(z))*
     1                (zi2d3 - zim2d3)
              write(6,*) '> > > > > > ai = ', zai
              write(6,*) '> > > > > > aip = ', zaipr
            end if

          end if

        else
cp        z lies in left half plane
cp        compute xi as defined in the representations in terms of
cp          bessel functions
          zpwh = zsqrt(-z)
          zxi = -z*zpwh
          dzxir = d2d3*dreal(zxi)
          dzxii = d2d3*dimag(zxi)
          zxi = dcmplx(dzxir,dzxii)
cp        set up arguments to recover bessel functions of the first
cp          kind in (3) and (6)
          if (dzxii .ge. dzero)
     1    then
cp          argument lies in upper half plane, so use appropriate
cp            identity
            z2xi = dcmplx(dzxii,-dzxir)
            z1d3f = zepd6
            zm1d3f = zempd6
            z2d3f = zepd3
            zm2d3f = zempd3

          else
cp          argument lies in lower half plane, so use appropriate
cp            identity
            z2xi = dcmplx(-dzxii,dzxir)
            z1d3f = zempd6
            zm1d3f = zepd6
            z2d3f = zempd3
            zm2d3f = zepd3
          end if

cp        use approximation depending on size of z
          if (dabsz .le. dzsmal)
     1    then
cp          use rational approximation
            zxi = -z
            call zcrary(z,zi1d3,zim1d3,zi2d3,zim2d3)
cp          recover ai(z) and ai'(z)
              zim1d3 = dcmplx(dreal(zim1d3)*dgm2d3,
     1                        dimag(zim1d3)*dgm2d3)
              zi1d3 = dcmplx(dreal(zi1d3)*dgm1d3,
     1                       dimag(zi1d3)*dgm1d3)
              zai = zim1d3 - z*zi1d3
c              zai = zm1d3f*zim1d3 + z*z1d3f*zi1d3
              zim2d3 = dcmplx(dreal(zim2d3)*dgm1d3,
     1                        dimag(zim2d3)*dgm1d3)
              zi2d3 = dcmplx(dreal(zi2d3)*d2g2d3,
     1                       dimag(zi2d3)*d2g2d3)
              zaipr = z*z*zi2d3 - zim2d3
c              zaipr = z*z*z2d3f*zi2d3 - zm2d3f*zim2d3
            write(6,*) '> > > > > ai = ', zai
            write(6,*) '> > > > > aip = ', zaipr

          else
cp          use backward recurrence
            call ziairy(z2xi,deps,zi1d3,zim1d3,zi2d3,zim2d3)
cp          recover ai(z) and ai'(z)
            zai = dcmplx(d1d3*dreal(zpwh),d1d3*dimag(zpwh))*
     1            (zm1d3f*zim1d3 + z1d3f*zi1d3)
            zaipr = dcmplx(d1d3*dreal(z),d1d3*dimag(z))*
     1              (zm2d3f*zim2d3 - z2d3f*zi2d3)
            write(6,*) '> > > > > ai = ', zai
            write(6,*) '> > > > > aip = ', zaipr
          end if

        end if

      else
cp      abs(z) is large
cp      check arg(z) to see which asymptotic form is appropriate
        if (dreal(z) .ge. dzero .or.
     1      dabs(dimag(z)) .ge. -dsqrt3*dreal(z))
     2  then
cp        abs(arg(z)) .le. 2*dpi/3  -  use asymptotic expansion for
cp          this region
          call zasary(z,zai,zaipr)

        else
cp        abs(arg(-z)) .lt. dpi/3  -  use asymptotic expansion for
cp          this region
          call zasaly(z,zai,zaipr)
        end if

      end if
      return
      end


      subroutine ziairy(z,deps,zi1d3,zim1d3,zi2d3,zim2d3)
ci----------------------------------------------------------------------
ci
ci    PURPOSE
ci      COMPUTE THE MODIFIED BESSEL FUNCTIONS OF THE FIRST KIND ORDERS 
ci      + OR - 1/3 AND + OR - 2/3 NEEDED TO COMPUTE THE AIRY FUNCTIONS
ci      AND THEIR DERIVATIVES FROM THEIR REPRESENTATION IN TERMS OF THE
ci      MODIFIED BESSEL FUNCTIONS.  THIS PROGRAM IS ONLY USED FOR Z
ci      LESS THAN TWO IN MODULUS AND IN THE CLOSED RIGHT HALF PLANE.
ci      THIS STEMS FROM THE FACT THAT THE VALUES OF THE MODIFIED 
ci      BESSEL FUNCTIONS OCCURING IN THE REPRESENTATIONS OF THE AIRY
ci      FUNCTIONS AND THEIR DERIVATIVES ARE ALMOST EQUAL FOR Z LARGE
ci      IN THE RIGHT HALF PLANE.  THIS MEANS THAT LOSS OF SIGNIFICANCE
ci      OCCURS IF THESE REPRESENTATIONS ARE USED FOR Z TO LARGE IN
ci      MAGNITUDE.  THIS ALGORITHM IS ALSO NOT USED FOR Z TOO SMALL,
ci      SINCE A LOW ORDER RATIONAL APPROXIMATION CAN BE USED INSTEAD.
ci
ci      THIS SUBROUTINE IS AN IMPLEMENTATION OF A MODIFIED VERSION OF
ci      MILLER'S ALGORITHM FOR COMPUTATION BY BACKWARD RECURRENCE
ci      FROM THE RECURRENCE RELATION
ci
ci        I-SUB-(NU-1) = (2*NU/Z)*I-SUB-NU + I-SUB-(NU+1)
ci
ci      SATISFIED BY THE MODIFIED BESSEL FUNCTIONS OF THE FIRST KIND.
ci      THE NORMALIZATION RELATIONSHIP USED IS
ci             NU Z 
ci        (Z/2)  E                 INF  (K+NU)*GAMMA(2 NU+K)
ci        -----------  = I (Z) + 2 SUM  -------------------- I (Z) .
ci        GAMMA(NU+1)     NU       K=1   K! GAMMA(1 + 2 NU)   NU+K
ci
ci      THIS MODIFICATION OF THE ALGORITHM IS GIVEN IN PART IN
ci
ci      OLVER, F.W.J. AND SOOKNE, D.J., NOTE ON BACKWARD RECURRENCE
ci        ALGORITHMS, MATH. OF COMP., VOL. 26, NO. 120, OCT. 1972.
ci
ci      AND FURTHER ELABORATED FOR THE BESSEL FUNCTIONS IN
ci  
ci      SOOKNE, D.J., BESSEL FUNCTIONS I AN J OF COMPLEX ARGUMENT 
ci        AND INTEGER ORDER, J. RES. NBS - SERIES B, VOL 77B, NOS.
ci        3 & 4, JULY-DECEMBER, 1973.
ci
ci      INSIGHT WAS ALSO GAINED FROM
ci
ci      CODY, W.J., PRELIMINARY REPORT ON SOFTWARE FOR THE MODIFIED
ci        BESSEL FUNCTIONS OF THE FIRST KIND, ARGONNE NATIONAL
ci        LABORATORY, APPLIED MATHEMATICS DIVISION, TECH. MEMO. 
ci        NO. 357, AUGUST, 1980.
ci
ci      CODY IMPLEMENTS THE ALGORITHM OF SOOKNE FOR FRACTIONAL ORDER
ci      AND NONNEGATIVE REAL ARGUMENT.  LIKE CODY, WE DO NOT CHANGE
ci      THE CONVERGENCE TESTING MECHANISM IN ANY SUBSTANTIAL WAY.
ci      HOWEVER, WE DO TRIM THE OVERHEAD BY MAKING THE ADDITIONAL
ci      ASSUMPTION THAT PERFORMING THE CONVERGENCE TEST FOR THE
ci      FUNCTIONS OF ORDER 2/3 WILL SUFFICE FOR ORDER 1/3 AS WELL.
ci      THIS ASSUMPTION HAS NOT BEEN ESTABLISHED BY RIGOUROUS
ci      ANALYSIS AT THIS TIME.  TWO ADDITIONAL CHANGES HAVE BEEN
ci      MADE FOR THE SAKE OF SPEED.  FIRST, THE STRONG CONVERGENCE
ci      CRITERIA IS COMPUTED IN SINGLE PRECISION SINCE MAGNITUDE IS
ci      THE MOST IMPORTANT THING HERE.  SECOND, THE TESTS ARE
ci      PERFORMED IN THE 1-NORM INSTEAD OF THE USUAL EUCLIDEAN
ci      NORM USED IN THE COMPLEX PLANE.  TO INSURE THE VALIDITY
ci      OF THE RESULTS, THE INEQUALITY
ci                                           2    2 
ci        ABS(X) + ABS(Y) .LE. SQRT(2) SQRT(X  + Y ) 
ci
ci      WAS USED TO MODIFY THE CONVERGENCE TESTS.
ci
ci      NOTE ALSO THAT FOR THE SAKE OF SPEED AND THE FACT THAT THIS
ci      SUBROUTINE WILL BE DRIVEN BY ANOTHER, CHECKS THAT ARE NOT
ci      ABSOLUTELY NECESSARY ARE NOT MADE.  UNDER THESE ASSUMPTIONS
ci      AN ERROR RETURN IS NOT NEEDED.
ci
ci      ARGUMENTS
ci        Z      DOUBLE PRECISION COMPLEX INPUT VARIABLE SET EQUAL
ci               TO THE ARGUMENT AT WHICH THE MODIFIED BESSEL
ci               FUNCTIONS COMPUTED BY THIS PROGRAM ARE TO BE
ci               EVALUATED.
ci        DEPS   DOUBLE PRECISION INPUT VARIABLE SET EQUAL TO THE
ci               MAXIMUM RELATIVE ERROR REQUIRED IN THE RESULTS.
ci        ZI1D3  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING
ci               THE VALUE OF I-SUB-1/3 (Z) COMPUTED.
ci        ZIM1D3 DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING
ci               THE VALUE OF I-SUB-(-1/3) COMPUTED.
ci        ZI2D3  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING
ci               THE VALUE OF I-SUB-2/3 (Z) COMPUTED.
ci        ZIM2D3 DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAINING
ci               THE VALUE OF I-SUB-(-2/3) (Z) COMPUTED.
ci
ci      DATE OF LAST REVISION
ci        APRIL 28, 1986
ci
ci      AUTHOR
ci        RICK A. WHITAKER
ci
ci----------------------------------------------------------------------

c     type declarations for global variables

      include 'utd.i'

      double complex z, zi1d3, zim1d3, zi2d3, zim2d3
      double precision     deps

c     type declarations for local variables
      double complex z1dz, zplst2, zp2, zpold2, zplst1, zp1, zpold1, 
     1           zsum1, zsum2, zd2pow, zero, zone
      double precision     dzr, dzi, dzrabs, dziabs, du, dv, d1dzi,
     1           d1dzr, dmodz, dtest, dn, d2n, dzp2r, d2d3,
     2           dzp2i, dnpn1, dnpn2, dnp2n1, dnp2n2, dfac1, dfac2,
     3           done, dtwo, d2sqr2, dhalf, dgm4d3, dgm5d3, d1d3,
     4           d4d3, d5d3, d8d3, d10d3, d14d3, d16d3
      double precision   spr, spi, splstr, splsti, slamn, skn, sone,
     1           stwo
      logical    lstcnv

      integer    n, l, nend

c     data statements defining constants used herein
cph
      data sone/1.0/, stwo/2.0/
cph
      data zero/(0.0d+00,0.0d+00)/, zone/(1.0d+00,0.0d+00)/
      data done/1.0d+00/, dtwo/2.0d+00/, dhalf/5.0d-01/,
     1     d1d3  /3.333333333333333d-01/, d2d3  /6.666666666666667d-01/,
     2     d4d3  /1.333333333333333d+00/, d5d3  /1.666666666666667d+00/,
     3     d8d3  /2.666666666666667d+00/, d10d3 /3.333333333333333d+00/,
     4     d14d3 /4.666666666666667d+00/, d16d3 /5.333333333333333d+00/,
     5     dgm4d3/8.929795115692492d-01/, dgm5d3/9.027452929509336d-01/,
     6     d2sqr2/2.828427124746190d+01/

      write(6,*) '> airy_bessel_i:'
      write(6,*) '> > z = ', z

cp    compute 1/z for use in recurrence for speed and abs(z)
      dzr = dreal(z)
      dzi = dimag(z)
      dzrabs = dabs(dzr)
      dziabs = dabs(dzi)
      du = dmax1(dzrabs,dziabs)
      dv = dmin1(dzrabs,dziabs)
      d1dzi = (done + (dv/du)**2)
      dmodz = du*dsqrt(d1dzi)
      d1dzi = du*d1dzi
      d1dzr = (dzr/du)/d1dzi
      d1dzi = -(dzi/du)/d1dzi
      z1dz = dcmplx(d1dzr,d1dzi)

cp    initialize for forward recursion based on order 2/3
      n = 0
      d2n = dfloat(n + n) + d4d3
      zplst2 = zone
      zp2 = dcmplx(d2n*d1dzr,d2n*d1dzi)

cp    calculate weak convergence test and set flag for weak 
cp      convergence loop
      dtest = d2sqr2/deps
      lstcnv = .false.


cp    loop until weak and strong convergence tests satisfied when
cp               recurring forward
   10   continue

cp      loop until current convergence test satisfied
   20     continue
cp        update n dependent quantities
          n = n + 1
          d2n = d2n + dtwo
cp        interchange values
          zpold2 = zplst2
          zplst2 = zp2
cp        recur forward one step
          zp2 = dcmplx(d2n*dreal(zplst2),d2n*dimag(zplst2))*z1dz
     1            + zpold2

cp        check if convergence test (in 1-norm) satisfied

          if (dabs(dreal(zp2)) + dabs(dimag(zp2)) .ge. dtest)
     1      go to 30
cp        end if

          go to 20
cp      repeat

   30   continue

cp      if strong convergence, then weak and strong convergence
          if (lstcnv) go to 40
cp      end if

cp      calculate strong convergence test in single precision.  see
cp        the olver and sookne papers cited for details.
        spr = dreal(zp2)
        spi = dimag(zp2)
        splstr = dreal(zplst2)
        splsti = dimag(zplst2)
cp      compute scale factor to avoid possible overflow
        slamn = dmax1(abs(spr),abs(spi))
cp      compute the k-sub-n of strong convergence lemma
        skn = sqrt(((spr/slamn)**2 + (spi/slamn)**2)/
     1             ((splstr/slamn)**2 + (splsti/slamn)**2))
cp      compute quantity needed for lambda-sub-n of strong 
cp        convergence lemma
        slamn = dble(n+1)/dmodz
cp      determine appropriate value for rho-sub-n of lemma
        if (skn + sone/skn .gt. stwo*slamn) skn = slamn +
     1    sqrt(slamn*slamn - sone)
cp      compute test value - sqrt(2) multiple already included
        dtest = dtest*dble(sqrt(skn-sone/skn))
cp      set strong convergence test flag
        lstcnv = .true.
        go to 10
   40   continue
cp    repeat

cp    prepare for backward recurrence for both orders 1/3 and 2/3
      dn = dfloat(n)
      n = n + 1
      d2n = dfloat(n+n)
      zplst1 = zero
      zplst2 = zero
cp    carefully compute 1/zp2 to avoid overflow in complex divide
      dzp2r = dreal(zp2)
      dzp2i = dimag(zp2)
      dzrabs = dabs(dzp2r)
      dziabs = dabs(dzp2i)
      du = dmax1(dzrabs,dziabs)
      dv = dmin1(dzrabs,dziabs)
      d1dzi = du*(done + (dv/du)**2)
      d1dzr = (dzp2r/du)/d1dzi
      d1dzi = -(dzp2i/du)/d1dzi
      zp1 = dcmplx(d1dzr,d1dzi)
      zp2 = zp1
cp    set up n dependent parameters used in normalization sum
      dnpn1 = dn + d1d3
      dnpn2 = dn + d2d3
      dnp2n1 = (dn - done) + d2d3
      dnp2n2 = (dn - done) + d4d3
cp    initialize normalization sum
      dfac1 = dnpn1*dnp2n1/dn
      zsum1 = dcmplx(dfac1*dreal(zp1),dfac1*dimag(zp1))
      dfac2 = dnpn2*dnp2n2/dn
      zsum2 = dcmplx(dfac2*dreal(zp2),dfac2*dimag(zp2))
cp    set ending loop index to correspond to k=1 term of the
cp      normalization relationship
      nend = n - 3

cp    if backward recurrence loop will be nontrivial
        if (nend .gt. 0)
     1then
cp      loop until backward recursion to k=1 term of normalization
          do 50 l=1,nend
cp        update n dependent quantities
          n = n - 1
          d2n = d2n - dtwo
          dfac1 = d2n + d2d3
          dfac2 = d2n + d4d3
cp        interchanges for order 1/3 recurrence
          zpold1 = zplst1
          zplst1 = zp1
cp        recur back one step for order 1/3
          zp1 = dcmplx(dfac1*dreal(zplst1),dfac1*dimag(zplst1))*
     1          z1dz + zpold1
cp        interchanges for order 2/3 recurrence
          zpold2 = zplst2
          zplst2 = zp2
cp        recur back one step for order 2/3
          zp2 = dcmplx(dfac2*dreal(zplst2),dfac2*dimag(zplst2))*
     1          z1dz + zpold2
cp        update quantities for computing normalization sums
          dn = dn - done
          dnpn1 = dn + d1d3
          dnp2n1 = dn - d1d3
          dnpn2 = dn + d2d3
          dnp2n2 = dnpn1
          dfac1 = dnp2n1/dn
          dfac2 = dnp2n2/dn
cp        update normalization sums
          zsum1 = (zsum1 + dcmplx(dnpn1*dreal(zp1),
     1                            dnpn1*dimag(zp1)))
          zsum1 = dcmplx(dfac1*dreal(zsum1),dfac1*dimag(zsum1))
          zsum2 = (zsum2 + dcmplx(dnpn2*dreal(zp2),
     1                            dnpn2*dimag(zp2)))
          zsum2 = dcmplx(dfac2*dreal(zsum2),dfac2*dimag(zsum2))
   50     continue
cp      repeat

      end if
      write(6,*) '> > sum1 = ', zsum1
      write(6,*) '> > sum2 = ', zsum2

cp    perform last two recurrence steps for order 1/3
      zpold1 = zplst1
      zplst1 = zp1
      zp1 = dcmplx(d14d3*dreal(zplst1),d14d3*dimag(zplst1))*
     1      z1dz + zpold1
      zsum1 = zsum1 + dcmplx(d4d3*dreal(zp1),d4d3*dimag(zp1))
      zpold1 = zplst1
      zplst1 = zp1
      zp1 = dcmplx(d8d3*dreal(zplst1),d8d3*dimag(zplst1))*z1dz +
     1      zpold1
      zsum1 = (zsum1 + zsum1) + zp1
      write(6,*) '> > sum1 = ', zsum1

cp    compute scale factor and scale results for order 1/3 case
      zd2pow = dcmplx(dhalf*dzr,dhalf*dzi)**(-d1d3)
      zpold1 = zd2pow*zexp(-z)
      zsum1 = zpold1*dcmplx(dgm4d3*dreal(zsum1),dgm4d3*dimag(zsum1))
      zplst1 = zplst1/zsum1
      zi1d3 = zp1/zsum1
      write(6,*) '> > zpold1 = ', zpold1
      write(6,*) '> > zd2pow = ', zd2pow
      write(6,*) '> > sum1   = ', zsum1

cp    perform last two recurrence steps for order 2/3
      zpold2 = zplst2
      zplst2 = zp2
      zp2 = dcmplx(d16d3*dreal(zplst2),d16d3*dimag(zplst2))*
     1      z1dz + zpold2
      zsum2 = zsum2 + dcmplx(d5d3*dreal(zp2),d5d3*dimag(zp2))
      zpold2 = zplst2
      zplst2 = zp2
      zp2 = dcmplx(d10d3*dreal(zplst2),d10d3*dimag(zplst2))*z1dz +
     1      zpold2
      zsum2 = (zsum2 + zsum2) + zp2
      write(6,*) '> > sum2 = ', zsum2

cp    compute scale factor and scale results for order 2/3 case
      zsum2 = zd2pow*zpold1*dcmplx(dgm5d3*dreal(zsum2),
     1                             dgm5d3*dimag(zsum2))
      zplst2 = zplst2/zsum2
      zi2d3 = zp2/zsum2
      write(6,*) '> > zpold1 = ', zpold1
      write(6,*) '> > zd2pow = ', zd2pow
      write(6,*) '> > sum2   = ', zsum2

cp    recur back one step from order 1/3 to get order -2/3
      zim2d3 = dcmplx(d2d3*dreal(zi1d3),d2d3*dimag(zi1d3))*z1dz +
     1         zplst1

cp    recur back one step from order 2/3 to get order -1/3
      zim1d3 = dcmplx(d4d3*dreal(zi2d3),d4d3*dimag(zi2d3))*z1dz +
     1         zplst2

      return
      end



      subroutine zkairy(z,deps,zk1d3,zk2d3,ier)
ci----------------------------------------------------------------------
ci
ci    PURPOSE
ci      COMPUTE APPROXIMATIONS TO THE MODIFIED BESSEL FUNCTIONS OF THE
ci      SECOND KIND OF ORDERS 1/3 AND 2/3 FOR MODERATE ARGUMENTS.  MORE
ci      SPECIFICALLY, THE PROGRAM COMPUTES
ci
ci        E (Z) = EXP(Z) SQRT(2 Z/PI) K (Z), FOR NU = 1/3 AND NU = 2/3.
ci         NU                          NU
ci
ci      THIS SUBPROGRAM USES A RATIONAL APPROXIMATION GIVEN IN
ci
ci      LUKE, Y.L., MATHEMATICAL FUNCTIONS AND THEIR APPROXIMATIONS,
ci        ACADEMIC PRESS, PP 366-367, 1975.
ci
ci      THOUGH THE APPROXIMATION CONVERGES IN ABS(ARG(Z)) .LE. PI,
ci      THE CONVERGENCE WEAKENS AS ABS(ARG(Z)) INCREASES.  ALSO, IN
ci      THE CASE OF REAL ORDER BETWEEN 0 AND 1, CONVERGENCE WEAKENS
ci      AS THE ORDER APPROACHES 1.  FOR THESE REASONS, WE ONLY USE
ci      THIS CODE FOR ABS(ARG(Z)) .LE. 3*PI/4 AND THE CONVERGENCE TEST
ci      IS PERFORMED ONLY FOR ORDER 2/3.
ci
ci      THE CODING OF THIS SUBPROGRAM IS ALSO INFLUENCED BY THE FACT
ci      THAT IT WILL ONLY BE USED FOR ABOUT 2 .LE. ABS(Z) .LE. 15.
ci      HENCE, CERTAIN CONSIDERATIONS OF OVERFLOW, UNDERFLOW, AND
ci      LOSS OF SIGNIFICANCE ARE UNIMPORTANT FOR OUR PURPOSE.
ci
ci      ARGUMENTS
ci        Z      DOUBLE PRECISION COMPLEX INPUT VARIABLE SET EQUAL 
ci               TO THE VALUE FOR WHICH THE QUANTITY E-SUB-NU IS TO
ci               BE COMPUTED.  IT IS RECOMMENDED THAT ABS(Z) NOT BE
ci               TOO SMALL AND THAT ABS(ARG(Z)) .LE. 3*PI/4.
ci        DEPS   DOUBLE PRECISION INPUT VARIABLE SET EQUAL TO THE
ci               MAXIMUM RELATIVE ERROR ALLOWABLE IN THE COMPUTED
ci               RESULTS.  THE RELATIVE ERROR TEST IS BASED ON THE
ci               COMPARISON OF SUCCESSIVE ITERATES.
ci        ZK1D3  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAIN-
ci               ING THE VALUE COMPUTED FOR E-SUB-(1/3) OF Z.
ci        ZK2D3  DOUBLE PRECISION COMPLEX OUTPUT VARIABLE CONTAIN-
ci               ING THE VALUE COMPUTED FOR E-SUB-(2/3) OF Z.
ci        IER    INTEGER OUTPUT VARIABLE SET EQUAL TO A COMPLETION
ci               CODE.  IER = 0 INDICATES NORMAL COMPLETION.  IF
ci               IER = 129, THEN CONVERGENCE FAILED IN 100 
ci               ITERATIONS.
ci
ci               REMARK:  ACCORDING TO PUBLISHED INFORMATION ABOUT
ci                        THE BEHAVIOUR OF THE ERROR FOR ORDERS
ci                        1/3 AND 2/3, IER = 129 SHOULD NEVER OCCUR
ci                        FOR THE DOMAIN OF Z THAT WE RECOMMEND.
ci                        INDEED, IN THE WORST CASE, SAY, Z=2 AND
ci                        ARG(Z) = 3*PI/4, WE EXPECT 20 ITERATIONS
ci                        TO GIVE 7 OR 8 DECIMALS OF ACCURACY.
ci
ci    DATE OF LAST REVISION
ci      MAY 7, 1986
ci
ci    AUTHOR
ci      RICK A. WHITAKER
ci
ci----------------------------------------------------------------------

c     type declarations for global variables

      include 'utd.i'

      double complex z, zk1d3, zk2d3
      double precision     deps
      integer    ier

c     type declarations for local variables
      double complex zone, zf10, zf11, zf12, zf13, zphi10,
     1           zphi11, zphi12, zphi13, zf20, zf21, zf22,
     2           zf23, zphi20, zphi21, zphi22, zphi23, zfact1,
     3           zfact2, zratnw, zratol
      double precision     dtwo, deight, d16, d24, dthree, dfive, 
     1           delti, dan1i, dan2i, dp12i, dp22i, dfco(8),
     2           dp13i, dp23i, dp11i, dp21i, dan1, dphico(6),
     3           dan2, dp11, dp12, dp13, dp21, dp22,
     4           dp23, delt, deta, dq, dgamm, dgam,
     5           danm1, danm2, dqzr, dqzi
      integer    i
c    
c     type declarations for debug variables
      double complex zrat1

c     type declarations for functions
      double complex dcmplx

c     data statements defining constants used herein
      data dtwo/2.0d+00/,deight/8.0d+00/,d16/16.0d+00/d24/24.0d+00/,
     1     dthree/3.0d+00/,dfive/5.0d+00/delti/32.0d+00/,
     2     dan1i/4.855555555555555d+01/dan2i/4.722222222222222d+01/,
     3     dp12i/3.144444444444444d+01/,dp22i/3.277777777777777d+01/,
     4     dp13i/-9.259259259259259d-01/,dp23i/1.296296296296296d+00/,
     5     dp11i/-7.907407407407407d+01/,dp21i/-8.129629629629629d+01/
      data zone/(1.0d+00,0.0d+00)/

c     data statements defining coefficients for initial polynomials
      data dfco/144.0d+00,77.0d+00,62208.0d+00,95472.0d+00,
     1          17017.0d+00,65.0d+00,90288.0d+00,13585.0d+00/
      data dphico/67.0d+00,91152.0d+00,12697.0d+00,79.0d+00,
     1            96336.0d+00,19633.0d+00/

      write(6,*) '> airy_bessel_k: z = ', z

      ier = 0

cp    initialize polynomials for recurrence
      zf10 = zone
      zf20 = zone
      zphi10 = zone
      zphi20 = zone
      zf11 = dcmplx((dfco(1)*dreal(z)+dfco(2))/dfco(2),
     1               dfco(1)*dimag(z)/dfco(2))
      zf12 = dcmplx(dfco(3)*dreal(z)+dfco(4),dfco(3)*dimag(z))*z
      zf12 = dcmplx((dreal(zf12)+dfco(5))/dfco(5),
     1               dimag(zf12)/dfco(5))
      zf21 = dcmplx((dfco(1)*dreal(z)+dfco(6))/dfco(6),
     1               dfco(1)*dimag(z)/dfco(6))
      zf22 = dcmplx(dfco(3)*dreal(z)+dfco(7),dfco(3)*dimag(z))*z
      zf22 = dcmplx((dreal(zf22)+dfco(8))/dfco(8),
     1               dimag(zf22)/dfco(8))
      zphi11 = dcmplx((dfco(1)*dreal(z)+dphico(1))/dfco(2),
     1                 dimag(zf11))
      zphi12 = dcmplx(dfco(3)*dreal(z)+dphico(2),
     1                dfco(3)*dimag(z))*z
      zphi12 = dcmplx((dreal(zphi12)+dphico(3))/dfco(5),
     1                 dimag(zphi12)/dfco(5))
      zphi21 = dcmplx((dfco(1)*dreal(z)+dphico(4))/dfco(6),
     1                 dimag(zf21))
      zphi22 = dcmplx(dfco(3)*dreal(z)+dphico(5),
     1                 dfco(3)*dimag(z))*z
      zphi22 = dcmplx((dreal(zphi22)+dphico(6))/dfco(8),
     1                 dimag(zphi22)/dfco(8))

      write(6,*) '> > f10 = ', zf10
      write(6,*) '> > f20 = ', zf20
      write(6,*) '> > f11 = ', zf11
      write(6,*) '> > f12 = ', zf12
      write(6,*) '> > f21 = ', zf21
      write(6,*) '> > f22 = ', zf22

      write(6,*) '> > phi10 = ', zphi10
      write(6,*) '> > phi20 = ', zphi20
      write(6,*) '> > phi11 = ', zphi11
      write(6,*) '> > phi12 = ', zphi12
      write(6,*) '> > phi21 = ', zphi21
      write(6,*) '> > phi22 = ', zphi22

cp    initialize for recursion
      zratol = zphi22/zf22
      zrat1 = zphi12/zf12
c      write(6,6010) zratol, zrat1
c 6010 format(/' zratol = (',d23.16,',',d23.16,')'
c     1       /' zrat1  = (',d23.16,',',d23.16,')')
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

cp    loop until maximum iterations used
cp         or    convergence
        do 10 i=1,100
cp      evaluate next term in recurrence for order 1/3 polynomials
        dqzr = dq*dreal(z)
        dqzi = dq*dimag(z)
        zfact1 = dcmplx(dqzr-dp11,dqzi)
        zfact2 = dcmplx(dqzr-dp12,dqzi)
        zf13 = zfact1*zf12 + zfact2*zf11 - dcmplx(dp13*dreal(zf10),
     1                                            dp13*dimag(zf10))
        zf13 = dcmplx(dreal(zf13)/dan1,dimag(zf13)/dan1)
        zphi13 = zfact1*zphi12 + zfact2*zphi11 -
     1           dcmplx(dp13*dreal(zphi10),dp13*dimag(zphi10))
        zphi13 = dcmplx(dreal(zphi13)/dan1,dimag(zphi13)/dan1)
cp      evaluate next term in recurrence for order 2/3 polynomials
        zfact1 = dcmplx(dqzr-dp21,dqzi)
        zfact2 = dcmplx(dqzr-dp22,dqzi)
        zf23 = zfact1*zf22 + zfact2*zf21 - dcmplx(dp23*dreal(zf20),
     1                                            dp23*dimag(zf20))
        zf23 = dcmplx(dreal(zf23)/dan2,dimag(zf23)/dan2)
        zphi23 = zfact1*zphi22 + zfact2*zphi21 -
     1           dcmplx(dp23*dreal(zphi20),dp23*dimag(zphi20))
        zphi23 = dcmplx(dreal(zphi23)/dan2,dimag(zphi23)/dan2)

cp      check for convergence
        zratnw = zphi23/zf23
c        write(6,6020) zf23,zphi23,zratnw
c 6020 format(/' zf23   = (',d23.16,',',d23.16,')'
c     1       /' zphi23 = (',d23.16,',',d23.16,')'
c     2       /' zratnw = (',d23.16,',',d23.16,')')
        zrat1 = zphi13/zf13
c        write(6,6030) zf13,zphi13,zrat1
c 6030 format(/' zf13   = (',d23.16,',',d23.16,')'
c     1       /' zphi13 = (',d23.16,',',d23.16,')'
c     2       /' zrat1  = (',d23.16,',',d23.16,')')


        if (zabs(zratnw-zratol)
     1      .lt. deps*zabs(zratnw)) go to 20

cp      prepare for next iteration
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
c        write(6,6100) delt,dp11,dp12,dp13,dan1,deta,danm1,dgamm,
c     1    dgam,dq
c 6100 format(/' delt = ',d23.16,2x,'dp11 = ',d23.16
c     1       /' dp12 = ',d23.16,2x,'dp13 = ',d23.16
c     2       /' dan1 = ',d23.16,2x,'deta = ',d23.16
c     3       /' danm1 = ',d23.16,2x,'dgamm = ',d23.16
c     4       /' dgam = ',d23.16,2x,'dq = ',d23.16)
   10   continue
cp    repeat
cp    . maximum iterations exceeded
        ier = 129
        return
cp    . convergence
   20   continue
        zk2d3 = zratnw
        zk1d3 = zphi13/zf13
cp    resume
      return
      end



      subroutine zcrary(z,zf1d3,zfm1d3,zf2d3,zfm2d3)
ci----------------------------------------------------------------------
ci
ci    PURPOSE
ci      This subroutine computes rational approximations to the
ci      hypergeometric functions related to the modified Bessel
ci      functions of orders nu = + or - 1/3 and + or - 2/3.  That is,
ci      A(z)/B(z), Where A(z) and B(z) are cubic polynomials with
ci      real coefficients, approximates
ci
ci        gamma(nu+1)                    2 
ci        ----------- I (Z) =  F (;nu+1;z /4) ,
ci         (Z/2)**nu   nu     0 1
ci     
ci      Where the function on the right is a generalized Gaussian
ci      hypergeometric function.  For abs(z) .le. 1/4  and
ci      abs(arg(z)) .le. pi/2, the approximations are accurate to
ci      about 16 decimals.
ci
ci      For further details including the four term recurrence
ci      relation satisfied by the numerator and denominator poly-
ci      nomials in the higher order approximants, see
ci
ci      Luke, Y.L., Mathematical Functions and their Approximations,
ci        Academic Press, pp 361-363, 1975.
ci
ci      An asymptotic expression for the error is given as well as
ci      other useful expressions in the event one wants to extend
ci      this subroutine to incorporate higher order approximants.
ci
ci      Note also that for the sake of speed and the fact that this
ci      subroutine will be driven by another, checks that are not
ci      absolutely necessary are not made.  Under these assumptions
ci      an error return is not needed.
ci
ci      ARGUMENTS
ci        z      Double precision complex input variable set equal
ci               to the argument at which the hypergeometric given
ci               above is to be evaluated.  Since the approximation
ci               is of fixed order, abs(z) must be small to insure
ci               sufficient accuracy of the computed results.
ci        zf1d3  Double precision complex output variable containing
ci               the approximate value of the hypergeometric
ci               function related to the modified Bessel function
ci               of order 1/3.
ci        zfm1d3 Double precision complex output variable containing
ci               the approximate value of the hypergeometric
ci               function related to the modified Bessel function
ci               of order -1/3.
ci        zf2d3  Double precision complex output variable containing
ci               the approximate value of the hypergeometric
ci               function related to the modified Bessel function
ci               of order 2/3.
ci        zfm2d3 Double precision complex output variable containing
ci               the approximate value of the hypergeometric
ci               function related to the modified Bessel function
ci               of order -2/3.
ci
ci    Date of last revision
ci      May 2, 1986
ci
ci    Author
ci      Rick A. Whitaker
ci
ci----------------------------------------------------------------------
C
C     Type declarations for global variables

      include 'utd.i'

      double complex z, zf1d3, zfm1d3, zf2d3, zfm2d3

c     type declarations for local variables
      double complex zone, zzz
      double precision     dx, dy, dr, ds, dt, dtwo,
     1           dal, dbe, da1d3(4), dam1d3(4),
     2           da2d3(4), dam2d3(4), db1d3(4),
     3           dbm1d3(4), db2d3(4), dbm2d3(4),
     4           dsmall

c     type declarations for functions used
      double complex dcmplx
      double precision     dreal, dimag

c     data statements defining routine constants used
      data dtwo/2.0d+00/,zone/(1.0d+00,1.0d+00)/

c     data statements defining machine dependent constant
c       dsmall is the 1/3 root of the smallest magnitude
c       floating-point number representable on the machine
c     *** temporary approximate value for vax 11/780 f_floating
      data dsmall/1.0d-12/

c     data statements defining coefficients for cubic polynomials
      data da1d3 /81d+00,32400d+00,2585520d+00,37920960d+00/,
     1     db1d3 /-35d+00,5040d+00,-574560d+00,37920960d+00/,
     2     dam1d3/81d+00,22680d+00,1156680d+00,7711200d+00/,
     3     dbm1d3/-10d+00,1260d+00,-128520d+00,7711200d+00/,
     4     da2d3 /162d+00,75735d+00,7270560d+00,139352400d+00/,
     5     db2d3 /-110d+00,16830d+00,-2019600d+00,139352400d+00/,
     6     dam2d3/162d+00,36855d+00,1415232d+00,4481568d+00/,
     6     dbm2d3/-7d+00,819d+00,-78624d+00,4481568d+00/

cp    check to see if z**3 will underflow and act accordingly

      write(6,*) '> airy_hyperg_rational:'
      write(6,*) '> > z = ', z

      if (zabs(z) .lt. dsmall)
     1then
cp      all ratios are 1 
        zf1d3  = zone
        zfm1d3 = zone
        zf2d3  = zone
        zfm2d3 = zone
      else
cp      initialize argument dependent quantities used throughout
        zzz = z**3
        dx = dreal(zzz)
        dy = dimag(zzz)
        dr = dtwo*dx
        ds = dx*dx + dy*dy

c       all of the following polynomial evaluations are done using
c       a modified of horner's rule which exploits the fact that
c       the polynomial coefficients are all real.  the algorithm is
c       discussed in detail in

c       knuth, d.e., the art of computer programming: seminumerical
c         algorithms (vol. 2), addison-wesley, pp 423-424, 1969.

c       if n is the degree of the polynomial, n-3 multiplies are
c       saved and 4*n-6 additions are saved.
c  
cp      evaluate numerator polynomial for nu=1/3 approximant
        dal = da1d3(1)
        dt  = ds*dal
        dal = da1d3(2) + dr*dal
        dbe = da1d3(3) - dt
        dt  = ds*dal
        dal = dbe + dr*dal
        dbe = da1d3(4) - dt
        zf1d3 = dcmplx(dal*dx+dbe,dal*dy)
cp      evaluate denominator polynomial for nu=1/3 approximant and
cp        compute ratio of numerator and denominator
        dal = db1d3(1)
        dt  = ds*dal
        dal = db1d3(2) + dr*dal
        dbe = db1d3(3) - dt
        dt  = ds*dal
        dal = dbe + dr*dal
        dbe = db1d3(4) - dt  
        zf1d3 = zf1d3/dcmplx(dal*dx+dbe,dal*dy)

cp      evaluate numerator polynomial for nu=-1/3 approximant
        dal = dam1d3(1)
        dt  = ds*dal
        dal = dam1d3(2) + dr*dal
        dbe = dam1d3(3) - dt
        dt  = ds*dal
        dal = dbe + dr*dal  
        dbe = dam1d3(4) - dt
        zfm1d3 = dcmplx(dal*dx+dbe,dal*dy)
cp      evaluate denominator polynomial for nu=-1/3 approximant and
cp        compute ratio of numerator and denominator
        dal = dbm1d3(1)
        dt  = ds*dal
        dal = dbm1d3(2) + dr*dal
        dbe = dbm1d3(3) - dt
        dt  = ds*dal
        dal = dbe + dr*dal
        dbe = dbm1d3(4) - dt
        zfm1d3 = zfm1d3/dcmplx(dal*dx+dbe,dal*dy)

cp      evaluate numerator polynomial for nu=2/3 approximant
        dal = da2d3(1)
        dt  = ds*dal
        dal = da2d3(2) + dr*dal
        dbe = da2d3(3) - dt
        dt  = ds*dal
        dal = dbe + dr*dal  
        dbe = da2d3(4) - dt
        zf2d3 = dcmplx(dal*dx+dbe,dal*dy)
cp      evaluate denominator polynomial for nu=2/3 approximant and
cp        compute ratio of numerator and denominator
        dal = db2d3(1)
        dt  = ds*dal
        dal = db2d3(2) + dr*dal
        dbe = db2d3(3) - dt
        dt  = ds*dal
        dal = dbe + dr*dal  
        dbe = db2d3(4) - dt
        zf2d3 = zf2d3/dcmplx(dal*dx+dbe,dal*dy)

cp      evaluate numerator polynomial for nu=-2/3 approximant
        dal = dam2d3(1)
        dt  = ds*dal
        dal = dam2d3(2) + dr*dal
        dbe = dam2d3(3) - dt  
        dt  = ds*dal
        dal = dbe + dr*dal
        dbe = dam2d3(4) - dt
        zfm2d3 = dcmplx(dal*dx+dbe,dal*dy)
cp      evaluate denominator polynomial for nu=-2/3 approximant and
cp        compute ratio of numerator and denominator
        dal = dbm2d3(1)
        dt  = ds*dal
        dal = dbm2d3(2) + dr*dal
        dbe = dbm2d3(3) - dt
        dt  = ds*dal
        dal = dbe + dr*dal
        dbe = dbm2d3(4) - dt
        zfm2d3 = zfm2d3/dcmplx(dal*dx+dbe,dal*dy)

      end if

      return
      end
      subroutine zasary(z,zai,zaipr)
ci----------------------------------------------------------------------
ci
ci    Purpose
ci      This subroutine evaluates ai(z) and ai'(z) from their asymptotic
ci      expansions for abs(arg(z)) .lt. 2*pi/3.  For speed, the number
ci      of terms needed to achieve about 16 decimals accuracy is tabled
ci      and determined from abs(z).
ci
ci      Note that for the sake of speed and the fact that this subroutine
ci      is to be called by another, checks for valid arguments are not
ci      made.  Hence, an error return is not needed.
ci
ci    Arguments
ci      z      Double precision complex input variable set equal to the
ci             value at which Ai(z) and its derivative are to be eval-
ci             uated.  this subroutine assumes abs(z) .gt. 15 and
ci             abs(arg(z)) .lt. 2*pi/3.
ci      zai    Double precision complex output variable containing the
ci             value computed for Ai(z).
ci      zaipr  Double precision complex output variable containing the
ci             value computed for Ai'(z).
ci
ci    Date of last revision
ci      May 3, 1986
ci
ci    Author
ci      Rick A. Whitaker
ci
ci----------------------------------------------------------------------

c     type declarations for global variables

      include 'utd.i'

      double complex z, zai, zaipr

c     type declarations for local variables

      double complex zout, zoutpr, zpw1d4, zxim, zmone
      double precision     dpmhd2, d2d3, dx, dy, dr, ds,
     1           dal, dbe, dalpr, dbepr, dsdata,
     2           dck(15), ddk(15)
      integer    nterm, ntermx, ndx, k, nterms(5)

c     type declarations for functions used
      double complex dcmplx, zsqrt, zexp
      double precision     dreal, dimag
      integer    int, min0

c     data statements defining constants used herein
      data d2d3  /6.666666666666667d-01/,
     1     dpmhd2/2.820947917738781d-01/,
     2     zmone /(-1.0d+00,0.0d+00)/
      data ntermx/15/,nterms/15,12,11,11,9/

c     coefficients for the expansion
      data dck( 1)/ 0.5989251356587907d+05/,
     1     dck( 2)/ 0.9207206599726415d+04/,
     2     dck( 3)/ 0.1533169432012796d+04/,
     3     dck( 4)/ 0.2784650807776026d+03/,
     4     dck( 5)/ 0.5562278536591708d+02/,
     5     dck( 6)/ 0.1234157333234524d+02/,
     6     dck( 7)/ 0.3079453030173167d+01/,
     7     dck( 8)/ 0.8776669695100169d+00/,
     8     dck( 9)/ 0.2915913992307505d+00/,
     9     dck(10)/ 0.1160990640255154d+00/,
     a     dck(11)/ 0.5764919041266972d-01/,
     b     dck(12)/ 0.3799305912780064d-01/,
     c     dck(13)/ 0.3713348765432099d-01/,
     d     dck(14)/ 0.6944444444444444d-01/,
     e     dck(15)/ 0.1000000000000000d+01/
c     
      data ddk( 1)/-0.6133570666385206d+05/,
     1     ddk( 2)/-0.9446354823095932d+04/,
     2     ddk( 3)/-0.1576357303337100d+04/,
     3     ddk( 4)/-0.2870332371092211d+03/,
     4     ddk( 5)/-0.5750830351391427d+02/,
     5     ddk( 6)/-0.1280729308073563d+02/,
     6     ddk( 7)/-0.3210493584648621d+01/,
     7     ddk( 8)/-0.9204799924129446d+00/,
     8     ddk( 9)/-0.3082537649010791d+00/,
     9     ddk(10)/-0.1241058960272751d+00/,
     a     ddk(11)/-0.6266216349203231d-01/,
     b     ddk(12)/-0.4246283078989483d-01/,
     c     ddk(13)/-0.4388503086419753d-01/,
     d     ddk(14)/-0.9722222222222222d-01/,
     e     ddk(15)/ 0.1000000000000000d+01/

      write(6,*) '> airy_asymp_absphase_ge_pio3:'
      write(6,*) '> > z = ', z

cp    compute -xi and z**(1/4)
      zpw1d4 = zsqrt(z)
      zxim = z*zpw1d4
      zxim = dcmplx(d2d3*dreal(zxim),d2d3*dimag(zxim))
      zpw1d4 = zsqrt(zpw1d4)

cp    compute outer factors in the expansions
      zoutpr = zexp(-zxim)
      zoutpr = dcmplx(dpmhd2*dreal(zoutpr),dpmhd2*dimag(zoutpr))
      zout = zoutpr/zpw1d4
      zoutpr = -zpw1d4*zoutpr

cp    determine number of terms to use
      nterm = nterms(min0(5,(int(zabs(z))-10)/5))
cp    initialize for modified horner's rule evaluation of sums
cp      it is assumed that at least three terms are used
      zxim = zmone/zxim
      dx = dreal(zxim)
      dy = dimag(zxim)
      dr = dx + dx
      ds = dx*dx + dy*dy
      ndx = ntermx - nterm + 1
      dal = dck(ndx)
      dalpr = ddk(ndx)
      ndx = ndx + 1
      dbe = dck(ndx)
      dbepr = ddk(ndx)
      ndx = ndx + 1

cp    loop until components contributing to sums are computed
        do 10 k=ndx,ntermx
        dsdata = ds*dal
        dal = dbe + dr*dal
        dbe = dck(k) - dsdata
        dsdata = ds*dalpr
        dalpr = dbepr + dr*dalpr
        dbepr = ddk(k) - dsdata
   10   continue
cp    repeat

cp    complete evaluation of the airy functions
      zai = zout*dcmplx(dal*dx+dbe,dal*dy)
      zaipr = zoutpr*dcmplx(dalpr*dx+dbepr,dalpr*dy)

      return
      end



      subroutine zasaly(z,zai,zaipr)
ci----------------------------------------------------------------------
ci
ci    Purpose
ci      This subroutine evaluates Ai(z) and Ai'(z) from their asymptotic
ci      expansions for abs(arg(-z)) .lt. pi/3.  For speed, the number
ci      of terms needed to achieve about 16 decimals accuracy is tabled
ci      and determined from abs(z).
ci
ci      Note that for the sake of speed and the fact that this subroutine
ci      is to be called by another, checks for valid arguments are not
ci      made.  Hence, an error return is not needed.
ci
ci    Arguments
ci      z      Double precision complex input variable set equal to the
ci             value at which Ai(z) and its derivative are to be eval-
ci             uated.  This subroutine assumes abs(z) .gt. 15 and
ci             abs(arg(-z)) .lt. pi/3.
ci      zai    Double precision complex output variable containing the
ci             value computed for Ai(z).
ci      zaipr  Double precision complex output variable containing the
ci             value computed for Ai'(z).
ci
ci    Date of last revision
ci      May 4, 1986
ci
ci    Author
ci      Rick A. Whitaker
ci
ci----------------------------------------------------------------------

c     Type declarations for global variables

      include 'utd.i'

      double complex z, zai, zaipr

c     type declarations for local variables
      double complex zwk, zsinxi, zcosxi, zpw1d4, zxi, zone
      double precision     dpimh, d2d3, d9d4, dx, dy, dr,
     1           ds, dals, dalc, dalprs, dalprc, dbes,
     2           dbec, dbeprs, dbeprc, dsdata, dpid4,
     3           dcks(9), dckc(9), ddks(9),
     4           ddkc(9)
      integer    nterm, ntermx, ndx, k, nterms(5)

c     type declarations for functions used
      double complex dcmplx, zsqrt, zsin, zcos
      double precision     dreal, dimag
      integer    int, min0

c     data statements defining constants used herein
      data d2d3  /6.666666666666667d-01/,d9d4/2.25d+00/,
     1     dpimh/5.641895835477563d-01/,
     2     dpid4/7.853981633974483d-01/,
     3     zone /(1.0d+00,0.0d+00)/
      data ntermx/9/,nterms/9,7,6,6,5/

c     coefficients for the expansion
      data dckc( 1)/ 0.2519891987160237d+08/,
     1     dckc( 2)/ 0.4195248751165511d+06/,
     2     dckc( 3)/ 0.9207206599726415d+04/,
     3     dckc( 4)/ 0.2784650807776026d+03/,
     4     dckc( 5)/ 0.1234157333234524d+02/,
     5     dckc( 6)/ 0.8776669695100169d+00/,
     6     dckc( 7)/ 0.1160990640255154d+00/,
     7     dckc( 8)/ 0.3799305912780064d-01/,
     8     dckc( 9)/ 0.6944444444444444d-01/
      data dcks( 1)/ 0.3148257417866826d+07/,
     1     dcks( 2)/ 0.5989251356587907d+05/,
     2     dcks( 3)/ 0.1533169432012796d+04/,
     3     dcks( 4)/ 0.5562278536591708d+02/,
     4     dcks( 5)/ 0.3079453030173167d+01/,
     5     dcks( 6)/ 0.2915913992307505d+00/,
     6     dcks( 7)/ 0.5764919041266972d-01/,
     7     dcks( 8)/ 0.3713348765432099d-01/,
     8     dcks( 9)/ 0.1000000000000000d+01/

      data ddks( 1)/-0.2569790838391133d+08/,
     1     ddks( 2)/-0.4289524004000691d+06/,
     2     ddks( 3)/-0.9446354823095932d+04/,
     3     ddks( 4)/-0.2870332371092211d+03/,
     4     ddks( 5)/-0.1280729308073563d+02/,
     5     ddks( 6)/-0.9204799924129446d+00/,
     6     ddks( 7)/-0.1241058960272751d+00/,
     7     ddks( 8)/-0.4246283078989483d-01/,
     8     ddks( 9)/-0.9722222222222222d-01/
      data ddkc( 1)/-0.3214536521400865d+07/,
     1     ddkc( 2)/-0.6133570666385206d+05/,
     2     ddkc( 3)/-0.1576357303337100d+04/,
     3     ddkc( 4)/-0.5750830351391427d+02/,
     4     ddkc( 5)/-0.3210493584648621d+01/,
     5     ddkc( 6)/-0.3082537649010791d+00/,
     6     ddkc( 7)/-0.6266216349203231d-01/,
     7     ddkc( 8)/-0.4388503086419753d-01/,
     8     ddkc( 9)/ 0.1000000000000000d+01/

      write(6,*) '> airy_asymp_absphase_lt_pio3:'
      write(6,*) '> > z = ', z

cp    set up working value of z
      zwk = -z
cp    compute xi and z**(1/4)
      zpw1d4 = zsqrt(zwk)
      zxi = zwk*zpw1d4
      zxi = dcmplx(d2d3*dreal(zxi),d2d3*dimag(zxi))
      zpw1d4 = zsqrt(zpw1d4)

cp    compute sine and cosine factors in the expansions
      zcosxi = dcmplx(dreal(zxi)+dpid4,dimag(zxi)) 
      zsinxi = zsin(zcosxi)
      zcosxi = zcos(zcosxi)

cp    determine number of terms to use
      nterm = nterms(min0(5,(int(zabs(z))-10)/5))
cp    initialize for modified horner's rule evaluation of sums
cp      it is assumed that at least three terms are used
      zwk = (zone/zwk)**3
      zwk = dcmplx(d9d4*dreal(zwk),d9d4*dimag(zwk))  
      dx = -dreal(zwk)
      dy = -dimag(zwk)
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

cp    loop until components contributing to sums are computed
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
   10   continue
cp    repeat

cp    complete evaluation of the airy functions
      zxi = zone/zxi
      zai = zsinxi*dcmplx(dals*dx+dbes,dals*dy) -
     1      zxi*zcosxi*dcmplx(dalc*dx+dbec,dalc*dy)
      zai = dcmplx(dpimh*dreal(zai),dpimh*dimag(zai))/zpw1d4
      zaipr = zcosxi*dcmplx(dalprc*dx+dbeprc,dalprc*dy) +
     1        zxi*zsinxi*dcmplx(dalprs*dx+dbeprs,dalprs*dy)
      zaipr = dcmplx(-dpimh*dreal(zaipr),-dpimh*dimag(zaipr))*zpw1d4

      return
      end





      subroutine uniform_hankel(zh1,zh2,zh1p,zh2p,z,znu)
cpogo///////////////////////////////////////////////////////////////////
cpogo     Purpose
cpogo       This routine uses the old uniform_hankel subroutine of
cpogo       Whitaker (now called uniform_hankel_olver) to compute the
cpogo       uniform asymptotic approximations of the Hankel functions
cpogo       and their derivatives including a patch for the case when
cpogo       the order equals or nearly equals the argument.  At such 
cpogo       points, Olver's expressions have zero denominators (and
cpogo       numerators) resulting in numerical problems.  This routine
cpogo       averages four surrounding points in the complex order plane
cpogo       to obtain the result in such cases.
cpogo
cpogo       Author:  R. J. Pogorzelski
cpogo
cpogo       Last revision: January 29, 1993
cpogo
cpogo///////////////////////////////////////////////////////////////////


      include 'utd.i'

      double complex dcmplx
      double precision r,ctest
      double complex z,zh1,zh2,zh1p,zh2p,znu,zh11,zh21,zh1p1,zh2p1,znu1,
     &                                   zh12,zh22,zh1p2,zh2p2,znu2,
     &                                   zh13,zh23,zh1p3,zh2p3,znu3,
     &                                   zh14,zh24,zh1p4,zh2p4,znu4

      write(6,*) '> hankel_uniform:'
      write(6,*) '> > nu = ', znu
      write(6,*) '> > z  = ', z

      ctest = zabs(znu)**(1./3.)/5.

      if(zabs(z-znu).gt.ctest) then

         call uniform_hankel_olver(zh1,zh2,zh1p,zh2p,z,znu)

      else
         r = 2.*ctest
         znu1 = znu+dcmplx(r,0.d0)
         znu2 = znu+dcmplx(0.d0,r)
         znu3 = znu-dcmplx(r,0.d0)
         znu4 = znu-dcmplx(0.d0,r)

         call uniform_hankel_olver(zh11,zh21,zh1p1,zh2p1,z,znu1)
         call uniform_hankel_olver(zh12,zh22,zh1p2,zh2p2,z,znu2)
         call uniform_hankel_olver(zh13,zh23,zh1p3,zh2p3,z,znu3)
         call uniform_hankel_olver(zh14,zh24,zh1p4,zh2p4,z,znu4)

         zh1 = 0.25*( zh11+ zh12+ zh13+ zh14)
         zh2 = 0.25*( zh21+ zh22+ zh23+ zh24)
         zh1p= 0.25*(zh1p1+zh1p2+zh1p3+zh1p4)
         zh2p= 0.25*(zh2p1+zh2p2+zh2p3+zh2p4)

      endif

      return
      end

      subroutine uniform_hankel_olver(zh1,zh2,zh1p,zh2p,z,znu)
ci----------------------------------------------------------------------
ci
ci    Purpose
ci      Use the uniform asymptotic expansion to compute approximate
ci      values for the Hankel functions of the first and second kinds
ci      of order znu along with their derivatives.
ci
ci    Arguments
ci      znu    Double precision complex input variable set equal to the
ci             order for which the Hankel functions are to be evaluated.
ci      z      Double precision complex input variable set equal to the
ci             argument at which the Hankel functions are to be
ci             evaluated.
ci      zh1    Double precision complex output variable containing
ci             the computed value for the Hankel function of the first
ci             kind.
ci      zh1p   Double precision complex output variable containing
ci             the computed value for the derivative of the Hankel
ci             function of the first kind.
ci      zh2    Double precision complex output variable containing
ci             the computed value for the Hankel function of the
ci             second kind.
ci      zh2p   Double precision complex output variable containing
ci             the computed value for the derivative of the Hankel
ci             function of the second kind.
ci 
ci    Date of last revision
ci      May 21, 1986
ci
ci    Author
ci      Rick A. Whitaker
ci
C////////////////////////////////////////////////////////////////////////
cpogo   Name changed to uniform_hankel_olver to accommodate interface
cpogo   routine which patches up the numerical failure of Olver's
cpogo   asymptotics when the order equals the argument.  January 29, 1993
C////////////////////////////////////////////////////////////////////////
ci---------------------------------------------------------------------

c     type declarations for global variables

      include 'utd.i'

      double complex znu, z, zh1, zh1p, zh2, zh2p

c     type declarations for local variables
      double complex  zcon1p, zcon1m, zcon2p, zcon2m, zt, ztsq,
     1                z1dnsq, zetm3h, zaip, zo4dp, zaim, zo4dm,
     2                zod2p, zod0dp, zod0dm, ztmp, zhat, znm1d3,
     3                znm2d3, zetrat, zod2m, zwksp(50), r_factor
      double precision  deps, depsai
      integer  nterms, ier, nzwksp, nusw

c     data statements defining constants used herein
c       note that zcon1p = 2*exp(dpi*i/3)
c                 zcon1m = 2*exp(-dpi*i/3)
c                 zcon2p = 4*exp(2*dpi*i/3)
c                 zcon2m = 4*exp(-2*dpi*i/3)
c                 nzwksp .gt. some function of nterms
      data zcon1p/( 1.0d+00, 1.732050807568877d+00)/,
     1     zcon1m/( 1.0d+00,-1.732050807568877d+00)/,
     2     zcon2p/(-2.0d+00, 3.464101615137755d+00)/,
     3     zcon2m/(-2.0d+00,-3.464101615137755d+00)/,
     4     nzwksp/50/

      data deps/1.0d-06/,depsai/1.0d-12/,nterms/4/


      write(6,*) '> hankel_uniform_olver: nu = ', znu, ' z = ', z

c////////////////////////////////////////////////////////////////////////
cpogo
cpogo   Extended to accommodate negative real orders.
cpogo   February 8, 1993    R. J. Pogorzelski
cpogo
c////////////////////////////////////////////////////////////////////////
cpogo
      nusw = 0
      if(dreal(znu).lt.0.)then
         nusw = 1
         znu = -znu
      endif
cpogo
c/////////////////////////////////////////////////////////////////////////
cpogo
cp    Compute outer factors in the uniform asymptotic expansions
cp    for the Hankel functions and their derivatives along with
cp    other important functions of nu and z.
      call uhanko(znu,z,depsai,zhat,z1dnsq,znm1d3,znm2d3,zt,ztsq,
     1            zetm3h,zetrat,zaip,zo4dp,zaim,zo4dm,zod2p,
     2            zod0dp,zod2m,zod0dm,ier)

c     Check for successful completion
      if (ier .eq. 0) then

c       Compute further terms in the expansions in their appropriate linear combinations.

        call uhnksm(zt,ztsq,z1dnsq,zetm3h,zaip,zo4dp,zaim,zo4dm,
     1              zod2p,zod0dp,zod2m,zod0dm,deps,nterms,zwksp,
     2              nzwksp,zh1,zh1p,zh2,zh2p,ier)

c       Assemble approximations.
        ztmp = zetrat*znm1d3
        zh1 = zcon1m*ztmp*zh1
        zh2 = zcon1p*ztmp*zh2
        ztmp = znm2d3/(zhat*zetrat)
        zh1p = zcon2p*ztmp*zh1p
        zh2p = zcon2m*ztmp*zh2p

c       Check for successful completion
        if (ier .ne. 0) then
          write(6,6000) ier,z,znu
 6000     format(//' *** WARNING ***'
     1            /' ier = ',i5,' from summation subroutine uhnksm.'
     2            /' z  = (',d23.16,',',d23.16,')'
     3            /' nu = (',d23.16,',',d23.16,')')
        end if

      else
        write(6,6010) ier,z,znu
 6010   format(//' *** WARNING ***'
     1          /' ier = ',i5,' from outer factors subroutine uhanko.'
     2          /' z  = (',d23.16,',',d23.16,')'
     3          /' nu = (',d23.16,',',d23.16,')')
      end if
cpogo
c/////////////////////////////////////////////////////////////////////////
cpogo
      if(nusw.eq.1)then
         r_factor = zexp((0.,1.)*znu*dpi)
         zh1 = zh1 *r_factor
         zh1p= zh1p*r_factor
         zh2 = zh2 /r_factor
         zh2p= zh2p/r_factor
         znu = -znu
      endif
cpogo
c//////////////////////////////////////////////////////////////////////////
cpogo
      return
      end



      subroutine uhanko(znu,z,deps,zhat,z1dnsq,znm1d3,znm2d3,zt,ztsq,
     1                  zetm3h,zetrat,zaip,zo4dp,zaim,zo4dm,zod2p,
     2                  zod0dp,zod2m,zod0dm,ier)
ci----------------------------------------------------------------------
ci
ci    PURPOSE
ci      Compute outer factors and associated functions of z and nu
ci      appearing in Olver's uniform asymptotic expansions of the
ci      Hankel functions of the first and second kinds and their der-
ci      ivatives.  The various functions of z and nu returned by uhanko
ci      are available for use in computing further terms in the
ci      expansions.
ci
ci----------------------------------------------------------------------

c     type declarations for global variables

      include 'utd.i'

      double complex znu, z, zhat, z1dnsq, znm1d3, znm2d3, znm4d3,
     1           zt, ztsq, zetm3h, zetrat, zaip, zo4dp, zaim,
     2           zo4dm, zod2p, zod0dp, zod2m, zod0dm
      double precision     deps
      integer    ier

c     type declarations for local variables and functions
      double complex znusq, zeta, zetphf, zetmhf, zargp, zargm, zaidp,
     1           zaidm, ze2pd3, zd2pd3
      integer    ier1, ier2
      logical    lzdiv

c     data statements defining constants used

      data ze2pd3 /(-.5d+00, 0.8660254037844386d+00)/,
     1     zd2pd3 /(-.5d+00,-0.8660254037844386d+00)/
cp    initialize completion code
      ier = 0

c     initialize zairy completion codes
      ier1 = 0
      ier2 = 0

      write(6,*) '> hankel_uniform_outer:'
      write(6,*) '> > nu = ', znu
      write(6,*) '> > z  = ', z

cp    if z/nu successfully computed
        if (lzdiv(z,znu,zhat))
     1then
cp      try to compute other nu and z dependent parameters except args 
cp        to airy functions
        call dparms(zhat,znu,zt,ztsq,znusq,z1dnsq,znm1d3,znm2d3,znm4d3,
     1              zeta,zetphf,zetmhf,zetm3h,zetrat,ier)

        write(6,*) '> > t      = ', zt
        write(6,*) '> > tsq    = ', ztsq
        write(6,*) '> > nusq   = ', znusq
        write(6,*) '> > _1dnsq = ', z1dnsq
        write(6,*) '> > nm1d3  = ', znm1d3
        write(6,*) '> > nm2d3  = ', znm2d3
        write(6,*) '> > nm4d3  = ', znm4d3
        write(6,*) '> > zeta   = ', zeta
        write(6,*) '> > etphf  = ', zetphf
        write(6,*) '> > etmhf  = ', zetmhf
        write(6,*) '> > etm3h  = ', zetm3h
        write(6,*) '> > etrat  = ', zetrat
        write(6,*) '> > status = ', ier

cp      if parameters successfully computed
          if (ier .eq. 0)
     1  then
cp        try to compute airy function arguments
          call aryarg(znm2d3,zeta,zargp,zargm,ier)
          write(6,*) '> > nm2d3  = ', znm2d3
          write(6,*) '> > zeta   = ', zeta
          write(6,*) '> > argp   = ', zargp
          write(6,*) '> > argm   = ', zargm
          write(6,*) '> > status = ', ier

cp        if parameters successfully computed
            if (ier .eq. 0)
     1    then
cp          compute airy functions and derivatives             
            call zairy(zargp,deps,zaip,zaidp,ier1)     
            write(6,*) '> > argp    = ', zargp
            write(6,*) '> > aip     = ', zaip
            write(6,*) '> > aipp    = ', zaidp
            write(6,*) '> > statusp = ', ier1
            call zairy(zargm,deps,zaim,zaidm,ier2)
            write(6,*) '> > argm    = ', zargm
            write(6,*) '> > aim     = ', zaim
            write(6,*) '> > aimp    = ', zaidm
            write(6,*) '> > statusm = ', ier2
cp          if airy functions successfully computed
              if (ier1 .eq. 0 .and. ier2 .eq. 0)
     1      then
cp            compute partial outer terms in expansions
              zo4dp = -zetmhf*znm4d3*ze2pd3*zaidp
              zo4dm = -zetmhf*znm4d3*zd2pd3*zaidm
              zod2p = -zetphf*znm2d3*zaip
              zod0dp = ze2pd3*zaidp
              zod2m = -zetphf*znm2d3*zaim
              zod0dm = zd2pd3*zaidm

            else
cp            set completion code for error in evaluation of airy fns
              ier = 134
            end if

          else
cp          set completion code for airy function args not computable
            ier = 133
          end if

        else
cp        set completion code indicating factors not successfully computed
          ier = 135
        end if

      else
cp      set completion code indicating z/nu not successfully computed
        ier = 130
      end if
      return
      end



      subroutine uhnksm(zt,ztsq,z1dnsq,zetm3h,zaip,zo4dp,zaim,
     1                  zo4dm,zod2p,zod0dp,zod2m,zod0dm,deps,
     2                  nterms,zwksp,nzwksp,zh1sm,zh1dsm,zh2sm,
     3                  zh2dsm,ier)
ci------------------------------------------------------------------------
ci
ci    PURPOSE
ci      Compute the sums in appropriate linear combinations appearing in
ci      Olver's uniform asymptotic expansions for the Hankel functions
ci      of the first and second kinds and their derivatives, using up to
ci      nterms (less than 5) to achieve relative error deps.
ci
ci------------------------------------------------------------------------

c     type declarations for global variables

      include 'utd.i'

      double complex zt, ztsq, z1dnsq, zetm3h, zaip, zo4dp, zaim,
     1           zo4dm, zod2p, zod0dp, zod2m, zod0dm, zh1sm, zh1dsm,
     2           zh2sm, zh2dsm, zwksp(50)
      double precision deps
      integer nzwksp, ier, nterms

c     type declarations for local variables and functions
      double complex za1, zb0, zb1, zc0, zc1, zd1, ztpowk,
     1           zatrm, zbtrm, zctrm, zdtrm, zsuma, zsumb, zsumc,
     2           zsumd, ztmpa, ztmpb, ztmpc, ztmpd, zone, z1dif,
     3           z1ddif, z2dif, z2ddif, zh1s, zh1ds, zh2s, zh2ds,
     4           z1dn2k

      double precision   dxtsq, dytsq, dr, dtwo, ds, dxzt3h,
     1                   dyzt3h, dytsq2, dthree, dxzu1, dyzu1, dxzv1, 
     2                   dyzv1, dukta, dvkta, duktb, dvktb, dukpta,
     3                   dvkpta, dukptb, dvkptb, dsdata, dmu(21), 
     4                   dlamda(21), a(66), b(66)

      integer    ndx, ndxp, nduv, ndxend, k, l, i2k,
     1           i2kp1, i2km1, ndxv, ndxvpl, i2kl

      logical    lcvgnc

      double complex dcmplx
      double precision   dabs, dreal, dimag

c     Data statements defining constants used in uhnksm
      data zone/(1.0d+00,0.0d+00)/
      data dtwo,dthree/2.0d+00,3.0d+00/

c     Coefficients for u and v polynomials appearing in olver's
c       uniform asymptotic expansions for the hankel functions
c       and their derivatives

      data a( 1) /  0.1000000000000000d+01/,
     1     a( 2) / -0.2083333333333333d+00/,
     2     a( 3) /  0.1250000000000000d+00/,
     3     a( 4) /  0.3342013888888889d+00/,
     4     a( 5) / -0.4010416666666667d+00/,
     5     a( 6) /  0.7031250000000000d-01/,
     6     a( 7) / -0.1025812596450617d+01/,
     7     a( 8) /  0.1846462673611111d+01/,
     8     a( 9) / -0.8912109136581421d+00/,
     9     a(10) /  0.7324218750000000d-01/
      data a(11) /  0.4669584423426247d+01/,
     1     a(12) / -0.1120700261622299d+02/,
     2     a(13) /  0.8789123535156250d+01/,
     3     a(14) / -0.2364086866378784d+01/,
     4     a(15) /  0.1121520996093750d+00/,
     5     a(16) / -0.2821207255820024d+02/,
     6     a(17) /  0.8463621767460073d+02/,
     7     a(18) / -0.9181824154324002d+02/,
     8     a(19) /  0.4253499984741211d+02/,
     9     a(20) / -0.7368794441223145d+01/
      data a(21) /  0.2271080017089844d+00/,
     1     a(22) /  0.2125701300392171d+03/,
     2     a(23) / -0.7652524681411816d+03/,
     3     a(24) /  0.1059990452528000d+04/,
     4     a(25) / -0.6995796273761325d+03/,
     5     a(26) /  0.2181905059814453d+03/,
     6     a(27) / -0.2649143028259277d+02/,
     7     a(28) /  0.5725014209747314d+00/,
     8     a(29) / -0.1919457662318407d+04/,
     9     a(30) /  0.8061722181737309d+04/
      data a(31) / -0.1358655000643414d+05/,
     1     a(32) /  0.1165539333686453d+05/,
     2     a(33) / -0.5305646972656250d+04/,
     3     a(34) /  0.1200902954101563d+04/,
     4     a(35) / -0.1080909194946289d+03/,
     5     a(36) /  0.1727727532386780d+01/,
     6     a(37) /  0.2020429133096615d+05/,
     7     a(38) / -0.9698059838863751d+05/,
     8     a(39) /  0.1925470012325315d+06/,
     9     a(40) / -0.2034001772804155d+06/
      data a(41) /  0.1222004649830175d+06/,
     1     a(42) / -0.4119265625000000d+05/,
     2     a(43) /  0.7109514160156250d+04/,
     3     a(44) / -0.4939153137207031d+03/,
     4     a(45) /  0.6074041843414307d+01/,
     5     a(46) / -0.2429191879005513d+06/,
     6     a(47) /  0.1311763614662977d+07/,
     7     a(48) / -0.2998015918538107d+07/,
     8     a(49) /  0.3763271297656404d+07/,
     9     a(50) / -0.2813563226586534d+07/
      data a(51) /  0.1268365250000000d+07/,
     1     a(52) / -0.3316451875000000d+06/,
     2     a(53) /  0.4521876953125000d+05/,
     3     a(54) / -0.2499830566406250d+04/,
     4     a(55) /  0.2438052940368652d+02/,
     5     a(56) /  0.3284469853072038d+07/,
     6     a(57) / -0.1970681911843223d+08/,
     7     a(58) /  0.5095260249266464d+08/,
     8     a(59) / -0.7410514821153266d+08/,
     9     a(60) /  0.6634451227472903d+08/
      data a(61) / -0.3756717666076335d+08/,
     1     a(62) /  0.1328876700000000d+08/,
     2     a(63) / -0.2785618250000000d+07/,
     3     a(64) /  0.3081864062500000d+06/,
     4     a(65) / -0.1388608984375000d+05/,
     5     a(66) /  0.1100171432495117d+03/

      data b( 1) /  0.1000000000000000d+01/,
     1     b( 2) /  0.2916666666666667d+00/,
     2     b( 3) / -0.3750000000000000d+00/,
     3     b( 4) / -0.3949652777777778d+00/,
     4     b( 5) /  0.5156250000000000d+00/,
     5     b( 6) / -0.1171875000000000d+00/,
     6     b( 7) /  0.1146496431327160d+01/,
     7     b( 8) / -0.2130533854166667d+01/,
     8     b( 9) /  0.1089257836341858d+01/,
     9     b(10) / -0.1025390625000000d+00/
      data b(11) / -0.5075635242854617d+01/,
     1     b(12) /  0.1238668710214120d+02/,
     2     b(13) / -0.9961006673177083d+01/,
     3     b(14) /  0.2793920993804932d+01/,
     4     b(15) / -0.1441955566406250d+00/,
     5     b(16) /  0.3015773273462785d+02/,
     6     b(17) / -0.9140711508856879d+02/,
     7     b(18) /  0.1005628359759295d+03/,
     8     b(19) / -0.4753911590576172d+02/,
     9     b(20) /  0.8502454757690430d+01/
      data b(21) / -0.2775764465332031d+00/,
     1     b(22) / -0.2247169946128867d+03/,
     2     b(23) /  0.8146235951180321d+03/,
     3     b(24) / -0.1138508263826370d+04/,
     4     b(25) /  0.7604126384523180d+03/,
     5     b(26) / -0.2411579284667969d+03/,
     6     b(27) /  0.3002362060546875d+02/,
     7     b(28) / -0.6765925884246826d+00/,
     8     b(29) /  0.2013089743407110d+04/,
     9     b(30) / -0.8497490948317704d+04/
      data b(31) /  0.1440997727955136d+05/,
     1     b(32) / -0.1245921356699312d+05/,
     2     b(33) /  0.5730098632812500d+04/,
     3     b(34) / -0.1315274658203125d+04/,
     4     b(35) /  0.1208074951171875d+03/,
     5     b(36) / -0.1993531703948975d+01/,
     6     b(37) / -0.2106404840887960d+05/,
     7     b(38) /  0.1014913238950858d+06/,
     8     b(39) / -0.2024212064239434d+06/,
     9     b(40) /  0.2150230445535821d+06/
      data b(41) / -0.1300843659496637d+06/,
     1     b(42) /  0.4424396093750000d+05/,
     2     b(43) / -0.7727732910156250d+04/,
     3     b(44) /  0.5459063720703125d+03/,
     4     b(45) / -0.6883914470672607d+01/,
     5     b(46) /  0.2520859497081193d+06/,
     6     b(47) / -0.1365304986690037d+07/,
     7     b(48) /  0.3131261070473134d+07/,
     8     b(49) / -0.3946845507298180d+07/,
     9     b(50) /  0.2965647725320941d+07/
      data b(51) / -0.1345235875000000d+07/,
     1     b(52) /  0.3545172500000000d+06/,
     2     b(53) / -0.4883626953125000d+05/,
     3     b(54) /  0.2737909667968750d+04/,
     4     b(55) / -0.2724882698059082d+02/,
     5     b(56) / -0.3395807814193124d+07/,
     6     b(57) /  0.2042343072273885d+08/,
     7     b(58) / -0.5295074376688679d+08/,
     8     b(59) /  0.7725855877372554d+08/,
     9     b(60) / -0.6943030354332107d+08/
      data b(61) /  0.3949369854080250d+08/,
     1     b(62) / -0.1404812500000000d+08/,
     2     b(63) /  0.2965335500000000d+07/,
     3     b(64) / -0.3310150312500000d+06/,
     4     b(65) /  0.1509357617187500d+05/,
     5     b(66) / -0.1215978927612305d+03/

c     lambda and mu coefficients appearing in the expansions

      data dlamda(1)/ 0.1041666666666667d+00/,
     1     dlamda(2)/ 0.8355034722222222d-01/,
     2     dlamda(3)/ 0.1282265745563272d+00/,
     3     dlamda(4)/ 0.2918490264641405d+00/,
     4     dlamda(5)/ 0.8816272674437577d+00/,
     5     dlamda(6)/ 0.3321408281862768d+01/,
     6     dlamda(7)/ 0.1499576298686255d+02/,
     7     dlamda(8)/ 0.7892301301158652d+02/,
     8     dlamda(9)/ 0.4744515388682643d+03/,
     9     dlamda(10)/0.3207490090890662d+04/
      data dlamda(11)/0.2408654964087401d+05/,
     1     dlamda(12)/0.1989231191695098d+06/,
     2     dlamda(13)/0.1791902007775344d+07/,
     3     dlamda(14)/0.1748437718003412d+08/,
     4     dlamda(15)/0.1837073796763307d+09/,
     5     dlamda(16)/0.2067904032945155d+10/,
     6     dlamda(17)/0.2482751937593589d+11/,
     7     dlamda(18)/0.3166945498173489d+12/,
     8     dlamda(19)/0.4277112686513472d+13/,
     9     dlamda(20)/0.6097113241139256d+14/
      data dlamda(21)/0.9148694223435640d+15/

      data dmu(1)/ -0.1458333333333333d+00/,
     1     dmu(2)/ -0.9874131944444445d-01/,
     2     dmu(3)/ -0.1433120539158951d+00/,
     3     dmu(4)/ -0.3172272026784136d+00/,
     4     dmu(5)/ -0.9424291479571203d+00/,
     5     dmu(6)/ -0.3511203040826354d+01/,
     6     dmu(7)/ -0.1572726362036805d+02/,
     7     dmu(8)/ -0.8228143909718595d+02/,
     8     dmu(9)/ -0.4923553705236705d+03/,
     9     dmu(10)/-0.3316218568547973d+04/
      data dmu(11)/-0.2482767424520859d+05/,
     1     dmu(12)/-0.2045265873151298d+06/,
     2     dmu(13)/-0.1838444917068210d+07/,
     3     dmu(14)/-0.1790568747352892d+08/,
     4     dmu(15)/-0.1878356353993943d+09/,
     5     dmu(16)/-0.2111438854691369d+10/,
     6     dmu(17)/-0.2531915342298413d+11/,
     7     dmu(18)/-0.3226140741130003d+12/,
     8     dmu(19)/-0.4352813796009286d+13/,
     9     dmu(20)/-0.6199585732586975d+14/
      data dmu(21)/-0.9295073331010611d+15/

      write(6,*) '> hankel_uniform_sum:'
      write(6,*) '> t      = ', zt
      write(6,*) '> tsq    = ', ztsq
      write(6,*) '> z1dnsq = ', z1dnsq
      write(6,*) '> zetm3h = ', zetm3h
      write(6,*) '> aip    = ', zaip
      write(6,*) '> zo4dp  = ', zo4dp
      write(6,*) '> aim    = ', zaim
      write(6,*) '> zo4dm  = ', zo4dm
      write(6,*) '> zod2p  = ', zod2p
      write(6,*) '> zod0dp = ', zod0dp
      write(6,*) '> zod2m  = ', zod2m
      write(6,*) '> zod0dm = ', zod0dm

cp    Initialize completion code
      ier = 0
cp    Initialize for modified horner's rule evaluation of u-sub-k and
cp      v-sub-k polynomials
      dxtsq = dreal(ztsq)
      dytsq = dimag(ztsq)
      dr = dtwo*dxtsq

cp    Compute square of magnituds
      ds = dxtsq*dxtsq + dytsq*dytsq
cp    Compute u-sub-1,2,3 and v-sub-1,2,3 and store for later use
      ndxv = 2*nterms + 1
      write(6,*) '> > nterms = ', nterms
      write(6,*) '> > ndxv   = ', ndxv
      ztpowk = zt
      zwksp(1) = ztpowk*dcmplx(a(2)*dxtsq+a(3),a(2)*dytsq)
      zwksp(ndxv+1) = ztpowk*dcmplx(b(2)*dxtsq+b(3),b(2)*dytsq)
      write(6,*) '> > tk   = ', ztpowk
      write(6,*) '> > u[0] = ', zwksp(1)
      write(6,*) '> > v[0] = ', zwksp(ndxv+1)
      dytsq2 = dytsq*dytsq
      ztpowk = zt*ztpowk
      zwksp(2) = ztpowk*dcmplx((a(4)*dxtsq+a(5))*dxtsq+a(6)-
     1                   a(4)*dytsq2,(dtwo*a(4)*dxtsq+a(5))*dytsq)
      zwksp(ndxv+2) = ztpowk*dcmplx((b(4)*dxtsq+b(5))*dxtsq+b(6)-
     1                      b(4)*dytsq2,(dtwo*b(4)*dxtsq+b(5))*dytsq)
      write(6,*) '> > tk   = ', ztpowk
      write(6,*) '> > u[1] = ', zwksp(2)
      write(6,*) '> > v[1] = ', zwksp(ndxv+2)
      ztpowk = zt*ztpowk
      zwksp(3) = ztpowk*dcmplx(((a(7)*dxtsq+a(8))*dxtsq+a(9))*dxtsq+
     1                   a(10)-(dthree*a(7)*dxtsq+a(8))*dytsq2,
     2                   ((dthree*a(7)*dxtsq+dtwo*a(8))*dxtsq+a(9)-
     3                   a(7)*dytsq2)*dytsq)
      zwksp(ndxv+3) = ztpowk*dcmplx(((b(7)*dxtsq+b(8))*dxtsq+b(9))*
     1                   dxtsq+b(10)-(dthree*b(7)*dxtsq+b(8))*dytsq2,
     2                   ((dthree*b(7)*dxtsq+dtwo*b(8))*dxtsq+b(9)-
     3                   b(7)*dytsq2)*dytsq)
      write(6,*) '> > tk   = ', ztpowk
      write(6,*) '> > u[2] = ', zwksp(3)
      write(6,*) '> > v[2] = ', zwksp(ndxv+3)

cp    Compute a-sub-1, b-sub-0,1, c-sub-0,1, d-sub-1 ... note that
cp      uhnksm exploits that fact that a-sub-0 = d-sub-0 = 1
cp      also, b-sub-k and c-sub-k are computed up to -zeta**(-1/2)
cp      -zeta**(1/2) factors, respectively.  These recurring factors
cp      are included as appropriate in the outer factors, thus saving
cp      repeated multiplications by them.
      dxzu1 = dreal(zwksp(1))
      dyzu1 = dimag(zwksp(1))
      dxzv1 = dreal(zwksp(ndxv+1))
      dyzv1 = dimag(zwksp(ndxv+1))
      dxzt3h = dreal(zetm3h)
      dyzt3h = dimag(zetm3h)
c      write(6,6020) dxzu1,dyzu1,dxzv1,dyzv1,dxzt3h,dyzt3h
c 6020 format(/' dxzu1 = ',d23.16,' dyzu1 = ',d23.16
c     1       /' dxzv1 = ',d23.16,' dyzv1 = ',d23.16
c     2       /' dxzt3h = ',d23.16,' dyzt3h = ',d23.16)
      za1 = (dcmplx(dmu(2)*dxzt3h,dmu(2)*dyzt3h)+dcmplx(dmu(1)*dxzu1,
     1              dmu(1)*dyzu1))*zetm3h + zwksp(2)
      zb0 = zwksp(1) + dcmplx(dlamda(1)*dxzt3h,dlamda(1)*dyzt3h)
      zb1 = zwksp(3) + (dcmplx(dlamda(3)*dxzt3h+dlamda(2)*dxzu1,
     1                         dlamda(3)*dyzt3h+dlamda(2)*dyzu1)*
     2         zetm3h + dcmplx(dlamda(1)*dreal(zwksp(2)),
     3                         dlamda(1)*dimag(zwksp(2))))*zetm3h
      zc0 = zwksp(ndxv+1) + dcmplx(dmu(1)*dxzt3h,dmu(1)*dyzt3h)
      zc1 = zwksp(ndxv+3) + (dcmplx(dmu(3)*dxzt3h+dmu(2)*dxzv1,
     1                              dmu(3)*dyzt3h+dmu(2)*dyzv1)*
     2             zetm3h + dcmplx(dmu(1)*dreal(zwksp(ndxv+2)),
     3                             dmu(1)*dimag(zwksp(ndxv+2))))*
     4             zetm3h
      zd1 = zwksp(ndxv+2) + dcmplx(dlamda(2)*dxzt3h+dlamda(1)*dxzv1,
     1                             dlamda(2)*dyzt3h+dlamda(1)*dyzv1)*
     2                      zetm3h
      write(6,*) '> > a1 = ', za1
      write(6,*) '> > b0 = ', zb0
      write(6,*) '> > b1 = ', zb1
      write(6,*) '> > c0 = ', zc0
      write(6,*) '> > c1 = ', zc1
      write(6,*) '> > d1 = ', zd1

cp    Compute terms
      zatrm = za1*z1dnsq
      zbtrm = zb1*z1dnsq
      zctrm = zc1*z1dnsq
      zdtrm = zd1*z1dnsq
cp    Compute sum of first two terms, thus initializing Kahan
cp      summing scheme
      zsuma = zone + zatrm
      ztmpa = zatrm - (zsuma - zone)
      zsumb = zb0 + zbtrm
      ztmpb = zbtrm - (zsumb - zb0)
      zsumc = zc0 + zctrm
      ztmpc = zctrm - (zsumc - zc0)
      zsumd = zone + zdtrm
      ztmpd = zdtrm - (zsumd - zone)
      write(6,*) '> > asum = ', zsuma
      write(6,*) '> > bsum = ', zsumb
      write(6,*) '> > csum = ', zsumc
      write(6,*) '> > dsum = ', zsumd

cp    Set convergence flag to no convergence indication
      lcvgnc = .false.

cp    Combine sums in form appearing in expansions
      zh1sm = zaip*zsuma + zo4dp*zsumb
      zh2sm = zaim*zsuma + zo4dm*zsumb
      zh1dsm = zod2p*zsumc + zod0dp*zsumd
      zh2dsm = zod2m*zsumc + zod0dm*zsumd
      zh1s = zaip + zo4dp*zb0
      zh2s = zaim + zo4dm*zb0
      zh1ds = zod2p*zc0 + zod0dp
      zh2ds = zod2m*zc0 + zod0dm
      write(6,*) '> > h1sum = ', zh1sm
      write(6,*) '> > h2sum = ', zh2sm
      write(6,*) '> > h1psum = ', zh1dsm
      write(6,*) '> > h2psum = ', zh2dsm
      write(6,*) '> > h1save = ', zh1s
      write(6,*) '> > h2save = ', zh2s
      write(6,*) '> > h1psave = ', zh1ds
      write(6,*) '> > h2psave = ', zh2ds

cp    Prepare to check convergence criteria for terms included thus
cp      far
      z1dif = zh1sm - zh1s
      z2dif = zh2sm - zh2s
      z1ddif = zh1dsm - zh1ds
      z2ddif = zh2dsm - zh2ds

cp    If convergence criteria now satisfied
        if (dabs(dreal(z1dif))+dabs(dimag(z1dif)) .lt.
     1      deps*(dabs(dreal(zh1sm))+dabs(dimag(zh1sm))) .and.
     2      dabs(dreal(z2dif)) + dabs(dimag(z2dif)) .lt.
     3      deps*(dabs(dreal(zh2sm))+dabs(dimag(zh2sm))) .and.
     4      dabs(dreal(z1ddif))+dabs(dimag(z1ddif)) .lt.
     5      deps*(dabs(dreal(zh1dsm))+dabs(dimag(zh1dsm))) .and.
     6      dabs(dreal(z2ddif))+dabs(dimag(z2ddif)) .lt.
     7      deps*(dabs(dreal(zh2dsm))+dabs(dimag(zh2dsm)))) 
     8      lcvgnc = .true.
cp    end if

cp    Save current sums combined as in expansion for next 
cp      convergence test
      zh1s = zh1sm
      zh2s = zh2sm
      zh1ds = zh1dsm
      zh2ds = zh2dsm

cp    Update index into u-sub-k and v-sub-k coefficients
      ndx = 10
      ndxp = 15
cp    Update index into storage for u and v polynomials
      nduv = 4
cp    Update power of nu**(-2)
      z1dn2k = z1dnsq     

cp    Loop until convergence criteria satisfied
cp         or    maximum number of terms reached
        do 30 k=2,nterms 
cp      Initialize for evaluation of two new u and v polynomials
cp        via Horner's rule modified for complex arguments and real
cp        coefficients
        ndxend = ndxp
        ndx = ndx + 1
        dukta = a(ndx)
        dvkta = b(ndx)
        ndx = ndx + 1
        duktb = a(ndx)
        dvktb = b(ndx)
        ndxp = ndxp + 1
        dukpta = a(ndxp)
        dvkpta = b(ndxp)
        ndxp = ndxp + 1
        dukptb = a(ndxp)
        dvkptb = b(ndxp)
        write(6,*) '> > > index = ', ndx
        write(6,*) '> > > indexp = ', ndxp
        write(6,*) '> > > ukta = ', dukta
        write(6,*) '> > > vkta = ', dvkta
        write(6,*) '> > > uktb = ', duktb
        write(6,*) '> > > vktb = ', dvktb
        write(6,*) '> > > ukpta = ', dukpta
        write(6,*) '> > > vkpta = ', dvkpta
        write(6,*) '> > > ukptb = ', dukptb
        write(6,*) '> > > vkptb = ', dvkptb
cp      update indices into coefficients to reflect initialization
        ndx = ndx + 1
        ndxp = ndxp + 1

cp      Loop until quantities to evaluate lowest order u and v 
cp                 polynomials and partial quantities to evaluate
cp                 next highest order polynomials computed
          do 10 l=ndx,ndxend
          dsdata = ds*dukta
          dukta = duktb + dr*dukta
          duktb = a(l) - dsdata
          dsdata = ds*dvkta
          dvkta = dvktb + dr*dvkta
          dvktb = b(l) - dsdata
          dsdata = ds*dukpta
          dukpta = dukptb + dr*dukpta
          dukptb = a(ndxp) - dsdata
          dsdata = ds*dvkpta
          dvkpta = dvkptb + dr*dvkpta
          dvkptb = b(ndxp) - dsdata
          write(6,*) '> > > > index     = ', l
          write(6,*) '> > > > a[index]  = ', a(l)
          write(6,*) '> > > > b[index]  = ', b(l)
          write(6,*) '> > > > indexp    = ', ndxp
          write(6,*) '> > > > a[indexp] = ', a(ndxp)
          write(6,*) '> > > > b[indexp] = ', b(ndxp)
          write(6,*) '> > > > ukta      = ', dukta
          write(6,*) '> > > > vkta      = ', dvkta
          write(6,*) '> > > > uktb      = ', duktb
          write(6,*) '> > > > vktb      = ', dvktb
          write(6,*) '> > > > ukpta     = ', dukpta
          write(6,*) '> > > > vkpta     = ', dvkpta
          write(6,*) '> > > > ukptb     = ', dukptb
          write(6,*) '> > > > vkptb     = ', dvkptb
          ndxp = ndxp + 1
   10     continue
cp      Repeat

cp      One more iteration for highest order polynomials
        dsdata = ds*dukpta
        dukpta = dukptb + dr*dukpta
        dukptb = a(ndxp) - dsdata
        dsdata = ds*dvkpta
        dvkpta = dvkptb + dr*dvkpta
        dvkptb = b(ndxp) - dsdata
	write(6,*) '> > > indexp = ', ndxp
	write(6,*) '> > > ukpta  = ', dukpta
	write(6,*) '> > > ukptb  = ', dukptb
	write(6,*) '> > > vkpta  = ', dvkpta
	write(6,*) '> > > vkptb  = ', dvkptb

cp      Update power appearing outside polynomials
        ztpowk = zt*ztpowk

cp      Post multiply and form new polynomials
        zwksp(nduv) = ztpowk*dcmplx(dukta*dxtsq+duktb,
     1                              dukta*dytsq)
        zwksp(ndxv+nduv) = ztpowk*dcmplx(dvkta*dxtsq+dvktb,
     1                                    dvkta*dytsq)
        write(6,*) '> > > nduv    = ', nduv
        write(6,*) '> > > u[nduv] = ', zwksp(nduv)
        write(6,*) '> > > v[nduv] = ', zwksp(ndxv+nduv)
        ztpowk = zt*ztpowk
        nduv = nduv + 1
        zwksp(nduv) = ztpowk*dcmplx(dukpta*dxtsq+dukptb,
     1                              dukpta*dytsq)
        zwksp(ndxv+nduv) = ztpowk*dcmplx(dvkpta*dxtsq+dvkptb,
     1                                   dvkpta*dytsq)
        write(6,*) '> > > nduv    = ', nduv
        write(6,*) '> > > u[nduv] = ', zwksp(nduv)
        write(6,*) '> > > v[nduv] = ', zwksp(ndxv+nduv)

cp      Update indices in preparation for next iteration
        nduv = nduv + 1
        ndx = ndxp
        i2k = 2*k
        i2km1 = i2k - 1
        i2kp1 = i2k + 1
        ndxp = ndxp + i2kp1 + 2
        write(6,*) '> > > index  = ', ndx
        write(6,*) '> > > indexp = ', ndxp
        write(6,*) '> > > i2k    = ', i2k
        write(6,*) '> > > i2km1  = ', i2km1
        write(6,*) '> > > i2kp1  = ', i2kp1

cp      initialize for evaluation of a, b, c, and d polynomials
cp        via horner's rule
        za1 = dcmplx(dmu(i2k)*dxzt3h+dmu(i2km1)*dxzu1,
     1               dmu(i2k)*dyzt3h+dmu(i2km1)*dyzu1)
        zb1 = dcmplx(dlamda(i2kp1)*dxzt3h+dlamda(i2k)*dxzu1,
     1               dlamda(i2kp1)*dyzt3h+dlamda(i2k)*dyzu1)
        zc1 = dcmplx(dmu(i2kp1)*dxzt3h+dmu(i2k)*dxzv1,
     1               dmu(i2kp1)*dyzt3h+dmu(i2k)*dyzv1)
        zd1 = dcmplx(dlamda(i2k)*dxzt3h+dlamda(i2km1)*dxzv1,
     1               dlamda(i2k)*dyzt3h+dlamda(i2km1)*dyzv1)
        write(6,*) '> > > a1 = ', za1
        write(6,*) '> > > b1 = ', zb1
        write(6,*) '> > > c1 = ', zc1
        write(6,*) '> > > d1 = ', zd1

cp      loop until partial a, b, c, and d evaluations done via 
cp                 horner's rule
          do 20 l=2,i2km1
          write(6,*) '> > > > l    = ', l
          ndxvpl = ndxv + l
          i2kl = i2k - l
          write(6,*) '> > > > i2kl = ', i2kl
          za1 = za1*zetm3h + dcmplx(dmu(i2kl)*dreal(zwksp(l)),
     1                              dmu(i2kl)*dimag(zwksp(l)))
          zd1 = zd1*zetm3h + 
     1                dcmplx(dlamda(i2kl)*dreal(zwksp(ndxvpl)),
     2                       dlamda(i2kl)*dimag(zwksp(ndxvpl)))
          i2kl = i2kp1 - l
          write(6,*) '> > > > i2kl = ', i2kl
          zb1 = zb1*zetm3h + dcmplx(dlamda(i2kl)*dreal(zwksp(l)),
     1                              dlamda(i2kl)*dimag(zwksp(l)))
          zc1 = zc1*zetm3h + dcmplx(dmu(i2kl)*dreal(zwksp(ndxvpl)),
     2                              dmu(i2kl)*dimag(zwksp(ndxvpl)))
          write(6,*) '> > > > a1   = ', za1
          write(6,*) '> > > > b1   = ', zb1
          write(6,*) '> > > > c1   = ', zc1
          write(6,*) '> > > > d1   = ', zd1
   20     continue
cp      repeat

cp      complete the evaluations
        za1 = za1*zetm3h + zwksp(i2k)
        zd1 = zd1*zetm3h + zwksp(ndxv+i2k)
        zb1 = zetm3h*(zb1*zetm3h +
     1         dcmplx(dlamda(1)*dreal(zwksp(i2k)),
     2                dlamda(1)*dimag(zwksp(i2k)))) + zwksp(i2kp1)
        zc1 = zetm3h*(zc1*zetm3h +
     1         dcmplx(dmu(1)*dreal(zwksp(ndxv+i2k)),
     2                dmu(1)*dimag(zwksp(ndxv+i2k)))) + 
     3         zwksp(ndxv+i2kp1)
        write(6,*) '> > > i2k   = ', i2k
        write(6,*) '> > > i2kp1 = ', i2kp1
        write(6,*) '> > > a1    = ', za1
        write(6,*) '> > > b1    = ', zb1
        write(6,*) '> > > c1    = ', zc1
        write(6,*) '> > > d1    = ', zd1

cp      Evaluate new terms for sums
        z1dn2k = z1dnsq*z1dn2k
        zatrm = za1*z1dn2k + ztmpa
        zbtrm = zb1*z1dn2k + ztmpb
        zctrm = zc1*z1dn2k + ztmpc
        zdtrm = zd1*z1dn2k + ztmpd
        write(6,*) '> > > aterm = ', zatrm
        write(6,*) '> > > bterm = ', zbtrm
        write(6,*) '> > > cterm = ', zctrm
        write(6,*) '> > > dterm = ', zdtrm

cp      Update sums via Kahan summing scheme
        ztmpa = zsuma
        zsuma = zsuma + zatrm
        ztmpa = zatrm - (zsuma - ztmpa)
        ztmpb = zsumb
        zsumb = zsumb + zbtrm
        ztmpb = zbtrm - (zsumb - ztmpb)
        ztmpc = zsumc
        zsumc = zsumc + zctrm
        ztmpc = zctrm - (zsumc - ztmpc)
        ztmpd = zsumd
        zsumd = zsumd + zdtrm
        ztmpd = zdtrm - (zsumd - ztmpd)
        write(6,*) '> > > asum = ', zsuma
        write(6,*) '> > > bsum = ', zsumb
        write(6,*) '> > > csum = ', zsumc
        write(6,*) '> > > dsum = ', zsumd

cp      Combine sume in form appearing in expansions
        zh1sm = zaip*zsuma + zo4dp*zsumb
        zh2sm = zaim*zsuma + zo4dm*zsumb
        zh1dsm = zod2p*zsumc + zod0dp*zsumd
        zh2dsm = zod2m*zsumc + zod0dm*zsumd
        write(6,*) '> > > h1sum   = ', zh1sm
        write(6,*) '> > > h2sum   = ', zh2sm
        write(6,*) '> > > h1psum  = ', zh1dsm
        write(6,*) '> > > h2psum  = ', zh2dsm
        write(6,*) '> > > h1save  = ', zh1s
        write(6,*) '> > > h2save  = ', zh2s
        write(6,*) '> > > h1psave = ', zh1ds
        write(6,*) '> > > h2psave = ', zh2ds

cp      Prepare for convergence tests
        z1dif = zh1sm - zh1s
        z2dif = zh2sm - zh2s
        z1ddif = zh1dsm - zh1ds
        z2ddif = zh2dsm - zh2ds
c       write(6,6090) z1dif,z2dif,z1ddif,z2ddif
c 6090 format(/' z1dif = (',d23.16,',',d23.16,')'
c     1       /' z2dif = (',d23.16,',',d23.16,')'
c     2       /' z1ddif = (',d23.16,',',d23.16,')'
c     3       /' z2ddif = (',d23.16,',',d23.16,')') 

cp      If convergence criteria met this term
          if (dabs(dreal(z1dif))+dabs(dimag(z1dif)) .lt.
     1        deps*(dabs(dreal(zh1sm))+dabs(dimag(zh1sm))) .and.
     2        dabs(dreal(z2dif)) + dabs(dimag(z2dif)) .lt.
     3        deps*(dabs(dreal(zh2sm))+dabs(dimag(zh2sm))) .and.
     4        dabs(dreal(z1ddif))+dabs(dimag(z1ddif)) .lt.
     5        deps*(dabs(dreal(zh1dsm))+dabs(dimag(zh1dsm))) .and.
     6        dabs(dreal(z2ddif))+dabs(dimag(z2ddif)) .lt.
     7        deps*(dabs(dreal(zh2dsm))+dabs(dimag(zh2dsm)))) 
     8  then
cp        Relative convergence criteria met, see if it was before
          if (lcvgnc)
     1    then
cp          Lonvergence - rel. error criteria met twice in a row
            go to 40
          else
cp          Set rel. error criteria met flag
            lcvgnc = .true.
          end if

        else
cp        Reset rel. error criteria flag
          lcvgnc = .false.
        end if

cp      Save combined sums for comparison next iteration
        zh1s = zh1sm
        zh2s = zh2sm
        zh1ds = zh1dsm
        zh2ds = zh2dsm
   30   continue
cp    repeat
cp    . All allowable terms used - set completion code
        ier = 177
cp    . Relative error criteria met twice in a row
   40   continue
cp    Resume
      return
      end



      subroutine dparms(zhat,znu,zt,ztsq,znusq,z1dnsq,znm1d3,znm2d3,
     1                  znm4d3,zeta,zetphf,zetmhf,zetm3h,zetrat,ier)
ci----------------------------------------------------------------------
ci
ci    PURPOSE
ci      Compute parameters depending on z and nu that appear in the
ci      uniform asymptotic expansions of the Hankel functions and
ci      their derivatives, except the arguments to the Airy functions.
ci
ci----------------------------------------------------------------------

c     type declarations for global variables

      include 'utd.i'

      double complex zhat, znu, zt, ztsq, znusq, z1dnsq, znm1d3,
     1           znm2d3, znm4d3, zeta, zetphf, zetmhf, zetm3h, zetrat
      integer    ier

c     type declarations for local variables and functions
      double complex ztemp, zone, zxi, zlnxi, zlnzet 
      double precision   dx, dy, dxabs, dyabs, done, dinf,
     1                   dinfsr, du, dv, dtemp, dhalf, dtwo,
     2                   dzero, d2pi, d2d3, dlncon, dsqr2, dxir,
     3                   dxii, dtemp2, d1d3, d1d4, d4d3
      double complex dcmplx, zsqrt, zlog, zexp 
      double precision   dreal, dimag, dabs  

c     data statements defining constants used in this subroutine
c     note that dinf and dinfsr are machine floating-point dependent
c     constants equal to the largest available floating-point number and
c     its square root, respectively.

      data dinf,dinfsr/1.0d+38,1.0d+19/

      data dzero,d1d4,d1d3/0.0d+00,0.25d+00,3.333333333333333d-01/,
     1     dhalf,d2d3,done/0.5d+00,6.666666666666667d-01,1.0d+00/,
     2     dtwo,d2pi/2.0d+00,6.283185307179586d+00/,
     3     dlncon/0.2703100720721096d+00/,
     4     dsqr2/1.4142135623730950d+00/,
     5     d4d3/1.333333333333333d+00/

      data zone/(1.0d+00,0.0d+00)/

cp    separate real and imaginary parts of zhat
      dx = dreal(zhat)
      dy = dimag(zhat)
      dxabs = dabs(dx)
      dyabs = dabs(dy)

cp    if 1-zhat**2 can be computed without overflow
        if (dxabs .le. dinfsr .and. dyabs .le. (dinfsr-done))
     1  then
cp        find max and min of abs(dx) and abs(dy)
          du = dxabs
          dv = dyabs
          if (du .lt. dv)
     1    then
            dtemp = du
            du = dv
            dv = dtemp
          end if
          if (du .ge. dhalf)
     1    then
            if (dv .gt. dinf/(dtwo*du)) go to 10         
          end if
        else
          go to 10
        end if

cp    then
cp      compute 1-zhat**2 and related constants
        ztemp = dcmplx(done-(dx-dy)*(dx+dy),-dtwo*dx*dy)
        ztemp = zsqrt(ztemp)
        dtemp = zabs(ztemp)
        dtemp = dsqrt(dreal(ztemp)*dreal(ztemp)+
     &          dimag(ztemp)*dimag(ztemp))
        zt = dcmplx(dreal(ztemp)/dtemp/dtemp,
     1                -dimag(ztemp)/dtemp/dtemp)
        ztsq = zt*zt
c        ztsq = dcmplx(dreal(ztemp)/dtemp/dtemp,
c     1                -dimag(ztemp)/dtemp/dtemp)
c        zt = zsqrt(ztsq)
        go to 20

cp    else
   10   continue
cp      set completion code - unable to compute 1-zhat**2 and exit
        ier = 131
        return
   20   continue
cp    end if

cp    if nu**2 can be computed without overflow
        if (zabs(znu) .le. dinfsr)
     1then
cp      compute znu**2
        znusq = znu*znu
        dtemp = zabs(znusq)
        z1dnsq = dcmplx(dreal(znusq)/dtemp/dtemp,
     1                  -dimag(znusq)/dtemp/dtemp)
cp      compute nu**(-2/3), nu**(-4/3), nu**(-1/3)
        znm4d3 = -zlog(znu)
        dtemp = dreal(znm4d3)
        dtemp2 = dimag(znm4d3)
        znm1d3 = zexp(dcmplx(d1d3*dtemp,d1d3*dtemp2))
        znm2d3 = zexp(dcmplx(d2d3*dtemp,d2d3*dtemp2))
        znm4d3 = zexp(dcmplx(d4d3*dtemp,d4d3*dtemp2))
      else
cp      set completion code - unable to compute nu**2
        ier = 132
        return
      end if

cp    compute xi = ln(1+(1-zhat**2)**(1/2)) -ln(z) - (1-zhat**2)**(1/2)
cp    using default branch of logarithm and square root
c      ztemp = zsqrt(ztemp)
      zxi = zlog(zone+ztemp) - zlog(zhat) - ztemp
      dxir = dreal(zxi)
      dxii = dimag(zxi)
      dtemp = zabs(zxi)
      zetm3h = dcmplx(d2d3*dxir/dtemp/dtemp,-d2d3*dxii/dtemp/dtemp)        

cp    compute principal value of ln(xi) and then adjust imaginary part
      zlnxi = zlog(zxi)
c      write(6,6000) zlnxi,zxi
c 6000 format(' zlnxi = (',d23.16,',',d23.16,')'
c     1       /'  zxi = (',d23.16,',',d23.16,')')

cp    prepare to adjust logarithm of xi to appropriate riemann sheet
      dtemp = dzero

cp    find adjustment necessary to get on proper riemann sheet

cp    if zhat is real
        if (dy .eq. dzero)
     1then
cp      if zhat is greater than one
          if (dx .gt. done) dtemp = d2pi
cp      end if
      else
cp      zhat is not real
cp      if zhat is in upper half-plane
          if (dy .gt. dzero)
     1  then
cp        if xi lies in upper half-plane
            if (dimag(zxi) .gt. dzero) dtemp = -d2pi
cp        end if
        else
cp        if xi lies in lower half-plane
            if (dimag(zxi) .lt. dzero) dtemp = d2pi
cp        end if
        end if
      end if
      write(6,*) '> > > dtemp = ', dtemp

cp    if logarithm of xi must be adjusted
        if (dtemp .ne. dzero) zlnxi =
     1      dcmplx(dreal(zlnxi),dimag(zlnxi)+dtemp)
cp    end if    
      write(6,*) '> > > zlnxi = ', zlnxi

cp    compute ln(zeta), zeta, zeta**(1/2), zeta**(-1/2)
      zlnzet = dcmplx(d2d3*dreal(zlnxi)+dlncon,
     1                d2d3*dimag(zlnxi))
      zeta = zexp(zlnzet)
      zetphf = zsqrt(zeta)
      dtemp = zabs(zetphf)
      zetmhf = dcmplx(dreal(zetphf)/dtemp/dtemp,
     1               -dimag(zetphf)/dtemp/dtemp)
c 
cp    compute (4*zeta/(1-zhat**2))**(1/4)
      ztemp = zlog(ztemp)
      zetrat = zexp(dcmplx(d1d4*dreal(zlnzet)-dhalf*dreal(ztemp),
     1                      d1d4*dimag(zlnzet)-dhalf*dimag(ztemp)))
      zetrat = dcmplx(dsqr2*dreal(zetrat),dsqr2*dimag(zetrat))
      return
      end



      subroutine aryarg(znm2d3,zeta,zargp,zargm,ier)
ci----------------------------------------------------------------------
ci
ci    Purpose
ci      Compute the arguments for the Airy function evaluations
ci      carefully to prevent premature overflow.  Note that the
ci      major work here is in lzdiv.  A faster, but less safe
ci      implementation can be obtained without use of lzdiv.
ci
ci    Arguments
ci      znm2d3 DOUBLE PRECISION COMPLEX INPUT VARIABLE SET EQUAL
ci             TO NU**(-2/3).  IN OUR IMPLEMENTATION, ZMN2D3 IS
ci             OUTPUT FROM DPARMS.
ci      zeta   Double precision complex input variable set equal
ci             to zeta in the uniform asymptotic expansions.  In
ci             our implementation, zeta is output from dparms.
ci      zargp  Double precision complex output variable set equal
ci             to exp(2*pi*i/3)*nu(2/3)*zeta.
ci      zargm  Double precision complex output variable set equal
ci             to exp(-2*pi*i/3)*nu(2/3)*zeta.
ci      ier    Integer output variable set equal to a completion code.
ci             ier = 0 indicates normal completion.  ier=133 indicates
ci             failure in computing nu(2/3)*zeta from the input
ci             zmn2d3 and zeta.
ci
ci----------------------------------------------------------------------

c     Type declarations for global variables
      double complex  znm2d3, zeta, zargp, zargm
      integer  ier

c     Type declarations for local variables
      double complex zexpp, zexpm

c     Type declarations for functions
      logical lzdiv

c     Data statements defining constants used herein
c       Observe that zexpp and zexpm are exp(2*dpi*i/3) and its 
c       reciprocal, respectively.
      data zexpp/(-5.0d-01,8.660254037844386d-01)/,
     1     zexpm/(-5.0d-01,-8.660254037844386d-01)/
c 
cp    Initialize completion code
      ier = 0

cp    if nu**(2/3)*zeta successfully computed
        if (lzdiv(zeta,znm2d3,zargm))
     1then
        write(6,*) '> > > airy_arg';
        write(6,*) '> > > > nm2d3 = ', znm2d3
        write(6,*) '> > > > zeta  = ', zeta
        write(6,*) '> > > > argm  = ', zargm
cp      compute zargp and zargm
        zargp = zexpp*zargm
        zargm = zexpm*zargm
        write(6,*) '> > > > argp = ', zargp
        write(6,*) '> > > > argm = ', zargm

      else
cp      set completion code
        ier = 133
      end if

      return
      end



      logical function lzdiv(z1,z2,z1dz2)
ci----------------------------------------------------------------------
ci
ci    Purpose
ci      Carefully compute z1/z2 avoiding overflow and destructive
ci      underflow.  If the quotient is successfully computed, then the
ci      logical value .true. is returned and the quotient is returned
ci      in zdw.  Otherwise, .false. is returned and the quotient is not.
ci
ci-----------------------------------------------------------------------

c     type declarations for global variables
      double complex z1,z2,z1dz2

c     type declarations for local variables and functions
      double precision dxhinf, dz1r, dz1i, dz2r, dz2i, dz1b,
     1                 dz2b, dz2ub, dtemp, dqr, dqi, denom,
     2                 dscale, done
      double precision dabs, dimag, dmax1, dreal

c     data statements defining constants used by lzdiv
c     note that dxhinf is a machine floating-point dependent constant
c     set equal to half the largest available floating-point number.
      data done,dxhinf/1.0d+00,0.5d+38/

cp    separate real and imaginary parts of arguments
      dz1r = dreal(z1)
      dz1i = dimag(z1)
      dz2r = dreal(z2)
      dz2i = dimag(z2)

cp    set up largest and smallest magnitudes needed
      dz1b = dmax1(dabs(dz1r),dabs(dz1i))
      dz2b = dabs(dz2r)
      dz2ub = dabs(dz2i)

      if (dz2b .lt. dz2ub)
     1then
        dtemp = dz2b
        dz2b = dz2ub
        dz2ub = dtemp
      end if

cp    If overflow will occur, then abort
        if (dz2b .lt. done) 
     1  then
          if (dz1b .gt. dz2b*dxhinf)
     1    then
            lzdiv = .false.
            return
          end if
        end if

cp    else
cp      Compute the quotient
        dz1r = dz1r/dz1b
        dz1i = dz1i/dz1b
        dz2r = dz2r/dz2b
        dz2i = dz2i/dz2b
        denom = done + (dz2ub/dz2b)**2
        dscale = dz1b/dz2b/denom
        dqr = (dz1r*dz2r + dz1i*dz2i)*dscale
        dqi = (dz2r*dz1i - dz1r*dz2i)*dscale 
        z1dz2 = dcmplx(dqr,dqi)
        lzdiv = .true.

cp    end if
      return
      end
