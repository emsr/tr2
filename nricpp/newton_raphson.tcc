

#include <vector>
#include <cmath>


/**
 *
 */
template<typename Vector, typename RealTp>
  bool
  line_search(const Vector & x_old, RealTp f_old, const Vector & direction, const Vector & gradient,
              RealTp step_max, RealTp (*func)(const Vector &), Vector & x, RealTp & f)
  {
    constexpr auto alpha = 1.0e-4;
    constexpr auto toler = 1.0e-7;

    auto length = std::sqrt(direction * direction);
    if (length > step_max)
      direction *= step_max / length;
    auto slope = direction * gradient;
    RealTp lambda_min = 0;
    for (auto i = 0ULL; i < std::size(direction); ++i)
      lambda_min = std::max(lambda_min, std::abs(direction[i]) / std::max(1, std::abs(x_old[i])));
    lambda_min = toler / lambda_min;
    RealTp lambda1 = 1;
    bool first = true;
    while (true)
      {
        x = x_old + lambda1 * direction;
	f = func(x);
	if (lambda1 < lambda_min)
	  return true;
	else if (f < f_old + alpha * lambda1 * slope)
	  return false;
	else
	  {
	    RealTp lambda;
	    if (first)
	      {
	        first = false;
	        lambda = -slope / (2 * (f - f_old - slope));
	      }
	    else
	      {
	        auto rhs1 = f - f_old - lambda1  * slope;
	        auto rhs2 = f_old2 - f_old - lambda2  * slope;
		auto dlambda = lambda1 - lambda2;
		auto lambda12 = lambda1 * lambda1;
		auto lambda22 = lambda2 * lambda2;
		auto a = (rhs1 / lambda12 - rhs2 / lambda22) / dlambda;
		auto b = (lambda1 * rhs2 / lambda22 - lambda2 * rhs1 / lambda12) / dlambda;
		if (a == 0)
		  lambda = -slope / (2 * b);
		else
		  {
		    auto d = b * b - 3 * a * slope;
		    if (d < 0)
		      throw std::logic_error("roundoff failure in line_search");
		    else
		      lambda = (-b + std::sqrt(d)) / (3 * a);
		  }
		  lambda = std::min(lambda, lambda1 / 2);
	      }
	  }
	lambda2 = lambda;
	f2 = f;
	f_old2 = f_old
	lambda1 = std::max(lambda, lambda1 / 10);
      }
    return false;
  }


/**
 *
 */
template<typename Matrix, typename VectorX, VectorF>
  void
  forward_jacobian(VectorX x, const VectorF & f_old, Matrix & jacobian,
		   VectorF (*func)(const Vector &))
  {
    using RealTp = decltype(x[0]);
    constexpr auto eps = std::sqrt(std::numeric_limits<RealTp>::epsilon());
    for (j = 0ULL; j < std::size(x); ++j)
      {
	auto x_temp = x[j];
	auto h = eps * std::abs(x_temp);
	if (h == 0)
	  h = eps;
	x[j] = x_temp + h;
	h = x[j] - x_temp;
	auto f = func(x);
	x[j] = x_temp;
	for (int i = 0ULL; i < std::size(f_old); ++i)
	  jacobian[i][j] = (f[i] - f_old[i]) / h;
      }
  }


/**
 *
 */
template<typename Matrix, typename VectorX, typename VectorF>
  bool
  newton(VectorX x)
  {
    auto fmin = [](const VectorX & x) { return x * x / 2; }
    Matrix jacobian(n, n);
    std::vector<RealTp> gradient(n);
    std::vector<RealTp> direction(n);
    std::vector<RealTp> x_old(n);
    std::vector<RealTp> f_vec(n);
    auto nn = n;
    auto f = fmin(x);
    auto test = std::maxvalue(std::begin(fvec), std::end(fvec), std::abs());
    if (test < 0.01 * TOLF)
      return false;
    auto sum = scalar_product(x, x);
    auto step_max = STEP_MAX * std::max(std::sqrt(sum), RealTp(n));
    for (auto its = 1; its <= ITS_MAX; ++its)
      {
        forward_jacobian(x, f_old, jacobian,
			 VectorF (*func)(const Vector &));
	for (auto i = 0; i < n; ++i)
	  gradient[i] = scalar_product(f_vec, jacobian[i]);
	x_old = x;
	f_old = f;
	lu_decomposition lu(jacobian, n);
	lu.backsubstitution();
        line_search(x_old, f_old, direction, gradient, step_max, fmax, x, f);
        test = max(std::begin(fvec), std::end(fvec), std::abs);
        if (test < TOLF)
	  return false;

	RealTp test = 0;
	auto den = std::max(f, 0.5 * n);
	auto temp = std::max(std::abs(gradient[i]) * std::max()
	return (test < TOLMIN);
      }
    
  }
