/*
g++ -o ConvexHullGiftWrap ConvexHullGiftWrap.cpp
./ConvexHullGiftWrap
*/

// http://www.sanfoundry.com/cpp-programming-examples-computational-geometry-problems-algorithms/

// A C++ program to find convex hull of a set of points
// Refer http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// for explanation of orientation()

#include <vector>
#include <iostream>

template<typename Tp>
  struct Point
  {
    int x;
    int y;
  };

// A utility function to return square of distance between p1 and p2
template<typename Tp>
  Tp
  dist(Point<Tp> p1, Point<Tp> p2)
  { return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y); }

// 2D cross product of OA and OB vectors,
// i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
template<typename Tp>
  Tp
  cross(Point<Tp> p0, Point<Tp> p1, Point<Tp> p2)
  { return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x); }

enum Orientation
{
  clockwise,
  collinear,
  counter_clockwise,
};

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
template<typename Tp>
  Orientation
  orientation(Point<Tp> p, Point<Tp> q, Point<Tp> r)
  {
    auto val = cross(p, q, r);

    if (val == 0)
      return collinear;
    else
      return (val > 0) ? clockwise : counter_clockwise;
  }

// Prints convex hull of a set of n points.
template<typename Tp>
  std::vector<Point<Tp>>
  convexHull(std::vector<Point<Tp>> points)
  {
    const int n = points.size();

    // There must be at least 3 points
    if (n < 3)
      return std::vector<Point<Tp>>();

    // Initialize Result
    int next[n];
    for (int i = 0; i < n; i++)
      next[i] = -1;

    // Find the lower leftmost point
    int l = 0;
    for (int i = 1; i < n; i++)
      {
	if (points[i].x < points[l].x)
	  l = i;
	else if (points[i].x == points[l].x && points[i].y < points[l].y)
	  l = i;
      }

    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again
    int p = l, q;
    do
      {
	// Search for a point 'q' such that orientation(p, i, q) is
	// counterclockwise for all points 'i'
	q = (p + 1) % n;
	for (int i = 0; i < n; ++i)
	  if (orientation(points[p], points[i], points[q]) == counter_clockwise)
            q = i;

	next[p] = q; // Add q to result as a next point of p
	p = q; // Set p as q for next iteration
      }
    while (p != l);

    // Store Result
    std::vector<Point<Tp>> hull;
    for (int i = 0; i < n; i++)
      if (next[i] != -1)
	hull.push_back(points[i]);
    return hull;
  }

// Driver program to test above functions
int
main()
{
  using Tp = long long;

  std::vector<Point<Tp>>
  points
  {
    { 0, 3 },
    { 2, 2 },
    { 1, 1 },
    { 2, 1 },
    { 3, 0 },
    { 0, 0 },
    { 3, 3 }
  };

  auto hull = convexHull(points);

  std::cout << "The points in the convex hull are: \n";
  for (auto h : hull)
    std::cout << "(" << h.x << ", " << h.y << ")\n";

  return 0;
}
