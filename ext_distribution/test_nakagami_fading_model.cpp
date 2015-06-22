// $HOME/bin/bin/g++ -std=c++0x -o test_nakagami_fading_model test_nakagami_fading_model.cpp NakagamiFadingModel.cpp

#include <iostream>
#include <iomanip>

#include "NakagamiFadingModel.h"
#include "histogram.h"

void
plot_histogram(const histogram<double> & bin, std::size_t per = 1)
{
  for (std::size_t i = 1; i <= bin.size(); ++i)
  {
    std::cout << bin.value(i) << '\t';
    for (std::size_t j = 0; j < bin[i]; j += per)
      std::cout << '*';
    std::cout << std::endl;
  }
}

int
main()
{
  histogram<double> bin(100, 0.0, 10.0);
  std::size_t per = 100;

  NakagamiFadingModel nakagami(2.0, 10.0 * std::log10(12.0));
  std::cout << "Nakagami fading" << std::endl;
  for (int i = 0; i < 1000 * per; ++i)
    bin << std::pow(10.0, nakagami() / 10.0);
  std::cout << "Mean: " << bin.mean() << std::endl;
  plot_histogram(bin, per);
  bin.reset();

  //  Test Rayleigh fading...
  NakagamiFadingModel rayleigh0(1.0, 10.0 * std::log10(1.0));
  std::cout << "Rayleigh fading" << std::endl;
  for (int i = 0; i < 1000 * per; ++i)
    bin << std::pow(10.0, rayleigh0() / 10.0);
  std::cout << "Mean: " << bin.mean() << std::endl;
  plot_histogram(bin, per);
  bin.reset();

  //  Test Rayleigh fading...
  NakagamiFadingModel rayleigh(1.0, 10.0 * std::log10(2.0));
  std::cout << "Rayleigh fading" << std::endl;
  for (int i = 0; i < 1000 * per; ++i)
    bin << std::pow(10.0, rayleigh() / 10.0);
  std::cout << "Mean: " << bin.mean() << std::endl;
  plot_histogram(bin, per);
  bin.reset();

  //  Test Rayleigh fading...
  NakagamiFadingModel rayleigh2(1.0, 10.0 * std::log10(8.0));
  std::cout << "Rayleigh fading" << std::endl;
  for (int i = 0; i < 1000 * per; ++i)
    bin << std::pow(10.0, rayleigh2() / 10.0);
  std::cout << "Mean: " << bin.mean() << std::endl;
  plot_histogram(bin, per);
}
