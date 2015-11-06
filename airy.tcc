#ifndef AIRY_TCC
#define AIRY_TCC 1


#include "complex_util.h"


template<typename _Tp>
  void
  __airy(const std::complex<_Tp>& __z, _Tp __eps,
         std::complex<_Tp>& __ai,
         std::complex<_Tp>& __aip,
         int& __error);

template<typename _Tp>
  void
  __airy_bessel_i(const std::complex<_Tp>& __z, _Tp __eps,
		  std::complex<_Tp>& __ip1d3,
		  std::complex<_Tp>& __im1d3,
		  std::complex<_Tp>& __ip2d3,
		  std::complex<_Tp>& __im2d3);

template<typename _Tp>
  void
  __airy_bessel_k(const std::complex<_Tp>& __z, _Tp __eps,
		  std::complex<_Tp>& __kp1d3,
		  std::complex<_Tp>& __kp2d3,
		  int& __error);

template<typename _Tp>
  void
  __airy_hyperg_rational(const std::complex<_Tp>& __z,
			 std::complex<_Tp>& __fp1d3,
			 std::complex<_Tp>& __fm1d3,
			 std::complex<_Tp>& __fp2d3,
			 std::complex<_Tp>& __fm2d3);

template<typename _Tp>
  void
  __airy_asymp_absarg_ge_pio3(std::complex<_Tp> __z,
			      std::complex<_Tp>& __ai,
			      std::complex<_Tp>& __aip);

template<typename _Tp>
  void
  __airy_asymp_absarg_lt_pio3(std::complex<_Tp> __z,
			      std::complex<_Tp>& __ai,
			      std::complex<_Tp>& __aip);

/**
    @brief
    This subroutine computes the Airy function Ai(z) and its first
    derivative in the complex z-plane.

    The algorithm used exploits numerous representations of the
    Airy function and its derivative.
    The representations are recorded here for reference:

		  sqrt(z)
    (1) Ai(z)	= ------- (I_(-1/3)(xi) - I_(1/3)(xi))
		     3

		  sqrt(z/3)
    (2) Ai(z)	= --------- K_(1/3)(xi)
		     pi

		    2/3  -5/6
		   2	3
		=  --------  z exp(-xi) U(5/6; 5/3; 2 xi)
		   sqrt(pi)

		  sqrt(z)
    (3) Ai(-z)  = ------- (J_(-1/3)(xi) + J_(1/3)(xi))
		     3

		  z
    (4) Ai'(z)  = - (I_(2/3)(xi) - I_(-2/3)(xi))
		  3

			 z
    (5) Ai'(z)  = - ---------- K_(2/3)(xi)
		    pi sqrt(3)

		      2/3  -7/6
		     4    3    2
		=  - -------- Z  exp(-xi) U(7/6; 7/3; 2 xi)
		     sqrt(pi)

		   z
    (6) Ai'(-z) =  - (J_(2/3)(xi) - J_(-2/3)(xi)) ,
		   3

	       2  3/2
    Where xi = - z    and U(a;b;z) is the confluent hypergeometric
	       3
    function as defined in

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
      difference equations, NBS J. Res., Series B, VOL 71B,
      pp 111-129, 1967.

    @see Olver, F. W. J. and Sookne, D. J., Note on backward recurrence
      algorithms, Math. Comp. Vol 26, No. 120, pp 941-947,
      OCT. 1972

    @see Sookne, D. J., Bessel Functions I and J of Complex Argument and
      Integer Order, NBS J. Res., Series B, Vol 77B, Nos 3& 4,
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
 
    @param[in] z   The argument at which the Airy function and its derivative
		   are to be computed.
    @param[in] eps Relative error required.  Currently, eps is used only
    		   in the backward recursion algorithms.
    @param[out] ai  The value computed for Ai(z).
    @param[out] aip The value computed for Ai'(z).
 */
