// /home/ed/bin/bin/g++ -std=c++11 -o test_specfun test_specfun.cpp


#include <iostream>
#include <iomanip>


#include "cmath"


int
main()
{
    //  sinc
    std::cout << std::endl << "x sinc" << std::endl;
    for (int i = 0; i <= 1000; ++i)
    {
        double x = 0.01 * (i - 500);
        std::cout << std::setw(12) << x
                  << std::setw(12) << sinc(x)
                  << std::endl;
    }

    //  logint
    std::cout << std::endl << "x logint" << std::endl;
    for (int i = 0; i <= 500; ++i)
    {
        double x = 0.01 * i;
        std::cout << std::setw(12) << x
                  << std::setw(12) << logint(x)
                  << std::endl;
    }
////  FAIL Past |2|
    std::cout << std::endl << "x cosint sinint" << std::endl;
    for (int i = 0; i <= 400; ++i)
    {
        double x = 0.01 * (i - 200);
        std::cout << std::setw(12) << x
                  << std::setw(12) << cosint(x)
                  << std::setw(12) << sinint(x)
                  << std::endl;
    }

    //  coshint sinhint
    std::cout << std::endl << "x coshint sinhint" << std::endl;
    for (int i = 0; i <= 1000; ++i)
    {
        double x = 0.01 * (i - 500);
        std::cout << std::setw(12) << x
                  << std::setw(12) << coshint(x)
                  << std::setw(12) << sinhint(x)
                  << std::endl;
    }

    std::cout << std::endl << "x jacobi_cn jacobi_sn jacobi_dn" << std::endl;
    for (int j = 0; j <= 5; ++j)
    {
        double k = j * 1.0;
	std::cout << std::endl;
	for (int i = 0; i <= 200; ++i)
	{
            double u = 0.01 * (i - 100);
            std::cout << std::setw(12) << u
                      << std::setw(12) << jacobi_cn(k, u)
                      << std::setw(12) << jacobi_sn(k, u)
                      << std::setw(12) << jacobi_dn(k, u)
                      << std::endl;
	}
    }

    std::cout << std::endl << "x fresnel_c fresnel_s" << std::endl;
    for (int i = 0; i <= 1000; ++i)
    {
        double x = 0.01 * (i - 500);
        std::cout << std::setw(12) << x
                  << std::setw(12) << fresnel_c(x)
                  << std::setw(12) << fresnel_s(x)
                  << std::endl;
    }

    std::cout << std::endl << "x airy_ai airy_bi" << std::endl;
    for (int i = 0; i <= 1000; ++i)
    {
        double x = 0.01 * (i - 500);
        std::cout << std::setw(12) << x
                  << std::setw(12) << airy_ai(x)
                  << std::setw(12) << airy_bi(x)
                  << std::endl;
    }

    std::cout << std::endl << "x chebyshev_t" << std::endl;
    for (int i = 0; i <= 200; ++i)
    {
        double x = 0.01 * (i - 100);
        std::cout << std::setw(12) << x
                  << std::setw(12) << chebyshev_t(0, x)
                  << std::setw(12) << chebyshev_t(1, x)
                  << std::setw(12) << chebyshev_t(2, x)
                  << std::setw(12) << chebyshev_t(3, x)
                  << std::setw(12) << chebyshev_t(4, x)
                  << std::setw(12) << chebyshev_t(5, x)
                  << std::endl;
    }

    std::cout << std::endl << "x chebyshev_u" << std::endl;
    for (int i = 0; i <= 200; ++i)
    {
        double x = 0.01 * (i - 100);
        std::cout << std::setw(12) << x
                  << std::setw(12) << chebyshev_u(0, x)
                  << std::setw(12) << chebyshev_u(1, x)
                  << std::setw(12) << chebyshev_u(2, x)
                  << std::setw(12) << chebyshev_u(3, x)
                  << std::setw(12) << chebyshev_u(4, x)
                  << std::setw(12) << chebyshev_u(5, x)
                  << std::endl;
    }

    std::cout << std::endl << "x chebyshev_v" << std::endl;
    for (int i = 0; i <= 200; ++i)
    {
        double x = 0.01 * (i - 100);
        std::cout << std::setw(12) << x
                  << std::setw(12) << chebyshev_v(0, x)
                  << std::setw(12) << chebyshev_v(1, x)
                  << std::setw(12) << chebyshev_v(2, x)
                  << std::setw(12) << chebyshev_v(3, x)
                  << std::setw(12) << chebyshev_v(4, x)
                  << std::setw(12) << chebyshev_v(5, x)
                  << std::endl;
    }

    std::cout << std::endl << "x chebyshev_w" << std::endl;
    for (int i = 0; i <= 200; ++i)
    {
        double x = 0.01 * i;
        std::cout << std::setw(12) << x
                  << std::setw(12) << chebyshev_w(0, x)
                  << std::setw(12) << chebyshev_w(1, x)
                  << std::setw(12) << chebyshev_w(2, x)
                  << std::setw(12) << chebyshev_w(3, x)
                  << std::setw(12) << chebyshev_w(4, x)
                  << std::setw(12) << chebyshev_w(5, x)
                  << std::endl;
    }

    std::cout << std::endl << "x gamma_u" << std::endl;
    for (int i = 0; i <= 200; ++i)
    {
        double x = 0.01 * i;
        std::cout << std::setw(12) << x;
        for (int j = 1; j <= 5; ++j)
        {
            double a = j * 0.5;
            std::cout << std::setw(12) << gamma_u(a, x);
	}
	std::cout << std::endl;
    }

    std::cout << std::endl << "x gamma_l" << std::endl;
    for (int i = 0; i <= 200; ++i)
    {
        double x = 0.01 * i;
        std::cout << std::setw(12) << x;
        for (int j = 1; j <= 5; ++j)
        {
            double a = j * 0.5;
            std::cout << std::setw(12) << gamma_l(a, x);
	}
	std::cout << std::endl;
    }

    std::cout << std::endl << "x pochhammer_u" << std::endl;
    for (int i = 0; i <= 200; ++i)
    {
        double x = 0.01 * (i - 100);
        std::cout << std::setw(12) << x;
        for (int j = 0; j <= 5; ++j)
        {
            double a = j * 1.0;
            std::cout << std::setw(12) << pochhammer_u(a, x);
	}
	std::cout << std::endl;
    }

    std::cout << std::endl << "x pochhammer_l" << std::endl;
    for (int i = 0; i <= 200; ++i)
    {
        double x = 0.01 * (i - 100);
        std::cout << std::setw(12) << x;
        for (int j = 0; j <= 5; ++j)
        {
            double a = j * 1.0;
            std::cout << std::setw(12) << pochhammer_l(a, x);
	}
	std::cout << std::endl;
    }

    std::cout << std::endl << "jacobi" << std::endl;
    for (int n = 0; n <= 5; ++n)
    {
	std::cout << "n=" << n << std::endl;
	for (int k = 0; k <= 200; ++k)
	{
            double x = 0.01 * (k - 100);
            //std::cout << std::endl;
            std::cout << std::setw(12) << x;
	    for (int i = 0; i <= 3; ++i)
	    {
                double alpha = i * 1.0;
                for (int j = 0; j <= 3; ++j)
                {
                    double beta = j * 1.0;
        	    std::cout << std::setw(12) << jacobi(n, alpha, beta, x);
                }
                //std::cout << std::endl;
	    }
            std::cout << std::endl;
	}
    }

    std::cout << std::endl << "gegenbauer" << std::endl;
    for (int n = 0; n <= 5; ++n)
    {
	std::cout << "n=" << n << std::endl;
	for (int k = 0; k <= 200; ++k)
	{
            double x = 0.01 * (k - 100);
            //std::cout << std::endl;
            std::cout << std::setw(12) << x;
	    for (int i = 0; i <= 3; ++i)
	    {
                double alpha = i * 1.0;
        	std::cout << std::setw(12) << gegenbauer(n, alpha, x);
	    }
            std::cout << std::endl;
	}
    }

    std::cout << std::endl << "zernike" << std::endl;
    for (int n = 0; n <= 3; ++n)
    {
	std::cout << "n=" << n << std::endl;
        for (int m = -n; m <= n; ++m)
        {
	    std::cout << "m=" << m << std::endl;
	    for (int k = 0; k <= 20; ++k)
	    {
        	double rho = 0.05 * k;
        	//std::cout << std::endl;
        	std::cout << std::setw(12) << rho;
		for (int i = 0; i <= 36; ++i)
		{
                    double phi = i * 3.141592654 / 180.0;
        	    std::cout << std::setw(12) << zernike(n, m, rho, phi);
		}
        	std::cout << std::endl;
	    }
        }
    }
}

