C
C     COPYRIGHT:  Copyright 1998-2009
C                 Alion Science and Technology
C                 US Govt Retains rights in accordance
C                 with DoD FAR Supp 252.227 - 7013.
C


cpogo/////////////////////////////////////////////////////////////////////////
cpogo
cpogo  These subroutines are designed to augment those in file CYLSUBS.
cpogo  They provide asymptotic evaluations of the field coupling matrix
cpogo   elements involving Hankel functions using simplifed asymptotics
cpogo   suitable for very large orders.
cpogo
cpogo////////////////////////////////////////////////////////////////////////

      subroutine asymptotic_field_coupling(field_coupling_matrix,
     &                                     nu,
     &                                     index,
     &                                     in_or_out)

      include 'utd.i'
      include 'profile.i'

      double complex field_coupling_matrix(4,4),
     &           nu,eta1,eta2,k_ratio,eta_ratio,eta_prod,
     &           crosspol_factor,l_ratio,p_ratio,
     &           zsqrt

      integer  index,
     &         in_or_out

      eta1 = zsqrt(profmu(index-1)/profepsilon(index-1))
      if(dreal(eta1).lt.0.)eta1=-eta1
      eta2 = zsqrt(profmu(index)/profepsilon(index))
      if(dreal(eta2).lt.0.)eta2=-eta2

      crosspol_factor = (nu*alpha/interfaceradius(index))
     &                 *(1./(profbeta(index)*profbeta(index))
     &                 - 1./(profbeta(index-1)*profbeta(index-1)))

      k_ratio = profk(index)/profk(index-1)
      eta_ratio = eta2/eta1
      eta_prod  = eta2*eta1

      field_coupling_matrix(1,1) =-(l_ratio(2,2,nu,index)*eta2
     &                             -l_ratio(1,1,nu,index)*eta1)
     &           *k_ratio*eta_ratio*p_ratio(1,1,nu,index)/eta2
      field_coupling_matrix(1,2) =-(l_ratio(2,2,nu,index)*eta2
     &                             -l_ratio(2,1,nu,index)*eta1)
     &           *k_ratio*eta_ratio*p_ratio(2,1,nu,index)/eta2
      field_coupling_matrix(2,1) = (l_ratio(1,2,nu,index)*eta2
     &                             -l_ratio(1,1,nu,index)*eta1)
     &           *k_ratio*eta_ratio*p_ratio(1,2,nu,index)/eta2
      field_coupling_matrix(2,2) =-(l_ratio(2,1,nu,index)*eta1
     &                             -l_ratio(1,2,nu,index)*eta2)
     &           *k_ratio*eta_ratio*p_ratio(2,2,nu,index)/eta2
      field_coupling_matrix(3,3) =-(l_ratio(2,2,nu,index)/eta2
     &                             -l_ratio(1,1,nu,index)/eta1)
     &           *k_ratio/eta_ratio*p_ratio(1,1,nu,index)*eta2
      field_coupling_matrix(3,4) =-(l_ratio(2,2,nu,index)/eta2
     &                             -l_ratio(2,1,nu,index)/eta1)
     &           *k_ratio/eta_ratio*p_ratio(2,1,nu,index)*eta2
      field_coupling_matrix(4,3) = (l_ratio(1,2,nu,index)/eta2
     &                             -l_ratio(1,1,nu,index)/eta1)
     &           *k_ratio/eta_ratio*p_ratio(1,2,nu,index)*eta2
      field_coupling_matrix(4,4) =-(l_ratio(2,1,nu,index)/eta1
     &                             -l_ratio(1,2,nu,index)/eta2)
     &           *k_ratio/eta_ratio*p_ratio(2,2,nu,index)*eta2
      field_coupling_matrix(1,3) =  k_ratio*eta_prod
     &             *crosspol_factor*p_ratio(1,1,nu,index)/eta2
      field_coupling_matrix(1,4) =  k_ratio*eta_prod
     &             *crosspol_factor*p_ratio(2,1,nu,index)/eta2
      field_coupling_matrix(2,3) = -k_ratio*eta_prod
     &             *crosspol_factor*p_ratio(1,2,nu,index)/eta2
      field_coupling_matrix(2,4) = -k_ratio*eta_prod
     &             *crosspol_factor*p_ratio(2,2,nu,index)/eta2
      field_coupling_matrix(3,1) = -k_ratio/eta_prod
     &             *crosspol_factor*p_ratio(1,1,nu,index)*eta2
      field_coupling_matrix(3,2) = -k_ratio/eta_prod
     &             *crosspol_factor*p_ratio(2,1,nu,index)*eta2
      field_coupling_matrix(4,1) =  k_ratio/eta_prod
     &             *crosspol_factor*p_ratio(1,2,nu,index)*eta2
      field_coupling_matrix(4,2) =  k_ratio/eta_prod
     &             *crosspol_factor*p_ratio(2,2,nu,index)*eta2

