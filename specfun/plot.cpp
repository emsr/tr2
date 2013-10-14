// /home/ed/bin/bin/g++ -std=c++11 -o plot plot.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./plot

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "cmath"

template<typename Num>
  void
  plot_func(std::ostream & out,
            std::function<Num (Num)> funk,
            Num x1, Num x2,
            const std::string & t1, const std::string & t2,
            const std::string & tx, const std::string & ty)
  {
    using len_t = decltype(t1.length());
    const len_t ISCREEN = 101;
    const len_t JSCREEN = 42;
    const char CORNER = '+';
    const char BLANK = ' ';
    const char XZERO = '.';
    const char YZERO = '-';
    const char YBORDER = '|';
    const char XBORDER = '-';
    const char FUNCPT = 'x';

    std::vector<std::string> screen(ISCREEN, std::string(JSCREEN, BLANK));
    std::string title1(ISCREEN, BLANK);
    std::string title2(ISCREEN, BLANK);
    std::string titlex(ISCREEN, BLANK);
    std::string titley(JSCREEN, BLANK);

    //
    //  Massage titles.
    //
    int len1 = std::min(t1.length(), ISCREEN);
    for (int j = 0; j < len1; ++j)
      title1[static_cast<int>((ISCREEN - len1)/2.0) + j] = t1[j];

    int len2 = std::min(t2.length(), ISCREEN);
    for (int j = 0; j < len2; ++j)
      title2[static_cast<int>((ISCREEN - len2)/2.0) + j] = t2[j];

    int lenx = std::min(tx.length(), ISCREEN);
    for (int j = 0; j < lenx; ++j)
      titlex[static_cast<int>((ISCREEN - lenx)/2.0) + j] = tx[j];

    int leny = std::min(ty.length(), JSCREEN);
    for (int j = 0; j < leny; ++j)
      titley.at(static_cast<int>((JSCREEN + leny)/2.0) - j) = ty.at(j);

    screen[0][0] = screen[0][JSCREEN - 1] = screen[ISCREEN - 1][0] = screen[ISCREEN - 1][JSCREEN - 1] = CORNER;

    for(int j = 1; j < JSCREEN - 1; ++j)
      screen[0][j] = screen[ISCREEN - 1][j] = YBORDER;

    for(int i = 1; i < ISCREEN - 1; ++i)
      screen[i][0] = screen[i][JSCREEN - 1] = XBORDER;

    //
    //  Evaluate the function over the requested interval.
    //  Keep track of the maximum and minimum values of the function.
    //  With this logic, the interval from ysml to ybig will always
    //  include the x axis (y == 0).
    //
    Num dx = (x2 - x1) / (ISCREEN - 1);
    Num x = x1;
    int izero = -1;
    if (x1 < 0.0 && x2 > 0.0)
      {
        izero = -(ISCREEN * x1) / (x2 - x1);
        for (int j = 1; j < JSCREEN - 1; ++j)
          screen.at(izero).at(j) = XZERO;
      }
    Num ysml = std::numeric_limits<Num>::max();
    Num ybig = -std::numeric_limits<Num>::max();
    std::vector<Num> y(ISCREEN);
    for (int i = 0; i < ISCREEN; ++i)
      {
        y[i] = funk(x);
        if (y[i] < ysml)
          ysml = y[i];
        if (y[i] > ybig)
          ybig = y[i];
        x += dx;
      }
    if (ysml > 0.0)
      ysml = 0.0;
    if (ybig < 0.0)
      ybig = 0.0;
    if (ybig == ysml)
      {
        ybig += 0.5;
        ybig -= 0.5;
      }
    Num djy = (JSCREEN - 1) / (ybig - ysml);
    int jzero = static_cast<int>(-ysml * djy);
    for (int i = 1; i < ISCREEN - 1; ++i)
      {
        screen.at(i).at(jzero) = YZERO;
        int j = static_cast<int>((y[i] - ysml) * djy);
        screen.at(i).at(j) = FUNCPT;
      }

    auto old_prec = out.precision(4);
    auto old_flags = out.flags(out.flags() | std::ios::showpoint);

    //
    //  Print title(s)
    //
    if (len1 > 0)
      {
        out << std::endl;
        if (leny > 0)
          out << "   ";
        for (int i = 0; i < 12; ++i)
          out << BLANK;
        out << title1 << std::endl;
      }
    if (len2 > 0)
      {
        out << std::endl;
        if (leny > 0)
          out << "   ";
        for (int i = 0; i < 12; ++i)
          out << BLANK;
        out << title2 << std::endl;
      }
    out << std::endl;

    //
    //  Print upper limit and top line.
    //
    if (leny > 0)
      out << ' ' << titley[JSCREEN - 1] << ' ';
    out << ' ' << std::setw(10) << std::showpos << ybig << ' ';
    for (int i = 0; i < ISCREEN; ++i)
      out << screen[i][JSCREEN - 1];
    out << std::endl;

    //
    //  Print graph.
    //
    for (int j = JSCREEN - 2; j >= 1; --j)
      {
        if (leny > 0)
          out << ' ' << titley[j] << ' ';
        if (j == jzero)
          out << ' ' << std::setw(10) << std::showpos << 0.0 << ' ';
        else
          for (int i = 0; i < 12; ++i)
            out << BLANK;
        for (int i = 0; i < ISCREEN; ++i)
          out << screen[i][j];
        out << std::endl;
      }

    //
    //  Print lower limit and bottom line.
    //
    if (leny > 0)
      out << ' ' << titley[0] << ' ';
    out << ' ' << std::setw(10) << std::showpos << ysml << ' ';
    for (int i = 0; i < ISCREEN; ++i)
      out << screen[i][0];
    out << std::endl;

    //
    //  Print lower and upper x limits.
    //
    if (leny > 0)
      out << "   ";
    out << "      "
        << std::setw(10) << std::showpos << x1;
    if (izero > -1)
      {
        out << std::setw(izero - 10) << ""
            << std::setw(10) << 0.0
            << std::setw(ISCREEN - 11 - izero) << "";
      }
    else
      out << std::setw(ISCREEN - 11) << "";
    out << std::setw(10) << std::showpos << x2 << std::endl;

    if (lenx > 0)
      {
        out << std::endl;
        if (leny > 0)
          out << "   ";
        for (int i = 0; i < 12; ++i)
          out << BLANK;
        out << titlex << std::endl;
      }
    out << std::endl;

    out.flags(old_flags);
    out.precision(old_prec);
  }