template<typename _Tp>
  void
  __airy(const std::complex<_Tp>& __z, _Tp __eps,
         std::complex<_Tp>& __ai,
         std::complex<_Tp>& __aip,
         int& __error)
  {
    using __cmplx = std::complex<_Tp>;

    static constexpr std::complex<_Tp>
      _S_eppid6{ 0.8660254037844386,  0.5},
      _S_empid6{ 0.8660254037844386, -0.5},
      _S_eppid3{ 0.5,  0.8660254037844386},
      _S_empid3{ 0.5, -0.8660254037844386},
      _S_j{0, 1};
    static constexpr _Tp
      _S_1d3  {3.33333333333333333e-01},
      _S_2d3  {6.66666666666666667e-01},
      _S_gm1d3{2.588194037928068e-01},
      _S_gm2d3{3.550280538878172e-01},
      _S_2g2d3{1.775140269439086e-01},
      _S_rsqpi{2.820947917738781e-01},
      pi    {3.1415926535897932385};
    static constexpr _Tp _S_small{0.25}, _S_big{15};

    __error = 0;

    std::cout << " > airy:\n";
    std::cout << " > > z = " << __z << '\n';
    std::cout << " > > small = " << _S_small << '\n';
    std::cout << " > > big   = " << _S_big << '\n';

    //  Compute modulus of z for later use
    auto __absz = std::abs(__z);
    std::cout << " > > absz = " << __absz << '\n';
    //  Check size of abs(z) and select appropriate methods
    if (__absz < _S_big)
      {
	//  Moderate or small abs(z)
	//  Check argument for right or left half plane
	if (std::real(__z) >= _Tp(0))
	  {
	    //  Argument in closed right half plane
	    //  Compute xi as defined in the representations in terms of Bessel functions
	    auto __sqrtz = std::sqrt(__z);
	    auto __xi = _S_2d3 * __z * __sqrtz;

	    //  Check for abs(z) too large for accuracy of representations (1) and (4)
	    if (__absz >= _Tp{2})
	      {
		//  Use rational approximation for modified Bessel functions of orders 1/3 and 2/3
		__airy_bessel_k(__xi, __eps, __ai, __aip, __error);
		//  Recover Ai(z) and Ai'(z)
		auto __p1d4c = std::sqrt(__sqrtz);
		__xi = _S_rsqpi * std::exp(-__xi);
		__ai *= __xi / __p1d4c;
		__aip *= -__xi * __p1d4c;
	      }
	    else
	      {
		//  Check for abs(z) small enough for rational approximation
		if (__absz <= _S_small)
		  {
		    //  Use rational approximation along with (1) and (4)
		    __cmplx __ip1d3, __im1d3, __ip2d3, __im2d3;
		    __airy_hyperg_rational(__z, __ip1d3, __im1d3, __ip2d3, __im2d3);
		    //  Recover Ai(z) and Ai'(z)
		    __im1d3 *= _S_gm2d3;
		    __ip1d3 *= _S_gm1d3;
		    __ai = __im1d3 - __z * __ip1d3;
		    __im2d3 *= _S_gm1d3;
		    __ip2d3 *= _S_2g2d3;
		    __aip = __z * __z * __ip2d3 - __im2d3;
		  }
		else
		  {
		    //  Use backward recurrence along with (1) and (4)
		    __cmplx __ip1d3, __im1d3, __ip2d3, __im2d3;
		    __airy_bessel_i(__xi, __eps, __ip1d3, __im1d3, __ip2d3, __im2d3);
		    //  Recover Ai(z) and Ai'(z)
		    __ai = _S_1d3 * __sqrtz * (__im1d3 - __ip1d3);
		    __aip = _S_1d3 * __z * (__ip2d3 - __im2d3);
                    std::cout << " > > > > > > ai = " << __ai << '\n';
                    std::cout << " > > > > > > aip = " << __aip << '\n';
		  }
	      }
	  }
	else
	  {
	    //  Argument lies in left half plane
	    //  Compute xi as defined in the representations in terms of Bessel functions
	    auto __sqrtz = std::sqrt(-__z);
	    auto __xi = -_S_2d3 * __z * __sqrtz;
	    //  Set up arguments to recover Bessel functions of the first kind in (3) and (6)
	    __cmplx __z2xi, __p1d3f, __m1d3f, __p2d3f, __m2d3f;
	    if (std::imag(__xi) >= _Tp(0))
	      {
		//  Argument lies in upper half plane, so use appropriate identity
		__z2xi = -_S_j * __xi;
		__p1d3f = _S_eppid6;
		__m1d3f = _S_empid6;
		__p2d3f = _S_eppid3;
		__m2d3f = _S_empid3;
	      }
	    else
	      {
		//  Argument lies in lower half plane, so use appropriate identity
		__z2xi = _S_j * __xi;
		__p1d3f = _S_empid6;
		__m1d3f = _S_eppid6;
		__p2d3f = _S_empid3;
		__m2d3f = _S_eppid3;
	      }

	    //  Use approximation depending on size of z
	    if (__absz <= _S_small)
	      {
		//  Use rational approximation
		__xi = -__z;
		__cmplx __ip1d3, __im1d3, __ip2d3, __im2d3;
		__airy_hyperg_rational(__z, __ip1d3, __im1d3, __ip2d3, __im2d3);
		//  Recover Ai(z) and Ai'(z)
		__im1d3 *= _S_gm2d3;
		__ip1d3 *= _S_gm1d3;
		__ai = __im1d3 - __z * __ip1d3;
		__im2d3 *= _S_gm1d3;
		__ip2d3 *= _S_2g2d3;
		__aip = __z * __z * __ip2d3 - __im2d3;
	      }
	    else
	      {
		//  Use backward recurrence
		__cmplx __ip1d3, __im1d3, __ip2d3, __im2d3;
		__airy_bessel_i(__z2xi, __eps, __ip1d3, __im1d3, __ip2d3, __im2d3);
		//  Recover Ai(z) and Ai'(z)
		__ai = _S_1d3 * __sqrtz * (__m1d3f * __im1d3 + __p1d3f * __ip1d3);
		__aip = _S_1d3 * __z * (__m2d3f * __im2d3 - __p2d3f * __ip2d3);
                std::cout << " > > > > > ai = " << __ai << '\n';
                std::cout << " > > > > > aip = " << __aip << '\n';
	      }
	  }
      }
    else
      {  //  abs(z) is large...
	//  Check arg(z) to see which asymptotic form is appropriate
	if (std::abs(std::arg(__z)) < 2 * pi / 3)
	  __airy_asymp_absarg_ge_pio3(__z, __ai, __aip);
	else
	  __airy_asymp_absarg_lt_pio3(__z, __ai, __aip);
      }
    return;
}


