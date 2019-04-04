//  Improved Complex Division
//  Michael Baudin (DIGITEO)
//  Robert Smith (Stanford University)

#include <complex>

std::complex<Tp>
  compdiv_improved(const std::complex<Tp> & x, const std::complex<Tp> & y)
  {
    a = std::real(x);
    b = std::imag(x);
    c = std::real(y);
    d = std::imag(y);
    if (std::abs(d) <= std::abs(c))
      {
	r = d / c;
	t = 1 / (c + d * r);
	if (r == 0)
	  {
            e = t * (a + d * (b / c));
            f = t * (b - d * (a / c));
	  }
	else
	  {
            e = t * (a + r * b);
            f = t * (b - r * a);
	  }
      }
    else
      {
	r = c / d;
	t = 1 / (c * r + d);
	if (r == 0)
	  {
	    e = t * (c * (a / d) + b);
	    f = t * (c * (b / d) - a);
	  }
	else
	  {
	    e = t * (r * a + b);
	    f = t * (r * b - a);
	  }
      }
    return std::complex<Tp>{e, f};
  }
