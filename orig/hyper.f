C
C     COPYRIGHT:  Copyright 1998-2006
C                 Alion Science and Technology
C                 US Govt Retains rights in accordance
C                 with DoD FAR Supp 252.227 - 7013.
C


      !
      !
      !
      complex function ccosh(x)

      complex x

      ccosh = (cexp(x) + cexp(-x)) / 2.0

      return

      end function


      !
      !
      !
      complex function csinh(x)

      complex x

      csinh = (cexp(x) - cexp(-x)) / 2.0

      return

      end function


      !
      !
      !
      double complex function zcosh(x)

      double complex x

      zcosh = (zexp(x) + zexp(-x)) / 2.0d0

      return

      end function


      !
      !
      !
      double complex function zsinh(x)

      double complex x

      zsinh = (zexp(x) - zexp(-x)) / 2.0d0

      return

      end function

