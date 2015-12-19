/*
 *  Fit data to a straight line. NRiC pp 523-528.
 */

template<typename _Tp>
  void
  linear_fit(const _Tp *x, const _Tp *y, int ndata,
	     const _Tp *sigma, bool mwt,
             _Tp& a, _Tp& b, _Tp& sigma_a, _Tp& sigma_b,
             _Tp& chi_sqr, _Tp& q)
  {
    _Tp sum_x = _Tp{}, sum_y = _Tp{}, sum_tsqr = _Tp{}, sum_weight;

    b = _Tp{0};
    if (mwt)
      {
        // Accumulate sums.
        sum_weight = _Tp{};
        for (int i = 0; i < ndata; ++i)
	  {
            auto wt = _Tp{1} / (sigma[i] * sigma[i]);
            sum_weight += wt;
            sum_x += x[i] * wt;
            sum_y += y[i] * wt;
          }
      }
    else
      {
        sum_weight = ndata;
        for (int i = 0; i < ndata; ++i)
	  {
            sum_x += x[i];
            sum_y += y[i];
          }
      }
    _Tp mean_x = sum_x / sum_weight;

    if (mwt)
      {
        for (int i = 0; i < ndata; ++i)
	  {
            auto t = (x[i] - mean_x) / sigma[i];
            sum_tsqr += t * t;
            b += t * y[i] / sigma[i];
          }
      }
    else
      {
        for (int i = 0; i < ndata; ++i)
	  {
            auto t = x[i] - mean_x;
            sum_tsqr += t * t;
            b += t * y[i];
          }
      }
    b /= sum_tsqr;
    a = (sum_y - sum_x * b) / sum_weight;
    sigma_a = std::sqrt((_Tp{1} + sum_x * sum_x / (sum_weight * sum_tsqr)) / sum_weight);
    sigma_b = std::sqrt(_Tp{1} / sum_tsqr);
    chi_sqr = _Tp{};

    if (!mwt)
      {
        for (int i = 0; i < ndata; ++i)
	  chi_sqr += (auto temp = y[i] - a - b * x[i], temp * temp);
        q = _Tp{1};
        auto sig_data = std::sqrt(chi_sqr / (ndata - 2));
        sigma_a *= sig_data;
        sigma_b *= sig_data;
      }
    else
      {
        for (int i = 0; i < ndata; ++i)
	  chi_sqr += (auto temp = y[i] - a - b * x[i], temp * temp) / sigma[i];
        q = __gnu_cxx::gamma_q(0.5 * (ndata - 2), 0.5 * chi_sqr);
      }
  }