/**
    Compute the modified Bessel functions of the first kind of
    orders +-1/3 and +-2/3 needed to compute the Airy functions
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
      (z/2)  e                   inf  (k+nu)*Gamma(2 nu+k)
      -----------  = I_nu(z) + 2 SUM  -------------------- I_(nu+k)(z) .
      Gamma(nu+1)                k=1   k! Gamma(1 + 2 nu)   

    This modification of the algorithm is given in part in

    Olver, F. W. J. and Sookne, D. J., Note on Backward Recurrence
      Algorithms, Math. of Comp., Vol. 26, no. 120, Oct. 1972.

    And further elaborated for the Bessel functions in
  
    Sookne, D. J., Bessel Functions I and J of Complex Argument 
      and Integer Order, J. Res. NBS - Series B, Vol 77B, Nos.
      3& 4, July-December, 1973.

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
    a status return is not needed.

    @param[in]  z     The argument at which the modified Bessel
		      functions computed by this program are to be evaluated.
    @param[in]  eps   The maximum relative error required in the results.
    @param[out] ip1d3 The value of I_(+1/3)(z).
    @param[out] im1d3 The value of I_(-1/3)(z).
    @param[out] ip2d3 The value of I_(+2/3)(z).
    @param[out] im2d3 The value of I_(-2/3)(z).
 */
template<typename _Tp>
  void
  __airy_bessel_i(const std::complex<_Tp>& __z, _Tp __eps,
		  std::complex<_Tp>& __ip1d3,
		  std::complex<_Tp>& __im1d3,
		  std::complex<_Tp>& __ip2d3,
		  std::complex<_Tp>& __im2d3)
  {
    using __cmplx = std::complex<_Tp>;

    constexpr __cmplx _S_zero{0}, _S_zone{1};
    constexpr _Tp
    	 _S_1d3  {3.333333333333333e-01}, _S_2d3  {6.666666666666667e-01},
    	 _S_4d3  {1.333333333333333e+00}, _S_5d3  {1.666666666666667e+00},
    	 _S_8d3  {2.666666666666667e+00}, _S_10d3 {3.333333333333333e+00},
    	 _S_14d3 {4.666666666666667e+00}, _S_16d3 {5.333333333333333e+00},
    	 _S_gm4d3{8.929795115692492e-01}, _S_gm5d3{9.027452929509336e-01},
    	 _S_2sqrt2{2.828427124746190e+01};

    std::cout << " > airy_bessel_i:\n";
    std::cout << " > > z = " << __z << '\n';

    //  Compute 1/z for use in recurrence for speed and abs(z)
    __cmplx __1dz;
    __safe_div(1, __z, __1dz);

    //  Initialize for forward recursion based on order 2/3
    int __n = 0;
    auto __d2n = _Tp(2 * __n) + _S_4d3;
    auto __plast2 = _S_zone;
    auto __p2 = __d2n * __1dz;

    //  Calculate weak convergence test and set flag for weak convergence loop
    auto __test = _S_2sqrt2 / __eps;
    bool __converged = false;

    //  loop until weak and strong convergence tests satisfied when recurring forward
    while (true)
      {
	do
	  {
    	    //  Update n dependent quantities
    	    ++__n;
    	    __d2n += 2;
    	    //  Interchange values
    	    auto __pold2 = __plast2;
    	    __plast2 = __p2;
    	    //  Recur forward one step
    	    __p2 = __1dz * __d2n * __plast2 + __pold2;
	  }
	while (__norm_L1(__p2) < __test);  //  Check if convergence test (in 1-norm) satisfied

	//  If strong convergence, then weak and strong convergence
	if (__converged)
    	  break;

	//  Calculate strong convergence test.
	//  See the Olver and Sookne papers cited for details.
	auto __rep2 = std::real(__p2);
	auto __imp2 = std::imag(__p2);
	auto __replast2 = std::real(__plast2);
	auto __implast2 = std::imag(__plast2);
	//  Compute scale factor to avoid possible overflow
	auto __lamn = __norm_Linf(__p2);
	//  Compute the kappa_n of strong convergence lemma
	auto __kapn = std::sqrt(((__rep2 / __lamn) * (__rep2 / __lamn)
    			     + (__imp2 / __lamn) * (__imp2 / __lamn))
    			   / ((__replast2 / __lamn) * (__replast2 / __lamn)
    			    + (__implast2 / __lamn) * (__implast2 / __lamn)));
	//  Compute quantity needed for lambda_n of strong convergence lemma
	__lamn = _Tp(__n + 1) / std::abs(__z);
	//  Determine appropriate value for rho_n of lemma
	if (__kapn + 1 / __kapn > 2 * __lamn)
    	  __kapn = __lamn + std::sqrt(__lamn * __lamn - 1);
	//  Compute test value - sqrt(2) multiple already included
	__test *= std::sqrt(__kapn - 1 / __kapn);
	//  Set strong convergence test flag
	__converged = true;
      }

    //  Prepare for backward recurrence for both orders 1/3 and 2/3
    auto __rn = _Tp(__n);
    ++__n;
    __d2n = _Tp(2 * __n);
    auto __plast1 = _S_zero;
    __plast2 = _S_zero;
    //  Carefully compute 1/p2 to avoid overflow in complex divide
    __cmplx __p1;
    __safe_div(1, __p2, __p1);
    __p2 = __p1;
    //  Set up n dependent parameters used in normalization sum
    auto __rnpn1 = __rn + _S_1d3;
    auto __rnpn2 = __rn + _S_2d3;
    auto __rnp2n1 = (__rn - _Tp(1)) + _S_2d3;
    auto __rnp2n2 = (__rn - _Tp(1)) + _S_4d3;
    //  Initialize normalization sum
    auto __fact1 = __rnpn1 * __rnp2n1 / __rn;
    auto __sum1 = __fact1 * __p1;
    auto __fact2 = __rnpn2 * __rnp2n2 / __rn;
    auto __sum2 = __fact2 * __p2;
    //  Set ending loop index to correspond to k=1 term of the
    //  normalization relationship
    auto __nend = __n - 3;

    //  if backward recurrence loop will be nontrivial
    if (__nend > 0)
      {
	//  Loop until backward recursion to k=1 term of normalization
	for (int __l = 1; __l <= __nend; ++__l)
	  {
    	    //  Update n dependent quantities
    	    --__n;
    	    __d2n -= 2;
    	    __fact1 = __d2n + _S_2d3;
    	    __fact2 = __d2n + _S_4d3;
    	    //  Interchanges for order 1/3 recurrence
    	    auto __pold1 = __plast1;
    	    __plast1 = __p1;
    	    //  Recur back one step for order 1/3
    	    __p1 = __1dz * __fact1 * __plast1 + __pold1;
    	    //  Interchanges for order 2/3 recurrence
    	    auto __pold2 = __plast2;
    	    __plast2 = __p2;
    	    //  Recur back one step for order 2/3
    	    __p2 = __1dz * __fact2 * __plast2 + __pold2;
    	    //  Update quantities for computing normalization sums
    	    __rn -= _Tp(1);
    	    __rnpn1 = __rn + _S_1d3;
    	    __rnp2n1 = __rn - _S_1d3;
    	    __rnpn2 = __rn + _S_2d3;
    	    __rnp2n2 = __rnpn1;
    	    __fact1 = __rnp2n1 / __rn;
    	    __fact2 = __rnp2n2 / __rn;
    	    //  Update normalization sums
    	    __sum1 += __rnpn1 * __p1;
    	    __sum1 *= __fact1;
    	    __sum2 += __rnpn2 * __p2;
    	    __sum2 *= __fact2;
	  }
      }
    std::cout << " > > sum1 = " << __sum1 << '\n';
    std::cout << " > > sum2 = " << __sum2 << '\n';

    //  Perform last two recurrence steps for order 1/3
    auto __pold1 = __plast1;
    __plast1 = __p1;
    __p1 = _S_14d3 * __plast1 * __1dz + __pold1;
    __sum1 += _S_4d3 * __p1;
    __pold1 = __plast1;
    __plast1 = __p1;
    __p1 = _S_8d3 * __plast1 * __1dz + __pold1;
    __sum1 = _Tp(2) * __sum1 + __p1;
    std::cout << " > > sum1 = " << __sum1 << '\n';

    //  Compute scale factor and scale results for order 1/3 case
    auto __zd2pow = std::pow(_Tp(0.5L) * __z, -_S_1d3);
    __pold1 = __zd2pow * std::exp(-__z);
    __sum1 *= _S_gm4d3 * __pold1;
    __plast1 /= __sum1;
    __ip1d3 = __p1 / __sum1;
    std::cout << " > > pold1 = " << __pold1 << '\n';
    std::cout << " > > zd2pow = " << __zd2pow << '\n';
    std::cout << " > > sum1 = " << __sum1 << '\n';

    //  Perform last two recurrence steps for order 2/3
    auto __pold2 = __plast2;
    __plast2 = __p2;
    __p2 = _S_16d3 * __plast2 * __1dz + __pold2;
    __sum2 += _S_5d3 * __p2;
    __pold2 = __plast2;
    __plast2 = __p2;
    __p2 = _S_10d3 * __plast2 * __1dz + __pold2;
    __sum2 = _Tp(2) * __sum2 + __p2;
    std::cout << " > > sum2 = " << __sum2 << '\n';

    //  Compute scale factor and scale results for order 2/3 case
    __sum2 *= _S_gm5d3 * __zd2pow * __pold1;
    __plast2 /= __sum2;
    __ip2d3 = __p2 / __sum2;
    std::cout << " > > pold1 = " << __pold1 << '\n';
    std::cout << " > > zd2pow = " << __zd2pow << '\n';
    std::cout << " > > sum2 = " << __sum2 << '\n';

    //  Recur back one step from order +1/3 to get order -2/3
    __im2d3 = _S_2d3 * __ip1d3 * __1dz + __plast1;

    //  Recur back one step from order +2/3 to get order -1/3
    __im1d3 = _S_4d3 * __ip2d3 * __1dz + __plast2;

    return;
  }


