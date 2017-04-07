/*
g++ -Wall -Wextra -o ConvexHullGrahamScan ConvexHullGrahamScan.cpp
./ConvexHullGrahamScan

g++ -std=c++14 -Wall -Wextra -o ConvexHullGrahamScan ConvexHullGrahamScan.cpp
./ConvexHullGrahamScan
*/

// http://www.sanfoundry.com/cpp-programming-examples-computational-geometry-problems-algorithms/

// A C++ program to find convex hull of a set of points
// Refer http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// for explanation of orientation()

#include <vector>
#include <stack>
#include <algorithm>
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
 * A utility function to find next to top in a stack.
 */
template<typename Tp>
  Point<Tp>
  nextToTop(std::stack<Point<Tp>>& S)
  {
    auto p = S.top();
    S.pop();
    auto res = S.top();
    S.push(p);
    return res;
  }

/**
 * Return square of distance between p1 and p2.
 */
template<typename Tp>
  Tp
  dist(Point<Tp> p1, Point<Tp> p2)
  { return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y); }

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
      return (val > 0) ? clockwise : counter_clockwise;
  }

/**
 * A comparison functor for Points.
 */
template<typename Tp>
  struct Compare
  {
    Point<Tp> p0;

    Compare(Point<Tp> center)
    : p0(center)
    { }

    bool
    operator()(Point<Tp> p1, Point<Tp> p2)
    {
      auto orient = orientation(p0, p1, p2);
      if (orient == collinear)
	return dist(p0, p1) < dist(p0, p2);
      else
	return orient == counter_clockwise;
    }
  };

/**
 * Return the convex hull of a set of points.
 */
template<typename Tp>
  std::vector<Point<Tp>>
  convexHull(std::vector<Point<Tp>> point)
  {
    const int n = point.size();

    // There must be at least 3 points.
    if (n < 3)
      return std::vector<Point<Tp>>();

    // Find the bottom-most point.
    auto y_min = point[0].y;
    int i_min = 0;
    for (int i = 1; i < n; ++i)
      {
	auto y = point[i].y;

	// Pick the bottom-most or chose the left-most point in case of tie
	if ((y < y_min) || (y_min == y && point[i].x < point[i_min].x))
	  y_min = point[i_min = i].y;
      }

    // Place the bottom-most point at first position.
    std::swap(point[0], point[i_min]);

    // Sort remaining points with respect to the first point.
    // A point p1 comes before p2 in sorted ouput if p2 has larger polar angle.
    // (in counterclockwise direction) than p1.
    std::sort(point.begin() + 1, point.end(), Compare<Tp>(point[0]));

    // Create an empty stack and push first three points to it.
    std::stack<Point<Tp>> S;
    S.push(point[0]);
    S.push(point[1]);
    S.push(point[2]);

    // Process remaining points.
    for (int i = 3; i < n; ++i)
      {
	// Keep removing top while the angle formed by points next-to-top,
	// top, and point[i] makes a non-left turn
	while (orientation(nextToTop(S), S.top(), point[i]) != counter_clockwise)
	  S.pop();
	S.push(point[i]);
      }

    // Store and return the convex hull.
    std::vector<Point<Tp>> hull;
    while (!S.empty())
      {
	hull.push_back(S.top());
	S.pop();
      }

    return hull;
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

  auto hull = convexHull(point);

  std::cout << "The points in the convex hull are: \n";
  for (auto h : hull)
    std::cout << "(" << h.x << ", " << h.y << ")\n";

  return 0;
}