c      argument1 = beta(index-1)*interfaceradius(index)
c      argument2 = beta(index)*interfaceradius(index)
c      call uniform_hankel(zh1,zh2,zh1p,zh2p,argument2,nu)
c      scale_factor = -dpi*zh1*zh2*argument1*argument1
c     & /(4.*k(index)*interfaceradius(index))
c      scale_factor = 1.
c      do 111 i=1,4
c      do 111 j=1,4
c      field_coupling_matrix(i,j)=
c     & field_coupling_matrix(i,j)*scale_factor
c 111  continue

      return
      end


cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      function p_ratio( ii, jj, nu, index )

      include 'utd.i'
      include 'profile.i'

      double complex  p_ratio,radical1,radical2,nu,
     &                conrad1,conrad2,r_factor,temp1,temp2,
     &                argument1,argument2,conarg1,conarg2,connu,
     &                zsqrt,zexp,dconjg,zlog
c      double complex  h11,h12,h21,h22,p11,p12,p21,p22,
c     &                h11p,h12p,h21p,h22p

      double precision  dimag,
     &                  s

      integer  ii, jj,
     &         nusw,
     &         index

      p_ratio = dcmplx(0.0d0, 0.0d0)

      nusw = 0
      if(dreal(nu).lt.0.)then
        nu = -nu
        nusw = 1
      endif

      argument1 = profbeta(index-1)*interfaceradius(index)
      argument2 = profbeta(index)  *interfaceradius(index)

      conarg1   = dconjg(argument1)
      conarg2   = dconjg(argument2)

      connu     = dconjg(nu)

      radical1  = zsqrt(nu*nu-argument1*argument1)
      radical2  = zsqrt(nu*nu-argument2*argument2)

      conrad1   = zsqrt(connu*connu-conarg1*conarg1)
      conrad2   = zsqrt(connu*connu-conarg2*conarg2)

      if((zabs(nu).gt.zabs(argument1))
     &          .and.(zabs(nu).gt.zabs(argument2)))then

        if((ii.eq.1).and.(jj.eq.1))then

          p_ratio = dconjg(zsqrt(conrad2/conrad1)
     &    *zexp(connu*zlog((connu+conrad1)/(connu+conrad2))
     &    +conrad2-conrad1))

          go to 99
        endif

        if((ii.eq.1).and.(jj.eq.2))then

          p_ratio =-zsqrt(radical2/radical1)
     &    *zexp(dconjg(connu*zlog(connu+conrad1))
     &    -(nu*zlog((nu+radical2)))+radical2-radical1)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.1))then

          p_ratio =-zsqrt(radical2/radical1)
     &    *zexp(nu*zlog((nu+radical1))
     &    -dconjg(connu*zlog(connu+conrad2))
     &    +radical2-radical1)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.2))then

          p_ratio = zsqrt(radical2/radical1)
     &    *zexp(nu*zlog((nu+radical1)/(nu+radical2))
     &    +radical2-radical1)

          go to 99
        endif

      endif

      if((zabs(nu).lt.zabs(argument1))
     &      .and.(zabs(nu).lt.zabs(argument2)))then

        if((dimag(argument1).eq.0.).and.(dimag(nu).lt.0.))then
                             radical1 =-radical1
                             conrad1  =-conrad1
        endif

        if((dimag(argument2).eq.0.).and.(dimag(nu).lt.0.))then
                             radical2 =-radical2
                             conrad2  =-conrad2
        endif

        if(dimag(argument1).gt.0.)then
                             radical1 =-radical1
                             conrad1  =-conrad1
        endif

        if(dimag(argument2).gt.0.)then
                             radical2 =-radical2
                             conrad2  =-conrad2
        endif

        temp1 = radical1
        temp2 = radical2

        if((dimag(argument1).eq.0.).and.(dimag(nu).eq.0.))then
                             radical1 = conrad1
                             conrad1  =-temp1
        endif

        if((dimag(argument2).eq.0.).and.(dimag(nu).eq.0.))then
                             radical2 = conrad2
                             conrad2  =-temp2
        endif

        s = 1.
        if(dimag(radical2/radical1).le.0.) s = -s

        if((ii.eq.1).and.(jj.eq.1))then

          p_ratio = dconjg(zsqrt(conrad2/conrad1)
     &    *zexp(connu*zlog((connu-conrad1)/(connu-conrad2)
     &    *(conarg2/conarg1))-conrad2+conrad1))

          go to 99
        endif

        if((ii.eq.1).and.(jj.eq.2))then

          p_ratio =s*zsqrt(-radical2/radical1)
     &    *zexp(dconjg(connu*zlog((connu-conrad1)/conarg1))
     &    -(nu*zlog((nu+radical2)/argument2))+radical2+radical1)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.1))then

          p_ratio =-s*zsqrt(-radical2/radical1)
     &          *zexp(nu*zlog((nu+radical1)/argument1)
     &    -dconjg(connu*zlog((connu-conrad2)/conarg2))
     &    -radical2-radical1)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.2))then

          p_ratio = zsqrt(radical2/radical1)
     &    *zexp(nu*zlog((nu+radical1)/(nu+radical2)
     &    *(argument2/argument1))+radical2-radical1)

          go to 99
        endif

      endif

      if((zabs(nu).lt.zabs(argument1))
     &      .and.(zabs(nu).gt.zabs(argument2)))then

        if((dimag(argument1).eq.0.).and.(dimag(nu).lt.0.))then
                             radical1 =-radical1
                             conrad1  =-conrad1
        endif

        if(dimag(argument1).gt.0.)then
                             radical1 =-radical1
                             conrad1  =-conrad1
        endif

        temp1 = radical1

        if((dimag(argument1).eq.0.).and.(dimag(nu).eq.0.))then
                             radical1 = conrad1
                             conrad1  =-temp1
        endif

        if((ii.eq.1).and.(jj.eq.1))then

          p_ratio = dconjg(zsqrt(conrad2/conrad1)
     &    *zexp(connu*zlog((connu-conrad1)/(connu+conrad2)
     &    *(conarg2/conarg1))+conrad2+conrad1))

          go to 99
        endif

        if((ii.eq.1).and.(jj.eq.2))then

          p_ratio = -zsqrt(-radical2/radical1)
     &    *zexp(dconjg(connu*zlog((connu-conrad1)/conarg1))
     &    -(nu*zlog((nu+radical2)/argument2))+radical2+radical1)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.1))then

          p_ratio = -zsqrt(radical2/radical1)
     &          *zexp(nu*zlog((nu+radical1)/argument1)
     &    -dconjg(connu*zlog((connu+conrad2)/conarg2))
     &    +radical2-radical1)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.2))then

          p_ratio = zsqrt(radical2/radical1)
     &    *zexp(nu*zlog((nu+radical1)/(nu+radical2)
     &    *(argument2/argument1))+radical2-radical1)

          go to 99
        endif

      endif

      if((zabs(nu).gt.zabs(argument1))
     &      .and.(zabs(nu).lt.zabs(argument2)))then

        if((dimag(argument2).eq.0.).and.(dimag(nu).lt.0.))then
                             radical2 =-radical2
                             conrad2  =-conrad2
        endif

        if(dimag(argument2).gt.0.)then
                             radical2 =-radical2
                             conrad2  =-conrad2
        endif

        temp2 = radical2

        if((dimag(argument2).eq.0.).and.(dimag(nu).eq.0.))then
                             radical2 = conrad2
                             conrad2  =-temp2
        endif

        if((ii.eq.1).and.(jj.eq.1))then

          p_ratio = dconjg(zsqrt(-conrad2/conrad1)
     &    *zexp(connu*zlog((connu+conrad1)/(connu-conrad2)
     &    *(conarg2/conarg1))-conrad2-conrad1))

          go to 99
        endif

        if((ii.eq.1).and.(jj.eq.2))then

          p_ratio = -zsqrt(radical2/radical1)
     &    *zexp(dconjg(connu*zlog((connu+conrad1)/conarg1))
     &    -(nu*zlog((nu+radical2)/argument2))+radical2-radical1)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.1))then

          p_ratio = -zsqrt(-radical2/radical1)
     &          *zexp(nu*zlog((nu+radical1)/argument1)
     &    -dconjg(connu*zlog((connu-conrad2)/conarg2))
     &    -radical2-radical1)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.2))then

          p_ratio = zsqrt(radical2/radical1)
     &    *zexp(nu*zlog((nu+radical1)/(nu+radical2)
     &    *(argument2/argument1))+radical2-radical1)

          go to 99
        endif

      endif

   99 if(nusw.eq.1)then
        r_factor = zexp((0.,2.)*nu*dpi)
        if((ii.eq.1).and.(jj.eq.2))p_ratio=p_ratio*r_factor
        if((ii.eq.2).and.(jj.eq.1))p_ratio=p_ratio/r_factor
        nu = -nu
      endif

