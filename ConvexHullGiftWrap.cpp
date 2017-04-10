/*
g++ -Wall -Wextra -o ConvexHullGiftWrap ConvexHullGiftWrap.cpp
./ConvexHullGiftWrap

g++ -std=c++14 -Wall -Wextra -o ConvexHullGiftWrap ConvexHullGiftWrap.cpp
./ConvexHullGiftWrap
*/

// A C++ program to find convex hull of a set of points

#include <vector>
#include <iostream>

/**
 * A two-dimensional point.
 */
template<typename Tp>
  struct Point
  {
    Tp x;
    Tp y;
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
 * An enumeration of the relative orientation of a sequence of three points.
 */
enum Orientation
{
  clockwise,
  collinear,
  counter_clockwise,
};

/**
 * Return the orientation of an ordered triplet (p, q, r) or points.
 */
template<typename Tp>
  Orientation
  orientation(Point<Tp> p, Point<Tp> q, Point<Tp> r)
  {
    auto val = cross(p, q, r);

    if (val == 0)
      return collinear;
    else
      return (val > 0) ? counter_clockwise : clockwise;
  }

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