/**
    @brief Compute approximations to the modified Bessel functions of the
    second kind of orders 1/3 and 2/3 for moderate arguments.
    More specifically, the program computes

    E_nu(z) = exp(z) sqrt(2 z/pi) K_nu(z), for nu = 1/3 and nu = 2/3.

    This subprogram uses a rational approximation given in

    Luke, Y. L., Mathematical functions and their approximations,
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

    @param[in] z   The value for which the quantity E_nu is to be computed.
		   it is recommended that abs(z) not be too small and that
		   abs(arg(z)) <= 3*pi/4.
    @param[in] eps The maximum relative error allowable in the computed
		   results. The relative error test is based on the comparison
		   of successive iterates.
    @param[out] kp1d3  The value computed for E_(+1/3)(z).
    @param[out] kp2d3  The value computed for E_(+2/3)(z).
    @param[out] status A completion code.
		       status = 0 indicates normal completion
		       status = 129, convergence failed in 100 iterations

    @note According to published information about the behaviour of the error for orders
	  1/3 and 2/3, status = 129 should never occur for the domain of z that we recommend.
	  indeed, in the worst case, say, z=2 and arg(z) = 3*pi/4, we expect 20 iterations
	  to give 7 or 8 decimals of accuracy.
 */
template<typename _Tp>
  void
  __airy_bessel_k(const std::complex<_Tp>& __z, _Tp __eps,
		  std::complex<_Tp>& __kp1d3,
		  std::complex<_Tp>& __kp2d3,
		  int& __status)
  {
    using __cmplx = std::complex<_Tp>;

    constexpr _Tp _S_an1i{ 4.8555555555555555555e+01L},
		  _S_an2i{ 4.7222222222222222222e+01L},
		  _S_p12i{ 3.1444444444444444444e+01L},
		  _S_p22i{ 3.2777777777777777777e+01L},
		  _S_p13i{-9.2592592592592592592e-01L},
		  _S_p23i{ 1.2962962962962962962e+00L},
		  _S_p11i{-7.9074074074074074074e+01L},
		  _S_p21i{-8.1296296296296296296e+01L};

    constexpr std::complex<_Tp> _S_zone{1};

    constexpr _Tp
    _S_f[8]
    { 144, 77, 62208, 95472, 17017, 65, 90288, 13585 };

    constexpr _Tp
    _S_phi[6]
    { 67, 91152, 12697, 79, 96336, 19633 };

    std::cout << " > airy_bessel_k: z = " << __z << '\n';

    __status = 0;

    //  Initialize polynomials for recurrence
    auto __f10 = _S_zone;
    auto __f20 = _S_zone;
    auto __f11 = _S_zone + _S_f[0] * __z / _S_f[1];
    auto __f12 = __z * (_S_f[2] * __z + _S_f[3]);
    __f12 = _S_zone + __f12 / _S_f[4];
    auto __f21 = _S_zone + _S_f[0] * __z / _S_f[5];
    auto __f22 = __z * (_S_f[2] * __z + _S_f[6]);
    __f22 = _S_zone + __f22 / _S_f[7];
    std::cout << " > > f10 = " << __f10 << '\n';
    std::cout << " > > f20 = " << __f20 << '\n';
    std::cout << " > > f11 = " << __f11 << '\n';
    std::cout << " > > f12 = " << __f12 << '\n';
    std::cout << " > > f21 = " << __f21 << '\n';
    std::cout << " > > f22 = " << __f22 << '\n';

    auto __phi10 = _S_zone;
    auto __phi20 = _S_zone;
    auto __phi11 = __cmplx((_S_f[0] * std::real(__z) + _S_phi[0]) / _S_f[1],
		       std::imag(__f11));
    auto __phi12 = __z * (_S_f[2] * __z + _S_phi[1]);
    __phi12 = (__phi12 + _S_phi[2]) / _S_f[4];
    auto __phi21 = __cmplx((_S_f[0] * std::real(__z) + _S_phi[3]) / _S_f[5],
		       std::imag(__f21));
    auto __phi22 = __z * (_S_f[2] * __z + _S_phi[4]);
    __phi22 = (__phi22 + _S_phi[5]) / _S_f[7];
    std::cout << " > > phi10 = " << __phi10 << '\n';
    std::cout << " > > phi20 = " << __phi20 << '\n';
    std::cout << " > > phi11 = " << __phi11 << '\n';
    std::cout << " > > phi12 = " << __phi12 << '\n';
    std::cout << " > > phi21 = " << __phi21 << '\n';
    std::cout << " > > phi22 = " << __phi22 << '\n';

    //  Initialize for recursion
    auto __ratold = __phi22 / __f22;
    auto __rat1 = __phi12 / __f12;
    auto __delt = _Tp(32);
    auto __an1 = _S_an1i;
    auto __an2 = _S_an2i;
    auto __p11 = _S_p11i;
    auto __p12 = _S_p12i;
    auto __p13 = _S_p13i;
    auto __p21 = _S_p21i;
    auto __p22 = _S_p22i;
    auto __p23 = _S_p23i;
    auto __eta = _Tp(24);
    auto __gamm = _Tp(3);
    auto __gam = _Tp(5);
    auto __q = _Tp(16) * __gam;

    //  Loop until maximum iterations used or convergence
    for (int __i = 1; __i < 100; ++__i)
      {
	//  Evaluate next term in recurrence for order 1/3 polynomials
	auto __qz = __q * __z;
	auto __fact1 = __qz - __p11;
	auto __fact2 = __qz - __p12;
	auto __f13 = __fact1 * __f12 + __fact2 * __f11 - __p13 * __f10;
	__f13 /= __an1;
	auto __phi13 = __fact1 * __phi12 + __fact2 * __phi11 - __p13 * __phi10;
	__phi13 /= __an1;

	//  Evaluate next term in recurrence for order 2/3 polynomials
	__fact1 = __qz - __p21;
	__fact2 = __qz - __p22;
	auto __f23 = __fact1 * __f22 + __fact2 * __f21 - __p23 * __f20;
	__f23 /= __an2;
	auto __phi23 = __fact1 * __phi22 + __fact2 * __phi21 - __p23 * __phi20;
	__phi23 /= __an2;

	//  check for convergence
	auto __ratnew = __phi23 / __f23;
	__rat1 = __phi13 / __f13;

	if (std::abs(__ratnew - __ratold) < __eps * std::abs(__ratnew))
	  {
	    //  Convergence.
	    __kp2d3 = __ratnew;
	    __kp1d3 = __phi13 / __f13;
	    return;
	  }

	//  Prepare for next iteration
	__ratold = __ratnew;
	__f20 = __f21;
	__f21 = __f22;
	__f22 = __f23;
	__phi20 = __phi21;
	__phi21 = __phi22;
	__phi22 = __phi23;
	__f10 = __f11;
	__f11 = __f12;
	__f12 = __f13;
	__phi10 = __phi11;
	__phi11 = __phi12;
	__phi12 = __phi13;
	__delt = __delt + _Tp(24);
	__p12 = __p12 + __delt;
	__p22 = __p22 + __delt;
	__eta += _Tp(8);
	__an1 += __eta;
	__an2 += __eta;
	auto __anm1 = __an1 - __delt - _Tp(16);
	auto __anm2 = __an2 - __delt - _Tp(16);
	__gamm = __gam;
	__gam += _Tp(2);
	__p23 = -__gam / __gamm;
	__p13 = __p23 * __anm1;
	__p23 = __p23 * __anm2;
	__p11 = -__an1 - __p12 - __p13;
	__p21 = -__an2 - __p22 - __p23;
	__q = _Tp(16) * __gam;
      }

    //  Maximum iterations exceeded
    __status = 129;

    return;
  }