c      call uniform_hankel_olver(h11,h21,h11p,h21p,argument1,nu)
c      call uniform_hankel_olver(h12,h22,h12p,h22p,argument2,nu)
c      p11 = h11/h12
c      p12 = h11/h22
c      p21 = h21/h12
c      p22 = h21/h22
c      write(6,*)ii,jj
c      write(6,234)p11,p12,p21,p22,p_ratio
c 234  format(1h ,2e20.8)

      if((zabs(nu).gt.zabs(argument1)).and.(zabs(nu).lt.
     &    zabs(argument2)))p_ratio=p_ratio*(argument1/nu)**nu

      if((zabs(nu).gt.zabs(argument2)).and.(zabs(nu).lt.
     &    zabs(argument1)))p_ratio=p_ratio*(nu/argument2)**nu

c      if((zabs(nu).gt.zabs(argument1))
c     &                  .and.(zabs(nu).gt.zabs(argument2)))
c     &   p_ratio = p_ratio*(argument2/argument1)**nu
c      p_ratio = (1.,0.)
c      write(6,234)nu,argument1,argument2
c  234 format(1h ,2e20.8)
c      write(6,*)'p_ratio = ',p_ratio
      return

      end function



cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      function l_ratio(ii,jj,nu,index)

      include 'utd.i'
      include 'profile.i'

      double complex  l_ratio, nu, zsqrt, argument