#include <iostream>
#include <functional>
using namespace std::placeholders;

int
main()
{
  plot_func<double>(std::cout, [](double x){ return x; },
          -3.0, 3.0,
          "y = x", "Hello, World!",
          "x", "x^2");

  plot_func<double>(std::cout, [](double x){ return x * x; },
          -3.0, 3.0,
          "y = x^2", "Hello, World!",
          "x", "x^2");

  plot_func<double>(std::cout, sinc<double>, -10.0, 10.0, "y = sinc(x)", "sinc(x) = sin(x)/x", "x", "sinc(x)");

  plot_func<double>(std::cout, logint<double>, 0.0, 5.0, "y = logint(x)", "logint(x)", "x", "logint(x)");

  plot_func<double>(std::cout, cosint<double>, -10.0, 10.0, "y = cosint(x)", "cosint(x)", "x", "cosint(x)");

  plot_func<double>(std::cout, sinint<double>, -10.0, 10.0, "y = sinint(x)", "sinint(x)", "x", "sinint(x)");

  plot_func<double>(std::cout, coshint<double>, -5.0, 5.0, "y = coshint(x)", "coshint(x)", "x", "coshint(x)");

  plot_func<double>(std::cout, sinhint<double>, -5.0, 5.0, "y = sinhint(x)", "sinhint(x)", "x", "sinhint(x)");

  using jacobi_t = double(*)(double, double);
  for (int j = 0; j <= 5; ++j)
  {
    double k = j * 1.0;
    std::ostringstream fname{};
    fname << "jacobi_cn(" << k << ",u)";
    auto jacobi = std::bind((jacobi_t)jacobi_cn<double>, k, _1);
    plot_func<double>(std::cout, jacobi, -1.0, 1.0, "y = " + fname.str(), fname.str(), "u", fname.str());
  }

  for (int j = 0; j <= 5; ++j)
  {
    double k = j * 1.0;
    std::ostringstream fname{};
    fname << "jacobi_sn(" << k << ",u)";
    auto jacobi = std::bind((jacobi_t)jacobi_sn<double>, k, _1);
    plot_func<double>(std::cout, jacobi, -1.0, 1.0, "y = " + fname.str(), fname.str(), "u", fname.str());
  }

  for (int j = 0; j <= 5; ++j)
  {
    double k = j * 1.0;
    std::ostringstream fname{};
    fname << "jacobi_dn(" << k << ",u)";
    auto jacobi = std::bind((jacobi_t)jacobi_dn<double>, k, _1);
    plot_func<double>(std::cout, jacobi, -1.0, 1.0, "y = " + fname.str(), fname.str(), "u", fname.str());
  }

  plot_func<double>(std::cout, fresnel_c<double>, -5.0, 5.0, "y = fresnel_c(x)", "fresnel_c(x)", "x", "fresnel_c(x)");

  plot_func<double>(std::cout, fresnel_s<double>, -5.0, 5.0, "y = fresnel_s(x)", "fresnel_s(x)", "x", "fresnel_s(x)");

  plot_func<double>(std::cout, airy_ai<double>, -5.0, 5.0, "y = airy_ai(x)", "airy_ai(x)", "x", "airy_ai(x)");

  plot_func<double>(std::cout, airy_bi<double>, -5.0, 5.0, "y = airy_bi(x)", "airy_bi(x)", "x", "airy_bi(x)");

  using cheb_t = double(*)(unsigned int, double);
  for (unsigned int n : {0, 1, 2, 5})
  {
    std::ostringstream fname{};
    fname << "chebyshev_t(" << n << ",x)";
    auto chebyshev = std::bind((cheb_t)chebyshev_t<double>, n, _1);
    plot_func<double>(std::cout, chebyshev, -1.0, 1.0, "y = " + fname.str(), fname.str(), "x", fname.str());
  }

  for (unsigned int n : {0, 1, 2, 5})
  {
    std::ostringstream fname{};
    fname << "chebyshev_u(" << n << ",x)";
    auto chebyshev = std::bind((cheb_t)chebyshev_u<double>, n, _1);
    plot_func<double>(std::cout, chebyshev, -1.0, 1.0, "y = " + fname.str(), fname.str(), "x", fname.str());
  }

  for (unsigned int n : {0, 1, 2, 5})
  {
    std::ostringstream fname{};
    fname << "chebyshev_v(" << n << ",x)";
    auto chebyshev = std::bind((cheb_t)chebyshev_v<double>, n, _1);
    plot_func<double>(std::cout, chebyshev, -1.0, 1.0, "y = " + fname.str(), fname.str(), "x", fname.str());
  }

  for (unsigned int n : {0, 1, 2, 5})
  {
    std::ostringstream fname{};
    fname << "chebyshev_w(" << n << ",x)";
    auto chebyshev = std::bind((cheb_t)chebyshev_w<double>, n, _1);
    plot_func<double>(std::cout, chebyshev, -1.0, 1.0, "y = " + fname.str(), fname.str(), "x", fname.str());
  }

  using gamma_t = double(*)(double, double);
  for (int j = 1; j <= 5; ++j)
  {
    double a = j * 0.5;
    std::ostringstream fname{};
    fname << "gamma_l(" << a << ",x)";
    auto gam_l = std::bind((gamma_t)gamma_l<double>, a, _1);
    plot_func<double>(std::cout, gam_l, 0.0, 2.0, "y = " + fname.str(), fname.str(), "x", fname.str());
  }

  for (int j = 1; j <= 5; ++j)
  {
    double a = j * 0.5;
    std::ostringstream fname{};
    fname << "gamma_u(" << a << ",x)";
    auto gam_u = std::bind((gamma_t)gamma_u<double>, a, _1);
    plot_func<double>(std::cout, gam_u, 0.0, 2.0, "y = " + fname.str(), fname.str(), "x", fname.str());
  }

  for (int j = 1; j <= 5; ++j)
  {
    double a = j * 0.5;
    std::ostringstream fname{};
    fname << "pochhammer_l(" << a << ",x)";
    auto poch_l = std::bind((gamma_t)pochhammer_l<double>, a, _1);
    plot_func<double>(std::cout, poch_l, 0.0, 2.0, "y = " + fname.str(), fname.str(), "x", fname.str());
  }

  for (int j = 1; j <= 5; ++j)
  {
    double a = j * 0.5;
    std::ostringstream fname{};
    fname << "pochhammer_u(" << a << ",x)";
    auto poch_u = std::bind((gamma_t)pochhammer_u<double>, a, _1);
    plot_func<double>(std::cout, poch_u, 0.0, 2.0, "y = " + fname.str(), fname.str(), "x", fname.str());
  }
}