/**
    @brief This subroutine computes rational approximations
    to the hypergeometric functions related to the modified Bessel
    functions of orders \nu = +-1/3 and \nu +- 2/3.  That is,
    A(z)/B(z), Where A(z) and B(z) are cubic polynomials with
    real coefficients, approximates

      \Gamma(\nu+1)
      ------------- I_\nu(z) = 0_F_1 (;\nu+1;z^2/4) ,
       (z/2)^nu 	

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
    a status return is not needed.

    @param[in]  z	The argument at which the hypergeometric given
			above is to be evaluated.  Since the approximation
			is of fixed order, abs(z) must be small to ensure
			sufficient accuracy of the computed results.
    @param[out]  fp1d3	The approximate value of the Gauss hypergeometric
			function related to the modified Bessel function
			of order +1/3.
    @param[out]  fm1d3	The approximate value of the Gauss hypergeometric
			function related to the modified Bessel function
			of order -1/3.
    @param[out]  fp2d3	The approximate value of the Gauss hypergeometric
			function related to the modified Bessel function
			of order +2/3.
    @param[out]  fm2d3	The approximate value of the Gauss hypergeometric
			function related to the modified Bessel function
			of order -2/3.
 */
template<typename _Tp>
  void
  __airy_hyperg_rational(const std::complex<_Tp>& __z,
			 std::complex<_Tp>& __fp1d3,
			 std::complex<_Tp>& __fm1d3,
			 std::complex<_Tp>& __fp2d3,
			 std::complex<_Tp>& __fm2d3)
  {
    using __cmplx = std::complex<_Tp>;

    constexpr __cmplx _S_zone{1};

    constexpr _Tp _S_ap1d3[4]{   81, 32400,  2585520,  37920960 };
    constexpr _Tp _S_bp1d3[4]{  -35,  5040,  -574560,  37920960 };
    constexpr _Tp _S_am1d3[4]{   81, 22680,  1156680,   7711200 };
    constexpr _Tp _S_bm1d3[4]{  -10,  1260,  -128520,   7711200 };
    constexpr _Tp _S_ap2d3[4]{  162, 75735,  7270560, 139352400 };
    constexpr _Tp _S_bp2d3[4]{ -110, 16830, -2019600, 139352400 };
    constexpr _Tp _S_am2d3[4]{  162, 36855,  1415232,   4481568 };
    constexpr _Tp _S_bm2d3[4]{   -7,   819,   -78624,   4481568 };

    std::cout << " > airy_hyperg_rational:\n";
    std::cout << " > > z = " << __z << '\n';

    //  Check to see if z^3 will underflow and act accordingly
    auto __zzz = __z * __z * __z;

    if (std::abs(__zzz) < _Tp(10) * std::numeric_limits<_Tp>::min())
      {
	__fp1d3  = _S_zone;
	__fm1d3 = _S_zone;
	__fp2d3  = _S_zone;
	__fm2d3 = _S_zone;
      }
    else
      {
	auto __r = 2 * std::real(__zzz);
	auto __s = std::norm(__zzz);

	//  All of the following polynomial evaluations are done using
	//  a modified of Horner's rule which exploits the fact that
	//  the polynomial coefficients are all real.  The algorithm is
	//  discussed in detail in:
	//  Knuth, D. E., The Art of Computer Programming: Seminumerical
	//  Algorithms (Vol. 2) Third Ed., Addison-Wesley, pp 486-488, 1998.

	//  If n is the degree of the polynomial, n - 3 multiplies are
	//  saved and 4 * n - 6 additions are saved.
	auto __horner
	{
	  [__r, __s, __zzz](const auto (&_S_c)[4])
	  {
	    auto __aa = _S_c[0];
	    auto __t  = __s * __aa;
	    __aa = _S_c[1] + __r * __aa;
	    auto __bb = _S_c[2] - __t;
	    __t  = __s * __aa;
	    __aa = __bb + __r * __aa;
	    __bb = _S_c[3] - __t;
	    return __aa * __zzz + __bb;
	  }
	};

	//  Evaluate numerator polynomial for nu=+1/3 approximant
/*
	auto al = _S_ap1d3[0];
	auto t  = s * al;
	al = _S_ap1d3[1] + r * al;
	auto be = _S_ap1d3[2] - t;
	t  = s * al;
	al = be + r * al;
	be = _S_ap1d3[3] - t;
	fp1d3 = al * zzz + be;
	//  Evaluate denominator polynomial for nu=+1/3 approximant
	//  and compute ratio of numerator and denominator
	al = _S_bp1d3[0];
	t  = s * al;
	al = _S_bp1d3[1] + r * al;
	be = _S_bp1d3[2] - t;
	t  = s * al;
	al = be + r * al;
	be = _S_bp1d3[3] - t;
	fp1d3 /= al * zzz + be;
*/
	__fp1d3 = __horner(_S_ap1d3) / __horner(_S_bp1d3);

	//  Evaluate numerator polynomial for nu=-1/3 approximant
/*
	al = _S_am1d3[0];
	t  = s * al;
	al = _S_am1d3[1] + r * al;
	be = _S_am1d3[2] - t;
	t  = s * al;
	al = be + r * al;
	be = _S_am1d3[3] - t;
	fm1d3 = al * zzz + be;
	//  Evaluate denominator polynomial for nu=-1/3 approximant
	//  and compute ratio of numerator and denominator
	al = _S_bm1d3[0];
	t  = s * al;
	al = _S_bm1d3[1] + r * al;
	be = _S_bm1d3[2] - t;
	t  = s * al;
	al = be + r * al;
	be = _S_bm1d3[3] - t;
	fm1d3 /= al * zzz + be;
*/
	__fm1d3 = __horner(_S_am1d3) / __horner(_S_bm1d3);

	//  Evaluate numerator polynomial for nu=+2/3 approximant
/*
	al = _S_ap2d3[0];
	t  = s * al;
	al = _S_ap2d3[1] + r * al;
	be = _S_ap2d3[2] - t;
	t  = s * al;
	al = be + r * al;
	be = _S_ap2d3[3] - t;
	fp2d3 = al * zzz + be;
	//  Evaluate denominator polynomial for nu=+2/3 approximant
	//  and compute ratio of numerator and denominator
	al = _S_bp2d3[0];
	t  = s * al;
	al = _S_bp2d3[1] + r * al;
	be = _S_bp2d3[2] - t;
	t  = s * al;
	al = be + r * al;
	be = _S_bp2d3[3] - t;
	fp2d3 /= al * zzz + be;
*/
	__fp2d3 = __horner(_S_ap2d3) / __horner(_S_bp2d3);

	//  Evaluate numerator polynomial for nu=-2/3 approximant
/*
	al = _S_am2d3[0];
	t  = s * al;
	al = _S_am2d3[1] + r * al;
	be = _S_am2d3[2] - t;
	t  = s * al;
	al = be + r * al;
	be = _S_am2d3[3] - t;
	fm2d3 = al * zzz + be;
	//  Evaluate denominator polynomial for nu=-2/3 approximant
	//  and compute ratio of numerator and denominator
	al = _S_bm2d3[0];
	t  = s * al;
	al = _S_bm2d3[1] + r * al;
	be = _S_bm2d3[2] - t;
	t  = s * al;
	al = be + r * al;
	be = _S_bm2d3[3] - t;
	fm2d3 /= al * zzz + be;
*/
	__fm2d3 = __horner(_S_am2d3) / __horner(_S_bm2d3);
      }

    return;
  }


