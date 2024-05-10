/*
g++ -std=c++20 -g -Wall -Wextra -o test_monotone_chain test_monotone_chain.cpp
./test_monotone_chain
*/

#include <iostream>

#include "ConvexHull.h"

int
main()
{
  using Tp = long long;

  std::vector<Point<Tp>>
  point
  {
    { 0, 3 },
    { 2, 2 },
    { 1, 1 },
    { 2, 1 },
    { 3, 0 },
    { 0, 0 },
    { 3, 3 }
  };

  auto hull = convex_hull_monotone_chain(point);

  std::cout << "The points in the convex hull are: \n";
  for (auto h : hull)
    std::cout << "(" << h.x << ", " << h.y << ")\n";

  return 0;
}
