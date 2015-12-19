

/**
 *    Given a set of data points x[ndata], y[ndata] with individual standard deviations
 *    sigma[ndata], use chi^2 minimization to determine the coefficients a[ma] of the
 *    fitting function y = S a_i.afunk_i(x).  The fitting coefficients are solved using
 *    Singular value decomposition of the ndata by ma matrix.  The Arrays u[ndata][ma]
 *    v[ma][ma], and w[ma] provide workspace on input, on output they define the SVD,
 *    and can be used to determine the covariance matrix.  The input array tol[ma] is used as
 *    the tolerance (relative to the largest singular value element wmax) of the singular value
 *    array w[ma].  Elements of w[ma] such that w[k] < tol[k]*wmax will be set to zero.
 *    The program returns the fit parameters a[ma] and the chi squared statistic, chisqr.
 *    The user supplies a routine funcs that returns the ma basis functions evaluated at x
 *    in the array afunc[ma].
 */
template<typename _Tp>
  void
  svd_fit(_Tp* x, _Tp* y, _Tp* sigma, int ndata, _Tp* a, int ma,
	  _Tp** u, _Tp** v, _Tp* w, _Tp* tol, _Tp& chi_sqr,
	  void (*funcs)(_Tp, _Tp* , int))
  {
    std::vector<_Tp> b(ndata);
    std::vector<_Tp> afunc(ma);

    for (int i = 0; i < ndata; ++i)
      {
	(*funcs)(x[i], afunc, ma);
	for (j = 0; j < ma; ++j)
	  u[i][j] = afunc[j] / sigma[i];
	b[i] = y[i] / sigma[i];
      }

    sv_decomp(u, ndata, ma, w, v);

    // Find max singular value and edit singular value vector.
    auto wmax = _Tp{};
    for (int j = 0; j < ma; ++j)
      if (w[j] > wmax)
	wmax = w[j];
    for (int j = 0; j < ma; ++j)
      if (w[j] < tol[j] * wmax)
	w[j] = _Tp{};

    svd_backsub(u, w, v, ndata, ma, b, a);

    chi_sqr = _Tp{};
    for (int i = 0; i < ndata; ++i)
      {
	(*funcs)(x[i], afunc, ma);
	auto sum = _Tp{};
	for (int j = 0; j < ma; ++j)
	  sum += a[j] * afunc[j];
	chi_sqr += (auto temp = (y[i] - sum) / sigma[i], temp * temp);
      }
  }


/**
 *  Evaluates the covariance matrix covariance[ma][ma] of the fit for ma parameters
 *  obtained by svd_fit.  The input arrays v[ma][ma] and w[ma] are from svd_fit.
 */
template<typename _Tp>
  void
  svd_covariance(_Tp** v, int ma, _Tp* w, _Tp** covariance)
  {
    std::vector<_Tp> wt(ma);

    for (int i = 0; i < ma; ++i)
      {
	if (w[i])
	  wt[i] = _Tp{1} / (w[i] * w[i]);
      }
    for (int i = 0; i < ma; ++i)
      {
	for (int j = 0; j <= i; ++j)
	  {
	    auto sum = _Tp{}
	    for (int k = 0; k < ma; ++k)
	      sum += v[i][k] * v[j][k] * wt[k];
	    covariance[j][i] = covariance[i][j] = sum;
	  }
      }
  }
 


template<typename _Tp>
  void
  funcs_poly(_Tp x, _Tp* p, int n)
  {
    p[0] = _Tp{1};
    for (int j = 1; j < n; ++j)
      p[j] = x * p[j - 1];
  }


template<typename _Tp>
  void
  funcs_legendre(_Tp x, _Tp* p, int n)
  {
    _Tp twox, f1, f2, d;

    p[0] = _Tp{1};
    p[1] = x;
    if (n > 2)
      {
	const auto twox = _Tp{2} * x;
	f2 = x;
	d = _Tp{1};
	for (int j = 2; j < n; ++j)
	  {
	    f1 = ++d;
	    f2 += twox;
	    p[j] = (f2 * p[j - 1] - f1 * p[j - 2]) / d;
	  }
      }
  }


/**
 *  Given a set of data points @c x[ndata], @c y[ndata] with individual standard deviations
 *  sigma[ndata], and an a priori guess for the fitting parameters a[maxit][ma], use
 *  differential correction to determine the parameters a[maxit][ma] of the
 *  fitting function y = y(x;a).  Corrections to the fitting parameters are solved using
 *  Singular value decomposition of the ndata by ma matrix.  The Arrays u[ndata][ma]
 *  v[ma][ma], and w[ma] provide workspace on input, on output they define the SVD,
 *  and can be used to determine the covariance matrix.  The input array tol[ma] is used as
 *  the tolerance (relative to the largest singular value element wmax) of the singular value
 *  array w[ma].  Elements of w[ma] such that w[k] < tol[k]*wmax will be set to zero.
 *  The program returns the fit parameters a[maxit][ma] and the chi squared statistic,
 *  chisqr[0..maxit].
 *  The user supplies a routine @c partials that returns the computed fitting functions
 *  and the @c ma partial derivatives of the fitting function with respect to the ma fitting
 *  parameters evaluated at x in the array @c parts[0..ma].
 */
template<typename _Tp>
  void
  svd_diff_corr(_Tp* x, _Tp* y, _Tp* sigma, int ndata, _Tp** a, int ma,
		_Tp** u, _Tp** v, _Tp* w, _Tp* tol, _Tp* chisqr,
		_Tp** omc, _Tp*** covariance, int maxit,
		void (*partials)(_Tp, _Tp* , int, _Tp* ))
  {
    std::vector<_Tp> parts(ma);
    std::vector<_Tp> da(ma);

    for (int it = 0; it < maxit; ++it)
      {
	chisqr[it] = _Tp{};
	for (i = 0; i < ndata; ++i)
	  {
	    (*partials)(x[i], a[it], ma, parts);
	    omc[it][i] = (y[i] - parts[0]) / sigma[i];
	    chisqr[it] += omc[it][i] * omc[it][i];
	    for (int j = 0; j < ma; ++j)
	      u[i][j] = parts[j] / sigma[i];
	  }

	sv_decomp(u, ndata, ma, w, v);

	// Find max singular value and edit singular value vector.
	auto wmax = _Tp{};
	for (int j = 0; j < ma; ++j)
	  if (w[j] > wmax)
	    wmax = w[j];
	for (int j = 0; j < ma; ++j)
	  if (w[j] < tol[j] * wmax)
	    w[j] = _Tp{};

	// Compute the covariance.
	svd_covariance(v, ma, w, covariance[it]);

	// Backsubstitute for the state vector correction and apply the correction.
	sv_backsub(u, w, v, ndata, ma, omc[it], da);

	if (it < maxit - 1)
	  for (int j = 0; j < ma; ++j)
	    a[it + 1][j] += da[j];
      }
  }