c      double complex  h1, h2, h1p, h2p, l11, l21

c  234 format(1h ,2e20.8)

      integer  ii, jj,
     &         index

      argument = profbeta(index+jj-2)*interfaceradius(index)
      l_ratio = -zsqrt(nu*nu-argument*argument)/argument

      if((zabs(nu).gt.zabs(argument)).and.(dreal(l_ratio).gt.0.))
     &l_ratio=-l_ratio
      if((zabs(nu).le.zabs(argument)).and.(dimag(l_ratio)*(2*ii-3)
     &.gt.0.))l_ratio=-l_ratio

c      l_ratio = l_ratio*k(index+jj-2)*dj/(beta(index+jj-2)*nu)
      l_ratio = l_ratio*profk(index+jj-2)*dj/(profbeta(index+jj-2))

c      call uniform_hankel_olver(h1,h2,h1p,h2p,argument,nu)
c      l11 = (h1p/h1)*(dj*k(index+jj-2)/beta(index+jj-2))
c      l21 = (h2p/h2)*(dj*k(index+jj-2)/beta(index+jj-2))
c      write(6,*)ii,jj,index
c      write(6,234)nu,argument,l11,l21,l_ratio

      return

      end function



cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      function q_ratio(ii,jj,nu,index)

      include 'utd.i'
      include 'profile.i'

      double complex  q_ratio, radical, nu,
     &                conrad, r_factor, temp,
     &                argument, conarg, connu
