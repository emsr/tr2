      subroutine h_debye( nu, arg, alpha, indexr, aorb, morn,
     *                    h1dbye, h2dbye, h1pdby, h2pdby, ierror )

      include 'utd.i'
      include 'hyper.i'

      double complex  nu, arg, alpha, h1dbye, h2dbye, h1pdby, h2pdby
      double complex  s1, s2, denom, thalpa, sinrat, jdbye
      double complex  snhalp
      double complex  exparg
      double precision  alphar, alphai
      integer  indexr, morn, ierror
      character*1  aorb
      write(6,*) 'hankel_debye:'
      write(6,*) '> > nu = ', nu
      write(6,*) '> > z  = ', arg

c      write(3,*) ' region = ',indexr
c      write(3,*) ' alpha = ',alpha
      alphar = dreal(alpha)
      alphai = dimag(alpha)
      thalpa = sinh(alpha)/cosh(alpha)
      snhalp = sinh(alpha)
c      write(3,*) ' alpha = ',alpha,' sinh(alpha) = ',snhalp
      denom = sqrt(dpi*arg/2.0d0)*sqrt(-dj*sinh(alpha))
c      write(3,*) ' arg = ',arg
c      write(3,*) ' denom = ',denom
      if (abs(dreal(nu*(thalpa-alpha))) .gt. 690.d0) then
        ierror = 1
        go to 10
      end if
      s1 = zexp(nu*(thalpa-alpha) - dj*dpi/4.0d0) / denom
      s2 = zexp(-nu*(thalpa-alpha) + dj*dpi/4.0d0) / denom
      exparg = nu*(thalpa-alpha) - dj*dpi/4.0d0
c      write(3,*) ' exparg = ',exparg      
c      write(3,*) ' s1 = ',s1
c      write(3,*) ' s2 = ',s2
      if (indexr .eq. 0) then
        h1dbye = 0.5d0*s1 - s2
        h2dbye = 0.5d0*s1 + s2
        h1pdby = snhalp*(0.5d0*s1+s2)
        h2pdby = snhalp*(0.5d0*s1-s2)
      else if (indexr .eq. 1) then
        h1dbye = s1
        h2dbye = s2
        h1pdby = snhalp*s1
        h2pdby = -snhalp*s2
      else if (indexr .eq. 2) then
        jdbye = s1/2.d0
        h2dbye = s2
        h1dbye = 2.0d0*jdbye - h2dbye
        h1pdby = snhalp*(s1+s2)
        h2pdby = -snhalp*s2
      else if (indexr .eq. 3) then
        h1dbye = s1
        h2dbye = s2 - s1
        h1pdby = snhalp*s1
        h2pdby = -snhalp*(s1+s2)
      else if (indexr .eq. 4) then
        h1dbye = s1
        h2dbye = s2 - zexp(2.*dj*nu*dpi)*s1
        h1pdby = snhalp*s1
        h2pdby = -snhalp*(s2+zexp(2.*dj*nu*dpi)*s1)
      else if (indexr .eq. 5) then
        h1dbye = s1 - zexp(-2.0d0*dj*nu*dpi)*s2
        h2dbye = s2
        h1pdby = snhalp*(s1+zexp(-2.0d0*dj*nu*dpi)*s2)
        h2pdby = -snhalp*s2
      else if (aorb .eq. 'A') then
        if ((abs(dimag(nu)) .lt. 1.0d-08) .and.
     *      (abs(dmod(dreal(nu),1.0d0)) .lt. 1.0d-08)) then
          sinrat = dcmplx(dfloat(morn),0.0d0)
        else
          sinrat = zsin(morn*nu*dpi)/zsin(nu*dpi)
        end if
        if (indexr .eq. 6) then
          h2dbye = s2 - zexp(dj*(morn+1)*nu*dpi)*sinrat*s1
          h1dbye = s1 - h2dbye
          h2pdby = -snhalp*(s2+zexp(dj*(morn+1)*nu*dpi)*sinrat*s1)
          h1pdby = snhalp*((1.0d0+zexp(dj*(morn+1)*nu*dpi)*sinrat)*s1
     *                     +s2)
        else if (indexr .eq. 7) then
          h1dbye = s1 - zexp(-dj*(morn+1)*nu*dpi)*sinrat*s2
          h2dbye = s2 - h1dbye
          h1pdby = snhalp*(s1+zexp(-dj*(morn+1)*nu*dpi)*sinrat*s2)
          h2pdby = -snhalp*
     *             ((1.0d0+zexp(-dj*(morn+1)*nu*dpi)*sinrat)*s2+s1)
        else
          ierror = 1
        end if
      else
        if ((abs(dimag(nu)) .lt. 1.0d-08) .and.
     *      (abs(dmod(dreal(nu),1.d0)) .lt. 1.0d-08)) then
          sinrat = dcmplx(dfloat(morn),0.0d0)
        else
          sinrat = zsin(morn*nu*dpi)/zsin(nu*dpi)
        end if
        if (indexr .eq. 6) then
          h1dbye = s1 - zexp(dj*(morn-1)*nu*dpi)*sinrat*s2
          h2dbye = s2 - zexp(2.0d0*dj*nu*dpi)*h2dbye
          h1pdby = snhalp*(s1+zexp(dj*(morn-1)*nu*dpi)*sinrat*s2)
          h2pdby = -snhalp*((1.0d0+zexp(dj*(morn+1)*nu*dpi)*sinrat)*s2+
     *                     zexp(2.0d0*dj*nu*dpi)*s1)
        else if (indexr .eq. 7) then
          h2dbye = s2 - zexp(-dj*(morn-1)*nu*dpi)*sinrat*s1
          h1dbye = s1 - zexp(-2.0d0*dj*nu*dpi)*h2dbye
          h2pdby = -snhalp*(s2+zexp(-dj*(morn-1)*nu*dpi)*sinrat*s1)
          h1pdby = snhalp*((1.d0+zexp(-dj*(morn+1)*nu*dpi)*sinrat)*s1+
     *                     zexp(-2.0d0*dj*nu*dpi)*s2)
        else
          ierror = 1
        end if
      end if
c
   10 continue
c
      return

      end subroutine
