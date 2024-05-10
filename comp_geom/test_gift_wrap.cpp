/*
g++ -std=c++20 -g -Wall -Wextra -o test_gift_wrap test_gift_wrap.cpp
./test_gift_wrap
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

  auto hull = convex_hull_gift_wrap(point);

  std::cout << "The points in the convex hull are: \n";
  for (auto h : hull)
    std::cout << "(" << h.x << ", " << h.y << ")\n";

  return 0;
}
