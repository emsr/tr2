// $HOME/bin/bin/g++ -std=c++0x -o test_rice_fading_model test_rice_fading_model.cpp RiceFadingModel.cpp

#include <iostream>
#include <iomanip>
#include <cmath>

#include "RiceFadingModel.h"
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

  RiceFadingModel rice(10.0 * std::log10(2.0), 0.0); // K = 2, A = 1
  for (int i = 0; i < 1000 * per; ++i)
    bin << std::pow(10.0, rice() / 10.0);
  std::cout << "Rice fading" << std::endl;
  std::cout << "Mean: " << bin.mean() << std::endl;
  plot_histogram(bin, per);
  bin.reset();

  //  Test Rayleigh fading...
  RiceFadingModel rayleigh(-200.0, 10.0 * std::log10(2.0)); // K = 0, A = 2
  for (int i = 0; i < 1000 * per; ++i)
    bin << std::pow(10.0, rayleigh() / 10.0);
  std::cout << "Rayleigh fading" << std::endl;
  std::cout << "Mean: " << bin.mean() << std::endl;
  plot_histogram(bin, per);
  bin.reset();

  //  Test Rayleigh fading...
  RiceFadingModel rayleigh2(-200.0, 10.0 * std::log10(8.0)); // K = 0, A = 8
  for (int i = 0; i < 1000 * per; ++i)
    bin << std::pow(10.0, rayleigh2() / 10.0);
  std::cout << "Rayleigh fading" << std::endl;
  std::cout << "Mean: " << bin.mean() << std::endl;
  plot_histogram(bin, per);
}
