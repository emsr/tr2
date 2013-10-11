// /home/ed/bin/bin/g++ -std=c++11 -o plot plot.cpp

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <iomanip>

void
plot_func(std::ostream & out,
          std::function<double (double)> funk,
          double x1, double x2,
          const std::string & t1, const std::string & t2,
          const std::string & tx, const std::string & ty)
{
    using len_t = decltype(t1.length());
    const len_t ISCREEN = 100;
    const len_t JSCREEN = 41;
    const char PLUS = '+';
    const char BLANK = ' ';
    const char ZERO = '-';
    const char YY = '|';
    const char XX = '-';
    const char FF = 'x';

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
      titley.at(static_cast<int>((JSCREEN - leny)/2.0) - j) = ty.at(j);

    screen[0][0] = screen[0][JSCREEN - 1] = screen[ISCREEN - 1][0] = screen[ISCREEN - 1][JSCREEN - 1] = PLUS;

    for(int j = 1; j < JSCREEN - 1; ++j)
      screen[0][j] = screen[ISCREEN - 1][j] = YY;

    for(int i = 1; i < ISCREEN - 1; ++i)
      screen[i][0] = screen[i][JSCREEN - 1] = XX;

    //
    //  Evaluate the function over the requested interval.
    //  Keep track of the maximum and minimum values of the function.
    //  With this logic, the interval from ysml to ybig will always
    //  include the x axis (y == 0).
    //
    double dx = (x2 - x1) / (ISCREEN - 1);
    double x = x1;
    double ysml = 0.0;
    double ybig = 0.0;
    std::vector<double> y(ISCREEN);
    for (int i = 0; i < ISCREEN; ++i)
      {
        y[i] = funk(x);
        if (y[i] < ysml)
          ysml = y[i];
        if (y[i] > ybig)
          ybig = y[i];
        x += dx;
      }
    if (ybig == ysml)
      ybig = ysml + 1.0;
    double dyj = (JSCREEN - 1) / (ybig - ysml);
    int jz = static_cast<int>(ysml * dyj);
    for (int i = 1; i < ISCREEN - 1; ++i)
      {
        screen.at(i).at(jz) = ZERO;
        int j = static_cast<int>((y[i] - ysml) * dyj);
        screen.at(i).at(j) = FF;
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
          out << std::setw(3) << "";
        for (int i = 0; i < 12; ++i)
          out << BLANK;
        out << title1 << std::endl;
      }
    if (len2 > 0)
      {
        out << std::endl;
        if (leny > 0)
          out << std::setw(3) << "";
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
      out << std::setw(3) << "";
    out << "      "
        << std::setw(10) << std::showpos << x1
        << std::setw(ISCREEN - 9) << ""
        << std::setw(10) << std::showpos << x2 << std::endl;

    if (lenx > 0)
      {
        out << std::endl;
        if (leny > 0)
          out << std::setw(3) << "";
        for (int i = 0; i < 12; ++i)
          out << BLANK;
        out << titlex << std::endl;
      }
    out << std::endl;

    out.flags(old_flags);
    out.precision(old_prec);
}

#include <iostream>

int
main()
{
  plot_func(std::cout, [](double x){ return x * x; },
          -3.0, 3.0,
          "y = x^2", "Hello, World!",
          "x", "y = x^2");
}
