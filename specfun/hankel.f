      subroutine hfuns( nu, arg, h1, h2, h1p, h2p, ierror )

      include  'utd.i'

      double complex  nu, arg, h1, h2, h1p, h2p
      double complex  sqtrm, alpha
      double precision  test, alphar, alphai
      double precision  mfun, nfun
      integer  morn, indexr, ierror, ierr
      character*1  aorb

      test = zabs((nu - arg)/nu**(1.0d0/3.0d0))
      ierror = 0
      if ( test .lt. 4.0d0 ) then
        call uniform_hankel( h1, h2, h1p, h2p, arg, nu )
      else
        sqtrm = zsqrt((nu/arg)**2 - 1.0d0)
        alpha = zlog((nu/arg) + sqtrm)
        if  ( dimag(alpha) .lt. 0.0d0 ) then
          alpha = -alpha
        end if
        alphar = dreal(alpha)
        alphai = dimag(alpha)
        if (       (dreal(nu) .gt. dreal(arg))
     *       .and. (dabs(dimag(nu/arg)) .le. 0.0d0) ) then
          indexr = 0
          aorb = ' '
        else
          call region( alpha, indexr, aorb, ierr )
        end if
        morn = 0
        if ( aorb .eq. 'A' ) then
          mfun = ((alphar*dtanh(alphar)-1.d0)*dtan(alphai)+alphai)/dpi
          morn = int(mfun)
          if (       (mfun .lt. 0.0d0)
     *         .and. (dmod(mfun,1.0d0) .ne. 0.0d0) ) then
            morn = morn - 1
          end if
        else if ( aorb .eq. 'B' ) then
          nfun = ((1.d0-alphar*dtanh(alphar))*dtan(alphai)-alphai)/dpi
          morn = int(nfun) + 1
          if (       (nfun .lt. 0.0d0)
     *         .and. (dmod(nfun,1.0d0) .ne. 0.0d0) ) then
            morn = morn - 1
          end if
        end if
        call h_debye( nu, arg, alpha, indexr, aorb, morn,
     *                h1, h2, h1p, h2p, ierror )
      end if

      return

      end