c      double complex  h1, h2, hp1, hp2, q12, q21

c  222 format(1h ,2e20.8)

      integer  ii, jj,
     &         nusw,
     &         index

      q_ratio = dcmplx(0.0d0, 0.0d0)

      nusw = 0
      if(dreal(nu).lt.0.)then
         nu = -nu
         nusw = 1
      endif

      argument = profbeta(index-1)*interfaceradius(index)

      conarg   = dconjg(argument)

      connu     = dconjg(nu)

      radical  = zsqrt(nu*nu-argument*argument)

      conrad   = zsqrt(connu*connu-conarg*conarg)

      if(zabs(nu).gt.zabs(argument))then

        if((ii.eq.1).and.(jj.eq.1))then

          q_ratio = 1.0

          go to 99
        endif

        if((ii.eq.1).and.(jj.eq.2))then

          q_ratio =
     &    -zexp(dconjg(connu*zlog(connu+conrad))
     &    -(nu*zlog(nu+radical)))

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.1))then

cph
cph the following code change was made to make the code compile
cph i don't know what pogo had in mind, but this branch is never
cph entered
cph
cph          q_ratio =
cph     &          -zexp(nu*zlog((nu+radical1))
cph     &    -dconjg(connu*zlog(connu+conrad2)))
          q_ratio =
     &          -zexp(nu*zlog((nu+radical))
     &    -dconjg(connu*zlog(connu+conrad)))

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.2))then

          q_ratio = 1.0

          go to 99
        endif

      else

        if((dimag(argument).eq.0.).and.(dimag(nu).lt.0.))then
                             radical =-radical
                             conrad  =-conrad
        endif

        if(dimag(argument).gt.0.)then
                             radical =-radical
                             conrad  =-conrad
        endif

        temp = radical

        if((dimag(argument).eq.0.).and.(dimag(nu).eq.0.))then
                             radical = conrad
                             conrad  =-temp
        endif

        if((ii.eq.1).and.(jj.eq.1))then

          q_ratio = 1.0

          go to 99
        endif

        if((ii.eq.1).and.(jj.eq.2))then

          q_ratio = -zsqrt((-1.d0,0.))*
     &    zexp(dconjg(connu*zlog((connu-conrad)/conarg))
     &    -(nu*zlog((nu+radical)/argument))+radical+radical)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.1))then

          q_ratio = zsqrt((-1.d0,0.))*
     &          zexp(nu*zlog((nu+radical)/argument)
     &    -dconjg(connu*zlog((connu-conrad)/conarg))
     &    -radical-radical)

          go to 99
        endif

        if((ii.eq.2).and.(jj.eq.2))then

          q_ratio = 1.0

          go to 99
        endif

      endif

   99 if(nusw.eq.1)then
        r_factor = zexp((0.,2.)*nu*dpi)
        if((ii.eq.1).and.(jj.eq.2))q_ratio=q_ratio*r_factor
        if((ii.eq.2).and.(jj.eq.1))q_ratio=q_ratio/r_factor
        nu = -nu
      endif

