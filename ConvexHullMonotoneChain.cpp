/*
g++ -std=c++14 -g -Wall -Wextra -o ConvexHullMonotoneChain ConvexHullMonotoneChain.cpp
./ConvexHullMonotoneChain
*/

#include <vector>
#include <algorithm>
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

    std::vector<Point<Tp>> hull(2 * n);

    // Sort points lexicographically.
    std::sort(point.begin(), point.end());

    // Test both branches.
    bool orig_direction = true;
    if (orig_direction)
      {
	int k = 0;

	// Build lower hull.
	for (int i = 0; i < n; ++i)
	  {
	    while (k >= 2 && cross(hull[k - 2], hull[k - 1], point[i]) <= 0)
	      --k;
	    hull[k++] = point[i];
	  }

	// Build upper hull.
	// Skip both endopints to avoid duplicating hull points.
	for (int i = n - 2, t = k + 1; i > 0; i--)
	  {
	    while (k >= t && cross(hull[k - 2], hull[k - 1], point[i]) <= 0)
	      --k;
	    hull[k++] = point[i];
	  }

	hull.resize(k);
	return hull;
      }
    else
      {
	int k = 0;

	// Build upper hull.
	for (int i = n - 1; i >= 0; i--)
	  {
	    while (k >= 2 && cross(hull[k - 2], hull[k - 1], point[i]) <= 0)
	      --k;
	    hull[k++] = point[i];
	  }

	// Build lower hull.
	// Skip both endopints to avoid duplicating hull points.
	for (int i = 1, t = k + 1; i < n - 1; ++i)
	  {
	    while (k >= t && cross(hull[k - 2], hull[k - 1], point[i]) <= 0)
	      --k;
	    hull[k++] = point[i];
	  }

	hull.resize(k);
	return hull;
      }
  }

// Driver program to test above functions
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
