
#include <iostream>
#include <iomanip>

#include "RiceFadingModel.h"
#include "NakagamiFadingModel.h"

int
main()
{
  //  Test Rice fading...
  RiceFadingModel rice(2.0, 1.0);
  std::cout << "Rice fading - Rice(2.0, 1.0)" << std::endl;
  for (int i = 0; i < 100; ++i)
    std::cout << std::setw(8) << rice() << std::endl;

  //  Test Rayleigh fading...
  RiceFadingModel rayleigh(0.0, 1.0);
  std::cout << "Rayleigh fading Rayleigh(1.0)" << std::endl;
  for (int i = 0; i < 100; ++i)
    std::cout << std::setw(8) << rayleigh() << std::endl;

  //  Test Nakagami fading...
  RiceFadingModel nakagami(2.0, 1.0);
  std::cout << "Nakagami fading - Nakagami(2.0, 1.0)" << std::endl;
  for (int i = 0; i < 100; ++i)
    std::cout << std::setw(8) << nakagami() << std::endl;
}