c      call uniform_hankel(h1,h2,hp1,hp2,argument,nu)
c      q12 = h1/h2
c      q21 = h2/h1
c      write(6,*)ii,jj
c      write(6,222)argument
c      write(6,222)nu
c      write(6,222)q12,q21,q_ratio
c      write(6,*)'q_ratio = ',q_ratio
      return
      end



ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      subroutine compute_surface_impedance( reflection_matrix,
     &                                      field_coef_matrix,
     &                                      nu, eta )

      include 'utd.i'
      include 'profile.i'

      double complex  surface_impedance_matrix(2,2),
     &                reflection_matrix(2,2),
     &                field_coef_matrix(4,4),
     &                nu, dnm, v,
     &                eta, l_ratio, q_ratio, deltar, delta,
     &                ep1, ep2, ez1, ez2, hp1, hp2, hz1, hz2,
     &                argument, l11, l21, q12, q21,
     &                hankel_1, hankel_2,
     &                hankel_1_prime, hankel_2_prime

      double precision  test

      integer  index

      common /zsurf/ surface_impedance_matrix


      write(6,*)reflection_matrix
      ep1 =      field_coef_matrix(1,1)
     &          +field_coef_matrix(1,2)*reflection_matrix(1,1)
     &          +field_coef_matrix(1,4)*reflection_matrix(2,1)
      ep2 =      field_coef_matrix(1,2)*reflection_matrix(1,2)
     &          +field_coef_matrix(1,3)
     &          +field_coef_matrix(1,4)*reflection_matrix(2,2)
      hz1 =      field_coef_matrix(2,1)
     &          +field_coef_matrix(2,2)*reflection_matrix(1,1)
      hz2 =      field_coef_matrix(2,2)*reflection_matrix(1,2)

      ez1 =      field_coef_matrix(4,4)*reflection_matrix(2,1)
      ez2 =      field_coef_matrix(4,3)
     &          +field_coef_matrix(4,4)*reflection_matrix(2,2)
      hp1 =      field_coef_matrix(3,1)
     &          +field_coef_matrix(3,2)*reflection_matrix(1,1)
     &          +field_coef_matrix(3,4)*reflection_matrix(2,1)
      hp2 =      field_coef_matrix(3,2)*reflection_matrix(1,2)
     &          +field_coef_matrix(3,3)
     &          +field_coef_matrix(3,4)*reflection_matrix(2,2)

      delta = hp1*hz2-hp2*hz1

      surface_impedance_matrix(1,1) = (hp1*ep2-hp2*ep1)/delta
      surface_impedance_matrix(1,2) = (ep1*hz2-ep2*hz1)/delta
      surface_impedance_matrix(2,1) = (hp1*ez2-hp2*ez1)/delta
      surface_impedance_matrix(2,2) = (ez1*hz2-ez2*hz1)/delta

      index = 1
      deltar = reflection_matrix(1,1)*reflection_matrix(2,2)
     &        -reflection_matrix(1,2)*reflection_matrix(2,1)

      write(6,234)surface_impedance_matrix
  234 format(1h ,2e20.8)

      argument = profbeta(noofinterfaces-1)
     &          *interfaceradius(noofinterfaces)
      test = 10.*(zabs(argument))**(1./3.)

