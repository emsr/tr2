/*
g++ -std=c++14 -g -Wall -Wextra -o ConvexHullJarvisMarch ConvexHullJarvisMarch.cpp
./ConvexHullJarvisMarch
*/

// A C++ program to find convex hull of a set of points

#include <vector>
#include <iostream>

#include "ConvexHull.h"

/**
 * Return the convex hull of a set of points.
 */
template<typename Tp>
  std::vector<Point<Tp>>
  convex_hull(std::vector<Point<Tp>> point)
  {
    const int n = point.size();

    // There must be at least 3 points.
    if (n < 3)
      return std::vector<Point<Tp>>();

    // Find the lower left-most point.
    int l = 0;
    for (int i = 1; i < n; ++i)
      {
	if (point[i].x < point[l].x)
	  l = i;
	else if (point[i].x == point[l].x && point[i].y < point[l].y)
	  l = i;
      }

    // Start from left-most point, keep moving counterclockwise
    // until reach the start point again.
    std::vector<Point<Tp>> hull;
    int p = l, q;
    do
      {
	hull.push_back(point[p]);

	// Search for a point 'q' such that orientation(p, i, q) is
	// counterclockwise for all points 'i'.
	q = (p + 1) % n;
	for (int i = 0; i < n; ++i)
	  if (orientation(point[p], point[i], point[q]) == counter_clockwise)
            q = i;

	p = q; // Set p as q for next iteration.
      }
    while (p != l);

    return hull;
  }

// Driver program to test above functions.
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

  auto hull = convex_hull(point);

  std::cout << "The points in the convex hull are: \n";
  for (auto h : hull)
    std::cout << "(" << h.x << ", " << h.y << ")\n";

  return 0;
}
