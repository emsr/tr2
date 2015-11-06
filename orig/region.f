C
C     COPYRIGHT:  Copyright 1998-2006
C                 Alion Science and Technology
C                 US Govt Retains rights in accordance
C                 with DoD FAR Supp 252.227 - 7013.
C

      subroutine region( alpha, indexr, aorb, ierror )

      include 'utd.i'

      double complex  alpha
      double precision  alphar, alphai, dreal, dimag
      double precision  f1, f2
      double precision  dsin, dcos, dsinh, dcosh
      integer  indexr, ierror
      character*1  aorb


      ierror = 0
      aorb = ' '
      alphar = dreal(alpha)
      alphai = dimag(alpha)

      f1 = 1.0d0 - alphai*dcos(alphai)/dsin(alphai)
     *           - alphar*dsinh(alphar)/dcosh(alphar)

      f2 = 1.0d0 + (dpi-alphai)*dcos(alphai)/dsin(alphai)
     *           - alphar*dsinh(alphar)/dcosh(alphar)

      if ((f1 .gt. 0.0d0) .and. (f2 .gt. 0.0d0)) then
        indexr = 1
      else if (f2 .gt. 0.0d0) then
        if (alphar .gt. 0.0d0) then
          indexr = 2
        else
          indexr = 3
        end if
      else if (f1 .gt. 0.0d0) then
        if (alphar .gt. 0.0d0) then
          indexr = 4
        else
          indexr = 5
        end if
      else
        if (alphar .gt. 0.0d0) then
          indexr = 6
        else
          indexr = 7
        end if
        if (alphai .le. (dpi/2.0d0)) then
          aorb = 'A'
        else
          aorb = 'B'
        end if
      end if

      return

      end