c      if(zabs(argument-nu).gt.test)then
      if ( dpi .gt. 4. ) then

      v = alpha*nu/(profbeta(noofinterfaces-1)**2
     &    *interfaceradius(noofinterfaces))

      dnm = 
     &   (l_ratio(1,1,nu,index)*q_ratio(1,2,nu,index)
     &   +l_ratio(2,1,nu,index)*reflection_matrix(2,2)
     &   +l_ratio(1,1,nu,index)*reflection_matrix(1,1)
     &   +l_ratio(2,1,nu,index)*q_ratio(2,1,nu,index)*deltar)

      surface_impedance_matrix(1,1) = eta
     &   *(q_ratio(1,2,nu,index)*(l_ratio(1,1,nu,index)**2+v*v)
     &   +(l_ratio(1,1,nu,index)*l_ratio(2,1,nu,index)+v*v)
     &   *(reflection_matrix(2,2)+reflection_matrix(1,1))
     &   +q_ratio(2,1,nu,index)*(l_ratio(2,1,nu,index)**2+v*v)*deltar
     &   +v*(l_ratio(1,1,nu,index)-l_ratio(2,1,nu,index))
     &   *(reflection_matrix(1,2)/eta-reflection_matrix(2,1)*eta))/dnm

      surface_impedance_matrix(1,2) = eta*((q_ratio(1,2,nu,index)
     &   +reflection_matrix(2,2)+reflection_matrix(1,1)
     &   +q_ratio(2,1,nu,index)*deltar)*v
     &   +(l_ratio(1,1,nu,index)-l_ratio(2,1,nu,index))
     &   *reflection_matrix(1,2)/eta)/dnm

      surface_impedance_matrix(2,1) = -eta*((q_ratio(1,2,nu,index)
     &   +reflection_matrix(2,2)+reflection_matrix(1,1)
     &   +q_ratio(2,1,nu,index)*deltar)*v
     &   -(l_ratio(1,1,nu,index)-l_ratio(2,1,nu,index))
     &   *reflection_matrix(2,1)*eta)/dnm

      surface_impedance_matrix(2,2) = -eta*(q_ratio(1,2,nu,index)
     &   +reflection_matrix(2,2)+reflection_matrix(1,1)
     &   +q_ratio(2,1,nu,index)*deltar)/dnm

      write(6,234)surface_impedance_matrix

      q21 = q_ratio(2,1,nu,index)
      q12 = q_ratio(1,2,nu,index)
      l11 = l_ratio(1,1,nu,index)
      l21 = l_ratio(2,1,nu,index)
      write(6,*)
      write(6,*)q12,q21,l11,l21

      call uniform_hankel(hankel_1,hankel_2,hankel_1_prime
     &,hankel_2_prime,argument,nu)
      q21 = hankel_2/hankel_1
      q12 = hankel_1/hankel_2
      l11 = profk(noofinterfaces-1)*(0.,1.)*hankel_1_prime
     &/(profbeta(noofinterfaces-1)*hankel_1)
      l21 = profk(noofinterfaces-1)*(0.,1.)*hankel_2_prime
     &/(profbeta(noofinterfaces-1)*hankel_2)
      write(6,*)
      write(6,*)q12,q21,l11,l21
      write(6,*)

      else

      call uniform_hankel(hankel_1,hankel_2,hankel_1_prime
     &,hankel_2_prime,argument,nu)
      q21 = hankel_2/hankel_1
      q12 = hankel_1/hankel_2
      l11 = profk(noofinterfaces-1)*(0.,1.)*hankel_1_prime
     &/(profbeta(noofinterfaces-1)*hankel_1)
      l21 = profk(noofinterfaces-1)*(0.,1.)*hankel_2_prime
     &/(profbeta(noofinterfaces-1)*hankel_2)

      v = alpha*nu/(profbeta(noofinterfaces-1)**2
     &            *interfaceradius(noofinterfaces))

      dnm = l11*q12+l21*reflection_matrix(2,2)
     &     +l11*reflection_matrix(1,1)+l21*q21*deltar

      surface_impedance_matrix(1,1) = eta*
     &     (q12*(l11*l11+v*v)
     &     +(l11*l21+v*v)*(reflection_matrix(2,2)
     &     +reflection_matrix(1,1))
     &     +q21*(l21*l21+v*v)*deltar
     &     +v*(l11-l21)*(reflection_matrix(1,2)/eta
     &     -reflection_matrix(2,1)*eta))
     &     /dnm

      surface_impedance_matrix(1,2) = eta
     &     *((q12+reflection_matrix(2,2)+reflection_matrix(1,1)
     &     +q21*deltar)*v+(l11-l21)*reflection_matrix(1,2)/eta)/dnm

      surface_impedance_matrix(2,1) = -eta
     &     *((q12+reflection_matrix(2,2)+reflection_matrix(1,1)
     &     +q21*deltar)*v-(l11-l21)*reflection_matrix(2,1)*eta)/dnm

      surface_impedance_matrix(2,2) = -eta*(q12
     &     +reflection_matrix(2,2)+reflection_matrix(1,1)
     &     +q21*deltar)/dnm

      write(6,234)surface_impedance_matrix

      endif

      return
      end
