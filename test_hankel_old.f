
c $HOME/bin/bin/gfortran -Wall -Wextra -I$HOME/ARAPP/diffraction -o test_hankel_old test_hankel_old.f region.f cylsubs.f hankel.f hankel_debye.f

      program test_hankel_old

      double complex h1, h2, h1p, h2p
      double complex z, nu, j
      double complex ai, aip
      integer ier

      nu = dcmplx(5.0d0, 0.0d0)
      z = dcmplx(1.0d0, -3.0d0)
      j = dcmplx(0.0d0, 1.0d0)

      call uniform_hankel(h1, h2, h1p, h2p, z, nu)

      write(6,*) '\n'
      write(6,*) 'nu     = ', nu
      write(6,*) 'z      = ', z
      write(6,*) 'H1(z)  = ', h1
      write(6,*) 'H1''(z) = ', h1p
      write(6,*) 'H2(z)  = ', h2
      write(6,*) 'H2''(z) = ', h2p
      write(6,*) 'J(z)   = ', (h1 + h2) / 2.0d0
      write(6,*) 'J''(z)  = ', (h1p + h2p) / 2.0d0
      write(6,*) 'Y(z)   = ', (h1 - h2) / (2.0d0 * j)
      write(6,*) 'Y''(z)  = ', (h1p - h2p) / (2.0d0 * j)

      call zairy(z, 1.0d-16, ai, aip, ier)
      write(6,*) ''
      write(6,*) 'z      = ', z
      write(6,*) 'Ai(z)  = ', ai
      write(6,*) 'Ai''(z) = ', aip
      end program
