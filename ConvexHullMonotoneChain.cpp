/*
g++ -Wall -Wextra -o ConvexHullMonotoneChain ConvexHullMonotoneChain.cpp
./ConvexHullMonotoneChain

g++ -std=c++14 -Wall -Wextra -o ConvexHullMonotoneChain ConvexHullMonotoneChain.cpp
./ConvexHullMonotoneChain
*/

#include <vector>
#include <algorithm>
#include <iostream>

/**
 * A two-dimensional point with lexicographical order.
 */
template<typename Tp>
  struct Point
  {
    Tp x;
    Tp y;

    bool
    operator<(const Point& p) const
    { return x < p.x || (x == p.x && y < p.y); }
  };

/**
 * Return square of distance between p1 and p2.
 */
template<typename Tp>
  Tp
  dist(Point<Tp> p1, Point<Tp> p2)
  { return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y); }

/**
 * Return the 2D cross product of p0->p1 and p0->p2 vectors,
 * i.e. the z-component of their 3D cross product.
 * Returns a positive value, if p0->p1->p2 makes a counter-clockwise turn,
 * negative for clockwise turn, and zero if the points are collinear.
 */
template<typename Tp>
  Tp
  cross(Point<Tp> p0, Point<Tp> p1, Point<Tp> p2)
  { return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x); }

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