/**
    @brief
    This subroutine evaluates Ai(z) and Ai'(z) from their asymptotic
    expansions for abs(arg(z)) < 2*pi/3.  For speed, the number
    of terms needed to achieve about 16 decimals accuracy is tabled
    and determined from abs(z).

    Note that for the sake of speed and the fact that this subroutine
    is to be called by another, checks for valid arguments are not
    made.  Hence, a status return is not needed.

    @param[in]  z Complex input variable set equal to the value at which
		  Ai(z) and its derivative are to be evaluated.
		  This subroutine assumes abs(z) > 15
		  and abs(arg(z)) < 2*pi/3.
    @param[out] ai  The value computed for Ai(z).
    @param[out] aip The value computed for Ai'(z).
 */
template<typename _Tp>
  void
  __airy_asymp_absarg_ge_pio3(std::complex<_Tp> __z,
			      std::complex<_Tp>& __ai,
			      std::complex<_Tp>& __aip)
  {
    constexpr _Tp _S_2d3   = 6.666666666666667e-01;
    constexpr _Tp _S_pmhd2 = 2.820947917738781e-01;
    constexpr int _S_ncoeffs = 15;
    constexpr int _S_numnterms = 5;
    constexpr int _S_nterms[5]{ _S_ncoeffs, 12, 11, 11, 9 };

    //  Coefficients for the expansion
    constexpr _Tp
    _S_ck[_S_ncoeffs]
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
    _S_dk[_S_ncoeffs]
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

    std::cout << " > airy_asymp_absarg_ge_pio3:\n";
    std::cout << " > > __z = " << __z << '\n';

    //  Compute -xi and z^(1/4)
    auto __pw1d4 = std::sqrt(__z);
    auto __xim = __z * __pw1d4;
    __xim *= _S_2d3;
    __pw1d4 = std::sqrt(__pw1d4);

    //  Compute outer factors in the expansions
    auto __zoutpr = std::exp(-__xim);
    __zoutpr *= _S_pmhd2;
    auto __zout = __zoutpr / __pw1d4;
    __zoutpr *= -__pw1d4;

    //  Determine number of terms to use
    auto __nterm = _S_nterms[std::min(_S_numnterms - 1, (int(std::abs(__z)) - 10) / 5)];
    //  Initialize for modified Horner's rule evaluation of sums.
    //  It is assumed that at least three terms are used.
    __xim = -_Tp(1) / __xim;
    auto __r = 2 * std::real(__xim);
    auto __s = std::norm(__xim);
    auto __index = _S_ncoeffs - __nterm;// + 1;
    auto __al = _S_ck[__index];
    auto __alpr = _S_dk[__index];
    ++__index;
    auto __be = _S_ck[__index];
    auto __bepr = _S_dk[__index];
    ++__index;

    for (int __k = __index; __k < _S_ncoeffs; ++__k)
      {
	auto __term = __s * __al;
	__al = __be + __r * __al;
	__be = _S_ck[__k] - __term;
	__term = __s * __alpr;
	__alpr = __bepr + __r * __alpr;
	__bepr = _S_dk[__k] - __term;
      }

    __ai = __zout * __al * __xim + __be;
    __aip = __zoutpr * __alpr * __xim + __bepr;

    return;
  }



