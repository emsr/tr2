/*
g++ -o ConvexHullJarvisMarch ConvexHullJarvisMarch.cpp
./ConvexHullJarvisMarch
*/

// A C++ program to find convex hull of a set of points
// Refer http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// for explanation of orientation()

#include <vector>
#include <iostream>

template<typename Tp>
  struct Point
  {
    Tp x;
    Tp y;
  };

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
template<typename Tp>
  int
  orientation(Point<Tp> p, Point<Tp> q, Point<Tp> r)
  {
    auto val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0)
      return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
  }

// Prints convex hull of a set of n points.
template<typename Tp>
  void
  convexHull(std::vector<Point<Tp>> points)
  {
    const int n = points.size();

    // There must be at least 3 points
    if (n < 3)
      return;

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
      for (int i = 0; i < n; i++)
	if (orientation(points[p], points[i], points[q]) == 2)
          q = i;

      next[p] = q; // Add q to result as a next point of p
      p = q; // Set p as q for next iteration
    }
    while (p != l);

    // Print Result
    for (int i = 0; i < n; i++)
    {
      if (next[i] != -1)
	std::cout << "(" << points[i].x << ", " << points[i].y << ")\n";
    }
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

  std::cout << "The points in the convex hull are: \n";
  convexHull(points);

  return 0;
}
