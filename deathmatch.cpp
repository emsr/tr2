// /home/ed/bin/bin/g++ -std=c++1z -o deathmatch deathmatch.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./deathmatch 100000

#include "uniform_inside_sphere_distribution.h"
#include "uniform_inside_sphere_distribution_old.h"
#include "timer.h"

#include <iostream>

template <std::size_t _Dim>
  void
  test(double radius = 1.0, int num = 1000000)
  {
    Timer timer;

    std::default_random_engine re; // the default engine

    __gnu_cxx::uniform_inside_sphere_distribution<_Dim> insphnew(radius);

    timer.start();
    for (int i = 0; i < num; ++i)
      insphnew(re);
    timer.stop();

    std::cout << std::endl << "  New (Rejection) Distribution - Dim=" << _Dim;
    std::cout << std::endl << "  Total time per iter = "
              << 1.0 * timer.time_elapsed() << " ms";
    std::cout << std::endl << "  Mean time per iter  = "
              << 1.0 * timer.time_elapsed() / num << " ms" << std::endl;

    __old::uniform_inside_sphere_distribution<_Dim> insphold(radius);

    timer.start();
    for (int i = 0; i < num; ++i)
      insphold(re);
    timer.stop();

    std::cout << std::endl << "  Old (Transform) Distribution - Dim=" << _Dim;
    std::cout << std::endl << "  Total time per iter = "
              << 1.0 * timer.time_elapsed() << " ms";
    std::cout << std::endl << "  Mean time per iter  = "
              << 1.0 * timer.time_elapsed() / num << " ms" << std::endl;
  }

int
main(int n_app_args, char ** app_arg)
{
  int num = 10000;
  if (n_app_args > 1)
    num = atoi(app_arg[1]);

  //  Segment

  test<1>(1.0, num);

  //  Circle

  test<2>(2.0, num);

  //  Sphere

  test<3>(3.0, num);

  //  Hypersphere

  test<4>(4.0, num);

  //  Etc...

  test<5>(5.0, num);

  test<6>(6.0, num);

  test<7>(7.0, num);

  test<8>(8.0, num);

  test<9>(9.0, num);
}