/**
    @brief This subroutine evaluates Ai(z) and Ai'(z) from their asymptotic
    expansions for abs(arg(-z)) < pi/3.  For speed, the number
    of terms needed to achieve about 16 decimals accuracy is tabled
    and determined from abs(z).

    Note that for the sake of speed and the fact that this subroutine
    is to be called by another, checks for valid arguments are not
    made.  Hence, an error return is not needed.

    @param[in] z  The value at which the Airy function and its derivative
	          are to be evaluated.
		  This subroutine assumes abs(z) > 15 and abs(arg(-z)) < pi/3.
    @param[out] ai  The computed value of the Airy function Ai(z).
    @param[out] aip The computed value of the Airy function derivative Ai'(z).
 */
template<typename _Tp>
  void
  __airy_asymp_absarg_lt_pio3(std::complex<_Tp> __z,
			      std::complex<_Tp>& __ai,
			      std::complex<_Tp>& __aip)
  {
    constexpr _Tp _S_2d3 {6.666666666666667e-01};
    constexpr _Tp _S_9d4 {2.25e+00};
    constexpr _Tp _S_pimh{5.641895835477563e-01};
    constexpr _Tp _S_pid4{7.853981633974483e-01};
    constexpr std::complex<_Tp> __zone{1};
    constexpr int _S_ncoeffs = 9;
    constexpr int _S_numnterms = 5;
    constexpr int _S_nterms[_S_numnterms]{ _S_ncoeffs, 7, 6, 6, 5 };

    //  coefficients for the expansion
    constexpr _Tp
    _S_ckc[_S_ncoeffs]
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
    _S_cks[_S_ncoeffs]
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
    _S_dks[_S_ncoeffs]
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
    _S_dkc[_S_ncoeffs]
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

    std::cout << " > airy_asymp_absarg_lt_pio3:\n";
    std::cout << " > > z = " << __z << '\n';

    //  Set up working value of z
    __z = -__z;
    //  Compute xi and z^(1/4)
    auto __pw1d4 = std::sqrt(__z);
    auto __xi = __z * __pw1d4;
    __xi *= _S_2d3;
    __pw1d4 = std::sqrt(__pw1d4);

    //  Compute sine and cosine factors in the expansions.
    auto __xiarg = __xi + _S_pid4;
    auto __sinxi = std::sin(__xiarg);
    auto __cosxi = std::cos(__xiarg);

    //  Determine number of terms to use.
    auto __nterm = _S_nterms[std::min(_S_numnterms - 1, (int(std::abs(__z)) - 10) / 5)];
    //  Initialize for modified Horner's rule evaluation of sums
    //  it is assumed that at least three terms are used
    __z = std::pow(__zone / __z, _Tp(3));
    __z *= _S_9d4;
    auto __r = -2 * std::real(__z);
    auto __s = std::norm(__z);
    auto __index = _S_ncoeffs - __nterm;

    auto __als = _S_cks[__index];
    auto __alc = _S_ckc[__index];
    auto __alprs = _S_dks[__index];
    auto __alprc = _S_dkc[__index];
    ++__index;

    auto __bes = _S_cks[__index];
    auto __bec = _S_ckc[__index];
    auto __beprs = _S_dks[__index];
    auto __beprc = _S_dkc[__index];
    ++__index;

    //  Loop until components contributing to sums are computed
    for (int __k = __index; __k < _S_ncoeffs; ++__k)
      {
	auto __term = __s * __als;
	__als = __bes + __r * __als;
	__bes = _S_cks[__k] - __term;
	__term = __s * __alc;
	__alc = __bec + __r * __alc;
	__bec = _S_ckc[__k] - __term;
	__term = __s * __alprs;
	__alprs = __beprs + __r * __alprs;
	__beprs = _S_dks[__k] - __term;
	__term = __s * __alprc;
	__alprc = __beprc + __r * __alprc;
	__beprc = _S_dkc[__k] - __term;
      }

    //  Complete evaluation of the Airy functions
    __xi = __zone / __xi;
    __ai = __sinxi * __als * __z + __bes
         - __xi * __cosxi * __alc * __z + __bec;
    __ai *= _S_pimh / __pw1d4;
    __aip = __cosxi * __alprc * __z + __beprc
	  + __xi * __sinxi * __alprs * __z + __beprs;
    __aip *= -_S_pimh * __pw1d4;

    return;
  }
  
#endif // AIRY_TCC
