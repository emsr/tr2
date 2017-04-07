/*
g++ -o ConvexHullMonotoneChain ConvexHullMonotoneChain.cpp
./ConvexHullMonotoneChain
*/

// Implementation of Andrew's monotone chain 2D convex hull algorithm.

#include <vector>
#include <algorithm>
#include <iostream>

template<typename Tp>
  struct Point
  {
    Tp x;
    Tp y;

    bool
    operator<(const Point &p) const
    { return x < p.x || (x == p.x && y < p.y); }
  };

// 2D cross product of OA and OB vectors,
// i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
template<typename Tp>
  Tp
  cross(const Point<Tp>& O, const Point<Tp>& A, const Point<Tp>& B)
  { return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x); }

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
template<typename Tp>
  std::vector<Point<Tp>>
  convex_hull(std::vector<Point<Tp>> P)
  {
    int n = P.size(), k = 0;
    std::vector<Point<Tp>> H(2 * n);

    // Sort points lexicographically
    std::sort(P.begin(), P.end());

    // Build lower hull.
    for (int i = 0; i < n; ++i)
    {
      while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0)
	--k;
      H[k++] = P[i];
    }

    // Build upper hull.
    for (int i = n - 2, t = k + 1; i >= 0; i--)
    {
      while (k >= t && cross(H[k - 2], H[k - 1], P[i]) <= 0)
	--k;
      H[k++] = P[i];
    }

    H.resize(k);
    return H;
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
  auto hull = convex_hull(points);
  for (auto h : hull)
    std::cout << "(" << h.x << ", " << h.y << ")\n";

  return 0;
}
